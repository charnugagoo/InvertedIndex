#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <string.h>
#include <sstream>
#include <istream>
#include <map>
#include <dirent.h>
#include <zlib.h>
#include <errno.h>
#include "parser.h"



// #if defined __GNUC__ || defined __APPLE__
// #include <ext/hash_map>
// #else
// #include <hash_map>
// #endif

using namespace std;

#define see(x) cout<<#x<<" "<<x<<endl

const int max_file_name_size = 100;

//to-do change map to hash_map

/*****************************************************************************/
//word file

map < string, int> word_set; // word id list
vector<vector<pair<int, int> > > word_doc_cnt; // word -> vector<  <docID, cnt>, <docID, cnt>, <docID, cnt>, ...   >
vector<int> word_doc_cnt_total; // word frequency in all docs.

// lexicon information.
struct lexicon_node {
	int start, length, total; // [start, end)
	string file_name;
	lexicon_node() {
		file_name = "";
		start = -1;
		length = -1;
		total = -1;
	}
	lexicon_node(string a, int b, int c, int d) {
		file_name = a;
		start = b;
		length = c;
		total = d;
	}
	void display() {
		printf("%s %d %d %d\n", file_name.data(), start, length, total);
	}
	void f_display(FILE* fw) {
		fprintf(fw, "%s %d %d %d\n", file_name.data(), start, length, total);
	}
};

bool is_lexicon_resized = false;
vector< lexicon_node > lexicon_list;

// Add a word information into the lexicon.
// @param input_word string The word to be added.
// @param docID int docID of the document in which the word occurs.
void add_word( string input_word, int docID) {
    //	see(input_word);see(docID);
	
	if(!input_word.length()) return;
	map < string, int >::iterator idx;
	idx = word_set.find(input_word);
    
	if(idx == word_set.end()) {
		word_set[input_word] = word_doc_cnt.size();//add new word in word list
		vector<pair<int, int> > temp;
		temp.push_back( make_pair( docID, 1) );
		word_doc_cnt.push_back(temp);
		word_doc_cnt_total.push_back(1);
	} else {
		int id = (*idx).second;
		//in word_doc_cnt[id]
		if(word_doc_cnt[id].size() == 0 || word_doc_cnt[id].back().first != docID) {
            // assume insert all words in one doc at the same time
			word_doc_cnt[id].push_back(make_pair(docID, 1));
			++word_doc_cnt_total[id];
		} else {
			++word_doc_cnt[id].back().second;
			++word_doc_cnt_total[id];
		}
	}
    
	return;
}

// Collect the information of all the words in a particular document.
// @param word_file string The information of all the words in a particular document.
// @param docID int The docID of that document.
void add_word_budget( string word_file, int docID) {
	istringstream str(word_file);
    string sline;
    while(str>>sline) {
 		add_word(sline, docID);
    	str>>sline;
    }
    return;
}

// Search the information of a word.
// @param input_word string The word whose information to be searched.
// @return vector<pair<int, int> > word -> vector<  <docID, cnt>, <docID, cnt>, <docID, cnt>, ...   >
vector<pair<int, int> > find_word_id( string input_word ) {
	map < string, int >::iterator idx;
	idx = word_set.find(input_word);
    
	if(idx == word_set.end()) {
		vector<pair<int, int> > temp;
		return temp;
	} else {
		return word_doc_cnt[ (*idx).second ];
	}
}

// Init lexicon data structure.
void init_lexicon() {
	is_lexicon_resized = true;
	lexicon_list.clear();
	lexicon_list.resize(word_doc_cnt.size());
}

