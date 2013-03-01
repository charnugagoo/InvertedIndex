#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include <errno.h>
#include <string.h>
#include "parser.h"
#include <vector>
#include <string>

using namespace std;

//static const char * file_name = "/Users/jiankaidang/Google Drive/WebSearchEngines/nz2_merged/0_index";
//static const char * data_file = "/Users/jiankaidang/Google Drive/WebSearchEngines/nz2_merged/0_data";

/* Size of the block of memory to use for reading. */
#define LENGTH 1000

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

const int max_file_name = 100;

vector<pair< string, doc_node> > parse (string index_file, string data_file )
{
    vector<pair<string, doc_node> >res;
    gzFile file;
    file = gzopen (index_file.data(), "r");
    if (! file) {
        fprintf (stderr, "gzopen of '%s' failed: %s.\n", index_file.data(),
                 strerror (errno));
        return res;
    }
    gzFile file_data;
    file_data = gzopen (data_file.data(), "r");
    if (! file_data) {
        fprintf (stderr, "gzopen of '%s' failed: %s.\n", data_file.data(),
                 strerror (errno));
        return res;
    }
    while (1) {
        char buffer[LENGTH];
        
        if(0==gzgets(file, buffer, LENGTH)) {
            break;
        }
        char url [20], s1[20], s2[20];
        int length = 0;
        int i, j, m;
        printf ("%s", buffer);
        
        sscanf(buffer, "%s %d %d %d %s %d %s", url, &i, &j, &length, s1, &m, s2);
        char data_buffer[length];
        gzread(file_data, data_buffer, length);
        
        printf ("%s\n", data_buffer);
        char *pool;
        int ret;
        
        pool = (char*)malloc(2*length+1);
        
        // parsing page
        ret = parser(url, data_buffer, pool, 2*length+1);
        
        // output words and their contexts
        if (ret > 0)
            printf("%s", pool);
            
        free(pool);
        res.push_back(   make_pair( string(pool) , doc_node(url, length, -1 ) ) );
    }
    gzclose (file);
    return res;
}