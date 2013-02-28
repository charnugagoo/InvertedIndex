
This is concerning the code in files 

  parser-revised-again.cpp and
  parser-revised-again.h.

which fixes other issues with the provided C parser.  
Thanks to Chi and Felix for help on this!

--------------------------------------------------------

Date: Fri, 26 Oct 2007 08:58:10 +0000
From: czhang03@students.poly.edu

Hi Professor Suel,

Sorry to send you another email before my homework submission, but I think
maybe this is helpful to those students who are using the Microsoft Visual
Studio as their IDE. Maybe the parser doesn't have such problems on Unix.

When running on the bigger data set, I found two more bugs (the first one may
not be too harmful, the second one cause a crash on Windows if the program is
developed on VC++):

1. when the parser meet '>' it will assume that it's the end of a TAG and start
to parse the tag, what if the '>' is just part of a corrupted text? The fix is
after detecting '>', check whether '<' was detected before.
2. In the function of tag_parser, it will check whether certain character is a
space, by calling a standard API: isspace. If that particular character is a
garbage one, the program will crash due to the isspace implementation of VC++.
The fix is: I add a new macro called xl_isascii, before checking whether that
character is space, check whether it's a garbage first.

I also re-prototyped the function parser() to make the previous fix not
ambiguous.

Chi 

------------------------------------------------------------------------

Date: Fri, 26 Oct 2007 15:14:02 +0000
From: czhang03@students.poly.edu

Hi Professor Suel,

For the all the bug fixes, they work for both of VC and gcc.

There are two places I tweaked for VC which I am not sure whether they fit for
gcc:

1. To compile the function isspace, I add a  header: #include <locale>
2. replace strncmp with strnicmp; strnicmp is a  case-insensitive version of
strncmp, not sure whether gcc recognizes it.

Chi

----------------------------------------------------------------------------

Date: Thu, 25 Oct 2007 03:10:44 -0400
From: ffiloz01@students.poly.edu

The provided parser has a problem dealing with
input that looks like "HTTP HEADER\r\n\r\n Foo>Bar".

Notice that > character has no corresponding < character. This causes
tag_parser() to receive a pointer that points to nowhere. As a result the
program segfaults.

The problem occurs between lines 159 - 180. If < never occured, but >
does, then you can see from the code that ptag is never set to p.

My solution was to initilize ptag to NULL. Then add the following after
line 170:

if (ptag == NULL) {
?? p++;
?? continue;
}

I had this problem with some data in 16_data. That took care of the
problem.

I don't know the code well enough, but that seemed to fix the problem.
I'm hoping that skipping over the character doesn't mess up some counter
downwards.


