
Using the C parser in Python and Java:
**************************************

In Python:
-----

To call the parsing function in "parser.c" from a Python program, use the 
wrapper provided in "parsermodule.c". The file "try.py" gives an example on 
how to call wrapper.c from python (it assumes that the file "sample.html" is 
in the same directory). Here is how to compile the wrapper and how to 
execute "try.py":

On pdc-lab:

gcc -c -fpic -I/usr/local/Python/include/python2.2 parsermodule.c parser.c

gcc -shared parser.o parsermodule.o -o parsermodule.so

python try.py 


On bumblebee: first edit parsermodule.c and adjust the first #define in that
              file as described in the comments, and then do the following
              (use Python 2.3 instead of "python" which links to Python 2.0):

gcc -c -fpic -I/usr/local/Python-2.3.2/include/python2.3 parsermodule.c parser.c

gcc -shared parser.o parsermodule.o -o parsermodule.so

/opt/Python-2.3.2/bin/python  try.py


In Java:
--------

I don't have a wrapper ready for the parser, but you can use the JNI to call
C functions from Java. See http://java.sun.com/docs/books/tutorial/native1.1/
for more information.