bool save_index_file_split_vector(string filename, vector<int> idx) {
	if(!is_lexicon_resized)
		init_lexicon();//add lexicon process in this function
	string f1 = filename + "d.txt", f2 = filename + "f.txt";
	FILE* fw1 = fopen(f1.data(), "w");
	FILE* fw2 = fopen(f2.data(), "w");
	if(fw1 == NULL || fw2 == NULL) return false;
    
	int pos = 0, l = word_doc_cnt.size();
	for(int j = 0, sz = idx.size() ; j < sz; ++j) {
		int i = idx[j];
		if(i < 0 || i >= l) { continue;}
        
		int ll = word_doc_cnt[i].size();
        
		//lexicon
		int st = pos;
		pos += ll;
		lexicon_list[i].file_name = filename;
		lexicon_list[i].start = st;
		lexicon_list[i].length = ll;
		lexicon_list[i].total = word_doc_cnt_total[i];
        //		see(i);see(st);see(ll);
		
		//write in file
		// fprintf(fw1, "%d %d ", i, ll);
		// fprintf(fw2, "%d %d ", i, ll);
		// printf("%d %d ", i, ll);
		for(int j = 0; j < ll; ++j) {
			fprintf(fw1, "%d ", word_doc_cnt[i][j].first);
			fprintf(fw2, "%d ", word_doc_cnt[i][j].second);
            //			printf("%d %d ", word_doc_cnt[i][j].first, word_doc_cnt[i][j].second);
		}
		fprintf(fw1, "\n");
		fprintf(fw2, "\n");
        //		puts("");
	}
	fclose(fw1);
	fclose(fw2);
	return true;
}

// Save inverted index files. Limit the size of the files by the number of documents processed.
// @param filename string File name.
// @param start int The start position of the information to be processed.
// @param end int The end position of the information to be processed.
// @param max_numbers int The maximum number of documents to be processed.
// @return bool Whether the process is successful.
bool save_index_file_split_by_size(string filename, int start, int &end, int max_numbers) {
	if(!is_lexicon_resized)
		init_lexicon();//add lexicon process in this function
	string f1 = filename + "d.gz", f2 = filename + "f.gz";
    // We keep postings in compressed format on disk even during the index building operation.
	gzFile fw1 = gzopen(f1.data(), "ab");
	gzFile fw2 = gzopen(f2.data(), "ab");
	if(fw1 == NULL || fw2 == NULL) return false;
    
	int pos = 0, l = word_doc_cnt.size(), i, file_numbers = 0;;
	for(i = max(start, 0) ; i < l; ++i) {
        
		int ll = word_doc_cnt[i].size();
        
		//lexicon
		int st = pos;
		pos += ll;
		lexicon_list[i].file_name = filename;
		lexicon_list[i].start = st;
		lexicon_list[i].length = ll;
		lexicon_list[i].total = word_doc_cnt_total[i];
		file_numbers += ll;
		if(file_numbers > max_numbers)
			break;
        //		see(i);see(st);see(ll);
		
		//write in file
		// fprintf(fw1, "%d %d ", i, ll);
		// fprintf(fw2, "%d %d ", i, ll);
		// printf("%d %d ", i, ll);
		for(int j = 0; j < ll; ++j) {
			gzprintf(fw1, "%d ", word_doc_cnt[i][j].first);
			gzprintf(fw2, "%d ", word_doc_cnt[i][j].second);
            //			printf("%d %d ", word_doc_cnt[i][j].first, word_doc_cnt[i][j].second);
		}
		gzprintf(fw1, "\n");
		gzprintf(fw2, "\n");
        //		puts("");
	}
	gzclose(fw1);
	gzclose(fw2);
	end = i;
	return true;
}



bool save_index_file_split_range(string filename, int start, int end) {
    vector<int> t(end - start);
	for(int i = 0; i < end - start; ++i) t[i] = i + start;
	return save_index_file_split_vector(filename, t);
}

lexicon_node find_lexicon_by_word(string word) {
	//should check before, dangerous here
	return lexicon_list[word_set[word]];
}

// Save lexicon file.
// @param lex_file_name string The file path and name of the lexicon file.
// @return bool Whether the process is successful.
bool save_lexicon_file(
                       string lex_file_name = "/Users/jiankaidang/Documents/WebSearchEngines/latest/InvertedIndex/parser/LexiconMetaData.txt") {
	if(is_lexicon_resized == false) return false;
	FILE* fw = fopen(lex_file_name.data(), "w");
	if(fw == NULL) return false;
    
	fprintf(fw, "%d\n", (int)word_set.size());
	// see(word_set.size());
	for(map<string, int>::iterator it = word_set.begin(); it != word_set.end(); ++it) {
		// see(lexicon_list[it->second].start);see(lexicon_list[it->second].length);
		fprintf(fw, "%s %d %s %d %d %d\n",
                it->first.data(), it->second,
				lexicon_list[it->second].file_name.data(),
				lexicon_list[it->second].total, lexicon_list[it->second].start, lexicon_list[it->second].length);
	}
    
	fclose(fw);
	return true;
}

