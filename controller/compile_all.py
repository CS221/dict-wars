#!/usr/bin/env python

import sys #would like to use argparse, but don't have it.
import re
import shutil
import os
import os.path
import signal
import subprocess
import threading

from constants import *

def usage():
    print "Usage: %s <user_id>" % sys.argv[0]
    print "Where <user_id> is a four-letter student account ID"

# Command line arguments.
if len(sys.argv) != 2:
    print "Wrong number of arguments supplied when running: %s" % (" ".join(sys.argv))
    usage()
    sys.exit(1)

#for arg in sys.argv[1:]:
user_id = sys.argv[1]   

# Clean argument.
if re.match(r"^[a-zA-Z0-9._-]+$", user_id) is None:
    print "User ID must be at least one letter long and " + \
          "contain only alphanumerics, dash, dot, and underscore."
    print "Given user ID was: \"%s\"." % user_id
    usage()
    exit(1)

TARGET_DIR=os.path.normpath(os.path.join(EXE_BASE_DIR, user_id))
SOURCE_DIR=os.path.normpath(os.path.join(SOURCE_BASE_DIR, user_id))

class LanguageCompilationUnit:
    """A language is:

- Name: "C++"
- Extension list: ["cc", "cpp", "C"]
- Command generator: makes the list of commands to run, given exe_path and file_path"""
    
    def __init__(self, name, ext_list, cmd_gen):
        self.name = name
        self.ext_list = ext_list
        self.generate_command = cmd_gen

    def partition_file_list(self, files):
        re_lang_files = re.compile(r'^.+\.(' + "|".join(self.ext_list) + r')$')
        return (filter(lambda f: re_lang_files.match(f) is not None, files),
                filter(lambda f: re_lang_files.match(f) is None, files))

# Note: -Os optimizes for size and otherwise at the -O2 level.
CPP_LANGUAGE = LanguageCompilationUnit("C++", ["cc", "cpp", "C"], 
                                       (lambda exe_path, file_path: ["g++", "-Wall", "-Os", "-o", exe_path, file_path]))

# Note: -Os optimizes for size and otherwise at the -O2 level.
C_LANGUAGE = LanguageCompilationUnit("C", ["c"], 
                                     lambda exe_path, file_path: ["gcc", "-Wall", "-Os", "-o", exe_path, file_path])


def test_LCU():
    assert CPP_LANGUAGE.partition_file_list(["a.cc", ".cc", "cbc.cc", "a.cpp", ".cpp", "cbc.cpp", "a.c", ".c", "cbc.c", "a.ccc"]) == \
        (["a.cc", "cbc.cc", "a.cpp", "cbc.cpp"], [".cc", ".cpp", "a.c", ".c", "cbc.c", "a.ccc"]), "partition_file_list failure"
    assert C_LANGUAGE.generate_command("abc", "def") == ["gcc", "-Wall", "-Os", "-o", "abc", "def"], \
        "generating C compile command failed"
    assert CPP_LANGUAGE.generate_command("abc", "def") == ["g++", "-Wall", "-Os", "-o", "abc", "def"], \
        "generating C++ compile command failed"
    #assert False, "forcing stop to run"


LANGUAGES = [CPP_LANGUAGE, C_LANGUAGE]

# Make sure the student has a directory.
shutil.rmtree(TARGET_DIR, True)  # Remove the directory, ignoring errors.
os.makedirs(TARGET_DIR)

def try_to_kill(process, error_msg):
    try:
        #process.kill()
        # Older version alternative to p.kill.
        os.kill(process.pid, signal.SIGKILL)
        print error_msg
    except OSError:
        pass

def process_files_for_language(src_files, language):
    # Grab just the .cpp files.
    (src_files, not_src_files) = language.partition_file_list(src_files)

    # Compile each cpp file.
    for src_file in src_files:
        src_path = os.path.join(SOURCE_DIR, src_file)
        exe_path = os.path.join(TARGET_DIR, src_file[:src_file.rindex('.')])
        cmd = ["g++", "-Wall", "-Os", "-o", exe_path, src_path]  # -Os compiles for minimal size
        print "Compiling file %s." % src_file
        sys.stdout.flush()
        p = subprocess.Popen(cmd, shell=False, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        t = threading.Timer(COMPILE_TIME_LIMIT, try_to_kill, 
                            [p, 
                             "Killed compilation due to timeout" + 
                             " (more than %s seconds)." % COMPILE_TIME_LIMIT])
        t.start()
        stdout, stderr = p.communicate()
        t.cancel()
        if p.returncode is not None and p.returncode != 0:
            print "Compilation error on %s" % src_file
            print stderr
            print
            shutil.rmtree(exe_path, True)
        elif p.returncode is not None:
            ## TODO: consider stripping (with strip or strip -s); however, looks like it only gains us a factor of 2 reduction in size.
            print "Compilation was successful!"
            print
        else:
            print "Unexpected error.  Please contact your instructor!"
            print
            print stderr
            print

    return not_src_files

if __name__ == "__main__":
    #test_LCU()

    src_files = os.listdir(SOURCE_DIR)

    # Drop non-alphanumeric/_/- files.
    bad_src_files = filter(lambda s: re.search(r'[^.a-zA-Z0-9_-]', s), src_files)
    src_files = filter(lambda s: not re.search(r'[^.a-zA-Z0-9_-]', s), src_files)
    for src_file in bad_src_files:
        print "Filename", src_file,"contains at least one character that is not: a letter, number, underscore, dash, or period.  Skipping it."
        
    for language in LANGUAGES:
        src_files = process_files_for_language(src_files, language)

    for src_file in src_files:
        print "Filename", src_file, "does not match the recognized languages:", ", ".join(map(lambda l: l.name, LANGUAGES)) + ".","Skipping it."
