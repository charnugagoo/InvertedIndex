//
//  Compress.cpp
//  Index
//
//  Created by Jiankai Dang on 3/6/13.
//  Copyright (c) 2013 Jiankai Dang. All rights reserved.
//

#include "Compress.h"
#include <zlib.h>
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