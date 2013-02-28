
Files parser.c and parser.h contain a very simple parser for HTML files.
File test.c has a little test program that reads the page stored in file
sample.html and parses it.

The parser takes as input a string containing the ENTIRE page (including
HTTP header), and also a string containing the URL. The parser then parses
both the page and the URL (so if the url is www.shoes.com, the words www,
shoes, and com will be prased out). The result is returned as one string
of the form
<word> <context> (line break) 
<word> <context> (line break)
 ....

For example, a page with URL "http://fun.com/fun.html" and with content 
"This is <b>so</b> much <b><em>fun</em></b>" will result in the output

http U
fun U
com U
fun U
html U
this P
is P
so B
much P
fun BI

where P stands for plain context, U for "in the URL", B for bold, I for 
italic (<em>). There are other contexts for title and headings. The result 
is returned inside a single character array that has to be allocated outside 
the parser and passed into it, with an integer (the last parameter for the
parser) specifying the size of this array so that the parser knows if the
output fits into the array. You can then easily process this output to
generate postings.

There are ways to call C/C++ procedures and programs from Java or Python
which you should be able to figure out. If you do, help others by explaining
how you did it. If you cannot do it, ask us for help.
 
Note: Thanks to Xiaohui Long for the code. Please send emails concerning
bugs to suel@poly.edu. (I may or may not be able to help myself, but I can 
ask Xiaohui who has graduated and left.)
