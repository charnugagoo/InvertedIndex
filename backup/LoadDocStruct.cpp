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

const int max_url_size = 1000;

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

bool load_lexicon_file(string doc_struct_file_name) {
	FILE* fr = fopen(doc_struct_file_name.data(), "r");
	if(fr == NULL) return false;
	int n;
	fscanf(fr, "%d", &n);
	doc_list.resize(n);

	char url[max_url_size];
	for(int i = 0; i < n; ++i) {
		int id, size, pr;
		fscanf(fr, "%d %s %d %d", &id, url, &size, &pr);
		printf("%d %s %d %d\n", id, url, size, pr);

		doc_set[string(url)] = id;

		doc_list[id].url = string(url);
		doc_list[id].size = size;
		doc_list[id].PR = pr;
	}

	return true;
}

doc_node find_doc_by_id(int id) {
	if(id >= doc_list.size()) {
		return doc_node("", -1, -1);
	}
	return doc_list[id];
}

doc_node find_doc_by_url(string url) {
	if(doc_set.find(url) == doc_set.end()) {
		return doc_node("", -1, -1);
	}
	return doc_list[ doc_set[url] ];
}

int main() {
	load_lexicon_file("dic_struc.txt");
	int in;
	while(cin>>in) {
		doc_node a = find_doc_by_id(in);
		a.display();
	}
	return 0;
}