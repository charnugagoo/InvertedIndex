What can the program do?

The program creates an inverted index structure, plus structures for the lexicon and for the docID-to-URL table from a set of downloaded web pages.
At the end of the program, all structures are stored on disk in binary data formats for these structures.
The program is able to to deal with data which is provided in a fairly raw form with various errors (truncated web pages, 404 errors).

Our inverted index is structured such that we can read a particular inverted list without reading the rest of the index, by looking up the start of the inverted list in the lexicon structure.

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

-----------------------------------------

What are the major functions and modules?

1. vector<pair<string, doc_node>> parse (string index_file, string data_file);

HTML Parsing.
@param index_file string The corresponding _index file.
@param data_file string The corresponding _data file.
@return vector<pair<string, doc_node>> Pairs of <words and their contexts, doc_node>.

2. bool save_index_file_split_by_size(string filename, int start, int &end, int max_numbers);

Save inverted index files. Limit the size of the files by the number of documents processed.
@param filename string File name.
@param start int The start position of the information to be processed.
@param end int The end position of the information to be processed.
@param max_numbers int The maximum number of documents to be processed.
@return bool Whether the process is successful.

3. bool save_lexicon_file(string lex_file_name);

Save lexicon file.
@param lex_file_name string The file path and name of the lexicon file.
@return bool Whether the process is successful.

4. bool save_doc_file(string filename);

Save the docID-to-URL table file.
@param filename string The file path and name of the docID-to-URL table file.
@return bool Whether the process is successful.

5. void add_word( string input_word, int docID);

Add a word information into the lexicon.
@param input_word string The word to be added.
@param docID int docID of the document in which the word occurs.

6. void add_word_budget( string word_file, int docID);

Collect the information of all the words in a particular document.
@param word_file string The information of all the words in a particular document.
@param docID int The docID of that document.

7. vector<pair<int, int> > find_word_id( string input_word );

Search the information of a word.
@param input_word string The word whose information to be searched.
@return vector<pair<int, int> > word -> vector<  <docID, cnt>, <docID, cnt>, <docID, cnt>, ...   >

-----------------------------------------------

How long does it take on the provided data set?

----------------------------------------

How large are the resulting index files?

-----------------------

How to run the program?

We use Xcode to develop our project.
To run the program, you could open Xcode project file: "parser.xcodeproj".
Then build and run the program in Xcode.