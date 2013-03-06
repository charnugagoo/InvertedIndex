What the program can do?

The program creates an inverted index structure, plus structures for the lexicon and for the docID-to-URL table from a set of downloaded web pages.
At the end of the program, all structures are stored on disk in binary data formats for these structures.
The program is able to to deal with data which is provided in a fairly raw form with various errors (truncated web pages, 404 errors).
To parse HTML pages, the program uses the C/C++ based parser.