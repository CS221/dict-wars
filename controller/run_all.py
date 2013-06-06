#!/usr/bin/env python

# Run the input generator and send its output straight to the
# dictionary runner (no intermediate file created).  This DOES limit
# the number of lines to the specified n.

from __future__ import with_statement
from constants import *

# TODO: set up to handle multiple simultaneous runs in a straightforward fashion.
# TODO: MOST LIKELY implementation: create temporary directory for output
# TODO: upon completion, move temporary directory to overwrite output directory?
# TODO: change cwd to some temp location in both scripts??
# TODO: wipe out env for the subprocess??
# TODO: don't dump all of stderr if too long??

import sys
import re
import os
import os.path
import signal
import subprocess
import threading
import shutil


def usage():
    print "Usage: %s <user_id>" % sys.argv[0]

def parse_dump(data, file):
    informed_only_one_problem = False
    problem = False
    for line in data.splitlines():
        fields = line.split(" ")
        if len(fields) < 2:
            if not problem:
                print "Failed to parse a line of input; no spaces."
            problem = True
            continue
        if len(fields) > 2:
            if not problem:
                print "Failed to parse a line of input; more than one space."
            problem = True
            continue
        command, key = fields
        if command != "I" and command != "F" and command != "R":
            if not problem:
                print "Failed to parse a line of input; command was not I, R, or F."
            problem = True
            continue
        try:
            key_n = long(key)
        except ValueError:
            if not problem:
                print "Failed to parse a line of input; non-numeric key."
            problem = True
            continue
        if key_n <= 0:
            if not problem:
                print "Failed to parse a line of input; key was not positive."
            problem = True
            continue

        if problem and not informed_only_one_problem:
            print "There was a problem parsing the input generated by your input generator."
            print "Only one problem is reported per input generator run to avoid excessive output."
            print "Further problem printing will be suppressed."
            informed_only_one_problem = True

        file.write("%s %d\n" % (command, key_n))
    return not problem

if len(sys.argv) != 2:
    print "Incorrect number of arguments given in command: %s" % " ".join(sys.argv)
    usage()
    exit(1)

user_id = sys.argv[1]
if re.match(r"^[a-zA-Z0-9._-]+$", user_id) is None:
    print "User ID must be at least one letter long and " + \
          "contain only alphanumerics, dash, dot, and underscore."
    print "Given user ID was: \"%s\"." % user_id
    usage()
    exit(1)

## PREVIOUSLY: we took n as a command-line argument.
# str_n = sys.argv[2]
# n = 1
# try:
#     n = long(str_n)
# except ValueError:
#     print "Input length must be a (long) integer; given \"%s\"" % str_n
#     usage()
#     exit(1)

# if n <= 0:
#     print "Input length must be positive; given \"%d\"" % n
#     usage()
#     exit(1)

## NOW: it's a constant in constants.py.
n = NUM_OPERATIONS

SOURCE_DIR = os.path.join(EXE_BASE_DIR, user_id)

files = []
try:
    files = os.listdir(SOURCE_DIR)
except os.error:
    # Note: cannot get "except os.error as e" to work for some reason.
    # Ah.. not present in Python 2.6 :(
    print "Unable to find executables for %s; error was: %s" % (user_id, sys.exc_info())
    pass

# Check that there aren't too many files to run.
if len(files) > MAX_RUNS:
    print "Found %d files, which is more than the maximum allowable of %d" % \
          (len(files), MAX_RUNS)
    print "Selecting only the first %d." % MAX_RUNS
    print "Full list of files was %s." % " ".join(files)

    files = files[:MAX_RUNS]

    print "Using only %s." % " ".join(files)


# Prep the output directory.
shutil.rmtree(os.path.join(TIMINGS_BASE_DIR, user_id), True)
try:
    os.makedirs(os.path.join(TIMINGS_BASE_DIR, user_id))
except os.error:
    print "Problem creating output directory for %s.  Error was: %s" % \
          (user_id, sys.exc_info())
    exit(1)
except error:
    # Leaf already exists.  Ignore.
    print "Ignoring warning that output directory for %s already exists." % \
          (user_id)
    pass

count = 1
for file in files:
    print "=============== PROCESSING FILE %d ===================" % count
    sys.stdout.flush()
    count = count + 1
    # Not an ironclad test, but useful for debugging.
    if not os.access(os.path.join(SOURCE_DIR, file), os.X_OK):
        print "Cannot run input generator \"%s\"; not an executable." % file
        print "Skipping to the next file."
        continue

    result_dir = os.path.normpath(os.path.join(TIMINGS_BASE_DIR, user_id, file))
    try:
        os.makedirs(result_dir)
    except os.error:
        print "Problem creating timing output file for %s.  Error was: %s" % \
            (user_id, sys.exc_info())
        print "Skipping to the next file."
        continue
    except error:
        # Leaf already exists.  Ignore.
        print "Ignoring warning that timing output for %s already exists." % user_id
        pass
    
    p = None
    try:
        cmd = ["./piped_run_ig_dict_limiting_line_count", 

               # The input generator:
               os.path.join(SOURCE_DIR, file), ("%u" % n),   

               # The dictionary runner:
               DICT_RUNNER, "-", `MAX_CPU_SECS_PER_ALG`, result_dir] + \
               MYSTERY_DICTS
               
        p = subprocess.Popen(cmd, stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE, env={})
    except:
        print "Unable to run input generator \"%s\" for undetermined reasons." % file
        print "Error was: %s" % (sys.exc_info(),)
        print "Skipping to the next file."
        continue

    if p is not None:
        def kill_if_too_long(p):
            # As always, not an ironclad test, just handy.
            if p.returncode is None:
                print "Input generator \"%s\" ran for more than the allowed time of %d secs.  Killing." % (file, MAX_EXECUTION_SECS)
                
                #p.kill()

                # Older version alternative to p.kill():
                os.kill(p.pid, signal.SIGKILL)
        
        t = threading.Timer(MAX_EXECUTION_SECS, kill_if_too_long, [p])

        t.start()
        stdout, stderr = p.communicate()

        # No negative impact if run even though timer has already fired.
        t.cancel() # May not catch the timer in time, but handy to try.
        
        if p.returncode is None or p.returncode != 0:
            print "Error running input generator \"%s\"." % file
            print "Standard output was:"
            print stdout
            print
            print
            print "Standard error was:"
            print stderr
            print 
            print
            print "Skipping to next file."
            continue

        print "Successfully ran input generator \"%s\"." % file
