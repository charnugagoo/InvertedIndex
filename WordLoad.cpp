#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <string.h>
#include <sstream>
#include <istream>
#include <map>

// #if defined __GNUC__ || defined __APPLE__
// #include <ext/hash_map>
// #else
// #include <hash_map>
// #endif

using namespace std;

#define see(x) cout<<#x<<" "<<x<<endl

//to-do lexicon
//to-do word pre-computing

map < string, int> word_set; // word id list
vector<vector<pair<int, int> > > word_doc_cnt; // word -> vector<  <docID, cnt>, <docID, cnt>, <docID, cnt>, ...   >

struct lexicon_node {
	int start, length; // [start, end)
	string file_name;
	lexicon_node() {
		file_name = "";
		start = -1;
		length = -1;
	}
	lexicon_node(string a, int b, int c) {
		file_name = a;
		start = b;
		length = c;
	}
	void display() {
		printf("%s %d %d\n", file_name.data(), start, length);
	}
	void f_display(FILE* fw) {
		fprintf(fw, "%s %d %d\n", file_name.data(), start, length);
	}
};

bool is_lexicon_resized = false;
vector< lexicon_node > lexicon_list;

void add_word( string input_word, int docID) {
	if(!input_word.length()) return;
	map < string, int >::iterator idx;
	idx = word_set.find(input_word);

	if(idx == word_set.end()) {
		word_set[input_word] = word_doc_cnt.size();//add new word in word list
		vector<pair<int, int> > temp;
		temp.push_back( make_pair( docID, 1) );
		word_doc_cnt.push_back(temp);
	} else {
		int id = (*idx).second;
		//in word_doc_cnt[id]
		if(word_doc_cnt[id].size() == 0 || word_doc_cnt[id].back().first != docID) { // assume insert all words in one doc at the same time
			word_doc_cnt[id].push_back(make_pair(docID, 1));
		} else {
			++word_doc_cnt[id].back().second;
		}
	}

	return;
}


void add_word_budget( string word_file, int docID) {
	istringstream str(word_file);
    string sline;
    while(str>>sline) {
 		add_word(sline, docID);
    	str>>sline;
    }
    return;
}

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

void init_lexicon() {
	is_lexicon_resized = true;
	lexicon_list.clear();
	lexicon_list.resize(word_doc_cnt.size());
}

bool save_index_file_split_vector(string filename, vector<int> idx) {
	if(!is_lexicon_resized)
		init_lexicon();//add lexicon process in this function
	string f1 = "d" + filename, f2 = "f" + filename;
	FILE* fw1 = fopen(f1.data(), "w");
	FILE* fw2 = fopen(f2.data(), "w");
	if(fw1 == NULL || fw2 == NULL) return false;

	int pos = 0, l = word_doc_cnt.size();
	for(int j = 0, sz = idx.size() ; j < sz; ++j) {
		int i = idx[j];
		if(i < 0 || i >= l) { puts("pointer out of memory error 1!"); continue;}


		int ll = word_doc_cnt[i].size();

		//lexicon
		int st = pos;
		pos += ll;
		lexicon_list[i].file_name = filename;
		lexicon_list[i].start = st;
		lexicon_list[i].length = ll;
		// see(i);see(st);see(ll);
		
		//write in file
		// fprintf(fw, "%d %d ", i, ll);
		printf("%d %d ", i, ll);
		for(int j = 0; j < ll; ++j) {
			fprintf(fw1, "%d ", word_doc_cnt[i][j].first);
			fprintf(fw2, "%d ", word_doc_cnt[i][j].second);
			printf("%d %d ", word_doc_cnt[i][j].first, word_doc_cnt[i][j].second);
		}
		fprintf(fw1, "\n");
		fprintf(fw2, "\n");
		puts("");
	}
	fclose(fw1);
	fclose(fw2);
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

bool save_lexicon_file(string lex_file_name) {
	if(is_lexicon_resized == false) return false;
	FILE* fw = fopen(lex_file_name.data(), "w");
	if(fw == NULL) return false;

	fprintf(fw, "%d\n", (int)word_set.size());
	see(word_set.size());
	for(map<string, int>::reverse_iterator it = word_set.rbegin(); it != word_set.rend(); ++it) {
		// see(lexicon_list[it->second].start);see(lexicon_list[it->second].length);
		fprintf(fw, "%s %d %s %d %d\n", 
			it->first.data(), it->second, lexicon_list[it->second].file_name.data(), lexicon_list[it->second].start, lexicon_list[it->second].length);
	}

	fclose(fw);
	return true;
}
int main() {
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
}