/*******Old help text:*******
Seriously: The help text below will list flags that you SHOULD be able
to use to run specific dictionaries.  For example, you SHOULD be able
to run a binary search tree with -bst.  Unfortunately, the executable
has been mixed up so that none of those flags work.  Instead, you have
mys01, mys02, mys03, ...  Each of the "mystery" flags uniquely
corresponds to exactly one of the real flags.  Your job is to
determine that correspondence and back up your determination with
strong, clear evidence.
*/

const char * mystery_help_text = R"HELP(

================= COLORFUL MOTIVATING STORY =========================

Oh no!  Swiper has stolen the dictionary implementations' names!  Can
you help Dora find the names?  Say "Swiper, no swiping!" and then help
Dora figure out which is which.

It doesn't matter for the project, but if you don't know who Dora and
Swiper are, see: http://www.nickjr.co.uk/shows/dora/index.aspx

P.S. If you are familiar with Dora, get frustrated with the project,
and need inspiration, try:
http://uncyclopedia.wikia.com/wiki/File:Dora_Kills_Swiper.jpg

=============== END COLORFUL MOTIVATING STORY =======================

As per the colourful motivating story, this program was originally 
able to execute operations on dictionary/set data structures given 
their name. But the flags were swapped out with mystery names in the 
format mysXX (XX is a number: 01, 02, ...), which uniquely correspond 
to one dictionary/set implementation, each. Your job is to determine 
which implementation (listed on the assignment page) corresponds to 
each of the mysXX.

For hints getting started, see:
http://www.ugrad.cs.ubc.ca/~cs221/current/fun/description

See below for help on running this command and the underlying
implementations.

)HELP";

const char * help_text = R"HELP(
The program accepts input where each line must follow one of the
following three forms:

I <key>
F <key>
R <key>

Therefore, every line should start with one of the three letters I, F,
or R, then have a space, then have a single number greater than zero,
and nothing else.

Keys are positive integers.  (Stored as either 32 or 64 bit integers.)

'I' means insert.  The behaviour when inserting a key already present
in the dictionary is undefined.

'F' means find.  (This performs a find operation but ignores the
result.  It can be very useful for distinguishing dictionary
implementations, however!)

'R' means remove/delete.  The behaviour when deleting a key not
already in the dictionary is undefined.

Sends its output to file(s) named after the flag of the dictionary or
dictionaries you choose to run.  (You may run multiple at one time to
avoid rereading the input, which can take a long time for, say,
millions of operations.)  

In the output, each line represents a timestamp.  A line will contain
two numbers.  The first is the number of the NEXT command the
dictionary will perform.  (So, when that number is 1, the dictionary
has not yet started.  When that number is 10001, the dictionary just
performed the 10000th operation and is about to perform the 10001st
operation.)

The second number is the amount of time (in ticks of an arbitrary
clock) since the dictionary started running (NOT just since the last
timestamp!).

We guarantee that you'll get AT LEAST one timestamp before the first
operation and one timestamp just after the final operation.
Otherwise, you'll get about one every 1000 steps, but we only report a
new timestamp when the time has measurably changed (i.e., the clock
has ticked).

)HELP";