void test_word() {
	string input( "word\nH\napple\nC\ndota\nD\ndota\nR\n" );
	add_word_budget(input, 0);
	string input2( "word\nH\ndota\nD\ndota\nR\nMS\nC\n" );
	add_word_budget(input2, 1);
	// while(cin>>input) {
	// 	vector<pair<int, int> > temp = find_word_id(input);
	// 	see(temp.size());
	// 	for(int i = 0; i < temp.size(); ++i) {see(temp[i].first); see(temp[i].second);}
	// }
	save_index_file_split_range("1.txt", 2, 4);puts("");
	vector<int> temp(2); temp[0] = 0; temp[1] = 1;
	save_index_file_split_vector("2.txt", temp);
	save_lexicon_file("lexicon_file.txt");
	puts("");
}

/*****************************************************************************/
//doc file

struct doc_node {
	string url;
	int PR, size;
	doc_node(){
		url = "";
		size = -1;
		PR = -1;
	}
	doc_node(string a, int b, int c) {
		url = a;
		PR = c;
		size = b;
	}
	void display() {
		printf("%s %d %d\n", url.data(), size, PR);
	}
	void f_display(FILE* fw) {
		fprintf(fw, "%s %d %d\n", url.data(), size, PR);
	}
};

vector<doc_node> doc_list;
map<string, int> doc_set;

//return docID
//given url, size (# of terms), pageRank,
int add_doc( string url, int sz = -1, int PR = -1 ) {
	if(doc_set.find(url) == doc_set.end()) {
		doc_set[url] = doc_list.size();
		doc_list.push_back(doc_node(url, sz, PR));
		return doc_list.size() - 1;
	} else {
		return doc_set[url];
	}
}

// Save the docID-to-URL table file.
// @param filename string The file path and name of the docID-to-URL table file.
// @return bool Whether the process is successful.
bool save_doc_file(string filename = "/Users/jiankaidang/Documents/WebSearchEngines/latest/InvertedIndex/parser/DocMetaData.txt") {
	FILE* fw = fopen(filename.data(), "w");
	if(fw == NULL) return false;
	fprintf(fw, "%d\n", (int)doc_list.size());
	// printf("%d\n", (int)doc_list.size());
	for(map<string, int>::iterator it = doc_set.begin(); it != doc_set.end(); ++it) {
		fprintf(fw, "%d %s %d %d\n", it->second, it->first.data(), doc_list[it->second].size, doc_list[it->second].PR);
        //		printf("%d %s %d %d\n", it->second, it->first.data(), doc_list[it->second].size, doc_list[it->second].PR);
	}
	fclose(fw);
	return true;
}

void test_doc() {
	add_doc("baidu1.com", 101, 21);
	add_doc("baidu2.com", 102, 22);
	add_doc("baidu3.com", 103, 23);
	add_doc("baidu4.com", 104, 24);
	save_doc_file("dic_struc.txt");
	puts("");
}

/*****************************************************************************/
/* HTML Parsing
 
 Load one compressed file into memory and then call library functions to uncompress it into another memory-based buffer.
 Find out where each page starts and ends.
 Determine the boundaries of the page in the file by looking at the page lengths in the corresponding _index file.
 Parse only text in that range, and then parse the next page.
 
 There may be pages with return codes 404 or 304 etc.
 No need to parse such pages!
 Detect and skip these various cases.
 
 Compress and uncompress gzipped files in C/C++ using the zlib library.
 zlib provides a set of gzip File I/O functions for reading and writing gzip files.
 */

// The maximum length of the buffer to read for one line.
#define LENGTH 1000
const int max_file_name = 100;

