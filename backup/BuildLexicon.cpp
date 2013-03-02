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

const int max_file_name_size = 100;

//to-do change map to hash_map

map < string, int> word_set; // word id list 
//shuold be changed to hash_map

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

vector< lexicon_node > lexicon_list;

bool load_lexicon_file(string lexicon_file_name) {
	FILE* fw = fopen(lexicon_file_name.data(), "r");
	if(fw == NULL) return false;

	int n;
	fscanf(fw, "%d", &n);

	lexicon_list.resize(n);	

	for(int i = 0; i < n; ++i) {
		char word_name[max_file_name_size], filename[max_file_name_size];
		int word_id, st, leng;
		fscanf(fw, "%s %d %s %d %d", word_name, &word_id, filename, &st, &leng);
		printf("%s %d %s %d %d\n", word_name, word_id, filename, st, leng);
		word_set[string(word_name)] = word_id;

		lexicon_list[word_id].file_name = string(filename);
		lexicon_list[word_id].start = st;
		lexicon_list[word_id].length = leng;

	}

	return true;
}

lexicon_node find_lexiton_by_word_id(int id) {
	if(id >= lexicon_list.size() || id < 0) {
		return lexicon_node("", -1, -1);
	}
	return lexicon_list[id];
}

lexicon_node find_lexiton_by_word(string word) {
	if(word_set.find(word) == word_set.end()) {
		return lexicon_node("", -1, -1);
	}
	return lexicon_list[ word_set[word] ];
}

int main() {
	load_lexicon_file("lexicon_file.txt");
	string in;
	while(cin>>in) {
		lexicon_node a = find_lexiton_by_word(in);
		a.display();
	}
	return 0;
}