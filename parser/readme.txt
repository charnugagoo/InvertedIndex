What can the program do?

The program creates an inverted index structure, plus structures for the lexicon and for the docID-to-URL table from a set of downloaded web pages.
At the end of the program, all structures are stored on disk in binary data formats for these structures.
The program is able to to deal with data which is provided in a fairly raw form with various errors (truncated web pages, 404 errors).

-----------------------

How to run the program?

----------------------------

How does it work internally?

We use C++ for this project.

To parse HTML pages, the program uses the C++ based parser.
Load one compressed file into memory and then call library functions to uncompress it into another memory-based buffer.
Find out where each page starts and ends.
Determine the boundaries of the page in the file by looking at the page lengths in the corresponding _index file.
Parse only text in that range, and then parse the next page.
There may be pages with return codes 404 or 304 etc.
No need to parse such pages!
Detect and skip these various cases.
 
We uncompress the files during parsing, by loading one compressed file into memory and then calling the right library function to uncompress it into another memory-based buffer, and find out where each page starts and ends. 
Compress and uncompress gzipped files in C++ using the zlib library.
zlib provides a set of gzip File I/O functions for reading and writing gzip files.
We store the final inverted lists in compressed form.
We keep postings in compressed format on disk even during the index building operation.

We assign docIDs in the order in which the pages are parsed.
The final inverted lists do not have the words or word IDs inside each posting anymore.

-----------------------------------------------

How long does it take on the provided data set?

----------------------------------------

How large are the resulting index files?

-----------------------------------------

What are the major functions and modules?

1. vector<pair<string, doc_node>> parse (string index_file, string data_file);

HTML Parsing.
@param index_file string The corresponding _index file.
@param data_file string The corresponding _data file.
@return vector<pair<string, doc_node>> Pairs of <words and their contexts, doc_node>.