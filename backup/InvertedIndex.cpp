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
/*
bool build( string file_list) {
	FILE* fr = fopen(file_list.data(), "r");
	if(fr == NULL) return false;
	char a[max_file_name_size], b[max_file_name_size];
	int n;
	fscanf(fr, "%s%s%d", a, b, &n);

	fclose(fr);
}

void look_up() {}

int main() {
	string file_list;
	cin>>file_list;
	build(file_list);

}
*/