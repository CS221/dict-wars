# Marked below in the "files" section are the lines you need to
# change. Also, to make your code run as fast as possible, change the
# flags option from debugging to optimizing below and do a clean make.

####### FLAGS

CC      = g++
LIBS    = -lm
BASE_DIR = .
INCLUDES = -I$(BASE_DIR)

# Debugging flags, remove '#' to activate.
#CFLAGS = -Wall -g -std=c++0x $(INCLUDES)

# Optimizing flags, remove '#' to activate.
#CFLAGS	= -Wall -O6 -DNDEBUG $(INCLUDES)

# Profiling flags, remove '#' to activate.
#CFLAGS	= -Wall -pg -O6 -DNDEBUG $(INCLUDES)

# Default compiles for the dictwars version.
#CFLAGS	= -g -DUSERID='"cs221"' -DINTERROGATE -Wall -O0 -std=c++0x $(INCLUDES)
CFLAGS	= -DUSERID='"cs221"' -DINTERROGATE -DNDEBUG -Wall -O2 -std=c++0x $(INCLUDES)
#CFLAGS	= -DUSERID='"cs221"' -DNDEBUG -Wall -O2 -std=c++0x $(INCLUDES)
#CFLAGS	= -g -Wall -O2 $(INCLUDES)

####### Files

# The name of the target file
TARGET  = dict_runner

# All headers in the project. Add your new header files here.
# Note that template .cpp files that are #include'd somewhere should go here
HEADERS = Dictionary.h BinarySearchTree.h SplayTree.h HashTableQP.h AvlTree.h Trie.h UnsortedListDict.h Timer.h HashTableChaining.h

# All source files in the project. Add your new source files here.
SOURCES = dict_runner.cpp Dictionary.h BinarySearchTree.h SplayTree.h HashTableQP.h AvlTree.h Trie.cpp UnsortedListDict.h Timer.h Timer.cpp 

# All object files in the project. Add .o versions of all your
# _untemplated_ .cpp files (probably no new files here for this project).
OBJECTS = dict_runner.o Timer.o 

####### Implicit rules
.SUFFIXES:

.SUFFIXES: .c .cc .cpp .o

.c.o:; $(CC) -c $(CFLAGS) $<
.cpp.o:; $(CC) -c $(CFLAGS) $<
.cc.o:; $(CC) -c $(CFLAGS) $<

####### Build rules
# No changes should be necessary in this section.

all: $(TARGET)

$(TARGET): $(OBJECTS) $(HEADERS)
	$(CC) $(CFLAGS) $(LIBS) -o $(TARGET) $(OBJECTS)
	strip -s $(TARGET)  # Unwise to use with debugging!

purip: $(OBJECTS)
	purify $(CC) -o purip $(OBJECTS) $(CFLAGS) $(LIBS)

$(OBJECTS): $(HEADERS)

show:
	@echo $(SOURCES) $(HEADERS)

clean:
	rm -f *.o *~ 

full_clean: clean
	rm -f $(TARGET)
