//
//  Compress.cpp
//  Index
//
//  Created by Jiankai Dang on 3/6/13.
//  Copyright (c) 2013 Jiankai Dang. All rights reserved.
//

#include "Compress.h"
#include <zlib.h>
#include <sstream>
using namespace std;
// Compress a file.
// @param infilename input file name
// @param outfilename output file name
void compress_one_file(const char *infilename, const char *outfilename){
    FILE *infile = fopen(infilename, "rb");
    if (infile == NULL) {
        return;
    }
    gzFile outfile = gzopen(outfilename, "wb");
    if (!infile || !outfile) return;
    char inbuffer[128];
    int num_read = 0;
    while ((num_read = fread(inbuffer, 1, sizeof(inbuffer), infile)) > 0) {
        gzwrite(outfile, inbuffer, num_read);
    }
    fclose(infile);
    gzclose(outfile);
}

void compress_all_files(string path = "/Users/charnugagoo/Dropbox/Study/WebSearchEngine/InvertedIndex/parser/InvertedIndex/") {
    for (int i = 0; i < 90; i++) {
        string temp;
        
		std::stringstream out;
		out << i;
		temp = out.str();
        
		string a = path + temp + ".txt";
        string c = path + temp + ".gz";
        compress_one_file(a.c_str(), c.c_str());
    }
}