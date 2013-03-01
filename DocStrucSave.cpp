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
		PR = b;
		size = c;
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

bool save_doc_file(string filename) {
	FILE* fw = fopen(filename.data(), "w");
	if(fw == NULL) return false;
	fprintf(fw, "%d\n", (int)doc_list.size());
	printf("%d\n", (int)doc_list.size());
	for(map<string, int>::iterator it = doc_set.begin(); it != doc_set.end(); ++it) {
		fprintf(fw, "%d %s %d %d\n", it->second, it->first.data(), doc_list[it->second].size, doc_list[it->second].PR);
		printf("%d %s %d %d\n", it->second, it->first.data(), doc_list[it->second].size, doc_list[it->second].PR);	
	}
	return true;
}

int main() {
	add_doc("baidu1.com", 101, 21);
	add_doc("baidu2.com", 102, 22);
	add_doc("baidu3.com", 103, 23);
	add_doc("baidu4.com", 104, 24);
	save_doc_file("dic_struc.txt");
	return 0;
}