// @param index_file string The corresponding _index file.
// @param data_file string The corresponding _data file.
// @return vector<pair<string, doc_node>> Pairs of <words and their contexts, doc_node>.
vector<pair<string, doc_node>> parse (string index_file, string data_file)
{
    vector<pair<string, doc_node> >res;
    gzFile file;
    // Opens a gzip (.gz) file for reading or writing.
    file = gzopen (index_file.data(), "rb");
    if (! file) {
        fprintf (stderr, "gzopen of '%s' failed: %s.\n", index_file.data(),
                 strerror (errno));
        return res;
    }
    gzFile file_data;
    // Opens a gzip (.gz) file for reading or writing.
    file_data = gzopen (data_file.data(), "r");
    if (! file_data) {
        fprintf (stderr, "gzopen of '%s' failed: %s.\n", data_file.data(),
                 strerror (errno));
        return res;
    }
    while (1) {
        char buffer[LENGTH];
        //Reads bytes from the compressed file until LENGTH-1 characters are read, or a newline character is read and transferred to buf, or an end-of-file condition is encountered.
        if(0==gzgets(file, buffer, LENGTH)) {
            break;
        }
        char url [20], s1[20], s2[20];
        int length = 0;
        int i, j, m;
        //        printf ("%s", buffer);
        
        sscanf(buffer, "%s %d %d %d %s %d %s", url, &i, &j, &length, s1, &m, s2);
        // The buffer of page data.
        char data_buffer[length];
        // Reads the given number of uncompressed bytes from the compressed file.
        gzread(file_data, data_buffer, length);
        
        //        printf ("%s\n", data_buffer);
        char *pool;
        int ret;
        
        pool = (char*)malloc(2*length+1);
        
        // parsing page
        ret = parser(url, data_buffer, pool, 2*length+1);
        
        // output words and their contexts
        //        if (ret > 0) printf("%s", pool);
        int t = 0;
        for(int x = 0, y = strlen(pool); x < y; ++x)
        	if(pool[x] == '\n')
				++t;
        //        see(pool);see(t);
        res.push_back(   make_pair( string(pool) , doc_node(url, t, -1 ) ) );
        
        free(pool);
        
    }
    gzclose (file);
    gzclose (file_data);
    return res;
}
//main

/*
 vector<string> all_files_in_folder(string path = "/Users/charnugagoo/Dropbox/Study/WebSearchEngine/InvertedIndex/WSE-data") {
 vector<string> res;
 DIR * dir = opendir(path.data());
 if(dir == NULL) return res;
 struct dirent *ent;
 while ((ent = readdir (dir)) != NULL) {
 printf ("%s\n", ent->d_name);
 res.push_back(string(ent->d_name));
 }
 closedir (dir);
 }
 */

vector<pair<string, string> > generate_file_name(
                                                 string path = "/Users/jiankaidang/Documents/WebSearchEngines/latest/InvertedIndex/WSE-data/",
                                                 int start = 0, int end = 83) {
	vector<pair<string, string> > res;
	for(int i = start; i < end; ++i) {
		
		string temp;
		std::stringstream out;
		out << i;
		temp = out.str();
        
		string a = path + temp + "_index";
		string b = path + temp + "_data";
		res.push_back(  make_pair(a, b) );
	}
	return res;
}

//compress




const int inverted_index_split_number = 2000000;
const string inverted_index_file_name = "/Users/jiankaidang/Documents/WebSearchEngines/latest/InvertedIndex/parser/InvertedIndex/";

int main() {
	vector<pair<string, string> > data_file = generate_file_name();
	for(int i = 0, l = data_file.size(); i < l; ++i) {
		vector<pair< string, doc_node> > url_doc_pair = parse(data_file[i].first, data_file[i].second);
    	for(int j = 0, ll = url_doc_pair.size(); j  < ll; ++j) {
    		int doc_id = add_doc(url_doc_pair[j].second.url, url_doc_pair[j].second.size, -1);
			// see(doc_id);
			
			add_word_budget( url_doc_pair[j].first, doc_id);
			
    	}
        printf("Reading Data File %d ...\n", i);
	}
    for(int i = 0, j = 0,
        l = word_set.size(); i < l; ++j) {
        
    	string temp;
       	std::stringstream out;
		out << j;
		temp = out.str();
        
        int end;
        
        save_index_file_split_by_size(inverted_index_file_name + temp,
                                      i, end, inverted_index_split_number);
        printf("Saving Index File %d from %dth to %dth...\n", j, i, end);
        i = end;
	}	
    save_doc_file();puts("Saving Doc Meta Data...");
    save_lexicon_file();puts("Saving Lexicon Meta Data...");
    see(word_set.size());
	return 0;
}

