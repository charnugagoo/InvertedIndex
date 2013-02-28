 
Note: this modification of the parser was provided by Chi Zhang, and it 
helped him avoid the crashes he experiences with the other parser. This
modified parser will check for the length of the document, not just for
'\0'. (Note however that for the original parser, you should not pass the
entire file, but only the part for one file. So if you make sure there is
a '\0' at the end, I think the original parser also does fine.)

Here are Chi's comments: (Thanks, Chi!)

The parser will continuously consume the char and parse it until a '\0' is 
met. It assumes that the to-be-parsed-buffer is a '\0' ended string.
 
There are two problems:
1. what if the to-be-parsed-buffer have a '\0' in the middle? (it's possible 
because we are talking about binary here although I haven't seen such situation)
2. what if the calling process passes in a buffer without '\0'? (this is what 
I did: I passed in the whole buffer of unzipped multi-pages file, the parser 
failed to stop and caused a data exception)
My fix is simple: not touching the prototype of the function, change the stop 
condition of the while loop from while(*p != '\0') to while ((p - doc) < maxlen);
in another word, using the known to-be-parsed length as the stop condition.
 
Chi

