#!/usr/bin/env python

from __future__ import with_statement
from constants import *

#import portalocker
import sys
import re
import os
import os.path
import signal
import subprocess
import threading
import shutil
import analysis

# Needs to write an overall comparisons file and a users file.
# Comparisons: dictionary1,dictionary2,confidence
# Users: name,generators,total
# (Total is the sum of the (distinct-)pairwise confidence of dictionaries for that user.


def string_is_float(s):
    try:
        float(s)
        return True
    except ValueError:
        return False



# Transfers user files to the web directory.
# Also, returns the key summary info for the user.
def transfer_user(input_base, output_base, user):
    input_dir = os.path.normpath(os.path.join(input_base, user))
    output_dir = os.path.normpath(os.path.join(output_base, user))

    # Find and parse the comparison file.
    values = []
    filename = os.path.join(input_dir, COMPARISONS_FILE)
    #print "Processing comparison file: " + filename
    try:
        with open(filename, "r") as input:
            values = input.readlines()
    except IOError:
        print "Problem reading input from %s.  Error was: %s" % \
            (filename, sys.exc_info())
        return None

    if len(values) == 0:
        print "Empty comparisons file for %s!" % filename
    else:
        # Strip the header line.
        values = values[1:]

    values = map(lambda s: s.split(','), values)
    if len(values) == 0:
        print "No rows to look at from %s." % filename
        return None
    if max(map(len, values)) > 3:
        print "Too many items in some row while loading comparisons from %s." % filename
        return None

    if min(map(len, values)) < 3:
        print "Too few items in some row while loading comparisons from %s." % filename
        return None

    if not reduce(lambda a,b: a and b, map(lambda a: string_is_float(a[2]), values)):
        print "Not all confidences are numbers while loading %s." % filename
        return None

    confidences = [(dict1, dict2, float(conf))
                   for (dict1, dict2, conf) in values]

    if not reduce(lambda a,b: a and b, map(lambda a: a[2] >= 0 and a[2] <= 1, confidences)):
        print "Not all confidences are between 0 and 1 while loading %s." % filename
        return None

    # Find and parse the generator file.
    values = []
    filename = os.path.join(input_dir, GENERATORS_FILE)
    try:
        with open(filename, "r") as input:
            values = input.readlines()
    except IOError:
        print "Problem reading input from %s.  Error was: %s" % \
            (filename, sys.exc_info())
        return None
    if len(values) == 0:
        print "Empty generators file for %s!" % filename
    else:
        # Strip the header line.
        values = values[1:]
    
    generators = values


    # Calculate the comparisons matrix.
    comparisons = {}
    for (dict1, dict2, conf) in confidences:
        if dict1 not in comparisons:
            comparisons[dict1] = {}
        if dict2 not in comparisons:
            comparisons[dict2] = {}
        if dict2 in comparisons[dict1]:
            print "Repeat comparison for %s vs %s." % (dict1, dict2)
        if dict1 in comparisons[dict2]:
            print "Repeat comparison for %s vs %s." % (dict2, dict1)
        comparisons[dict1][dict2] = conf
        comparisons[dict2][dict1] = conf

    # Calculate the total of all pairwise confidences.
    total = reduce(lambda a,b: a + b, map(lambda (dict1, dict2, conf): conf, confidences))

    # Return (gen_count,total,comparisons) (where the latter is a dict/dict matrix).
    return (len(generators), total, comparisons)


def process_all(input_base, output_base):
    # Find all the users avaliable.
    input_dir = os.path.normpath(input_base)
    try:
        # Get only the directories (in case there's anything else there!).
        dirs = [d for d in os.listdir(input_dir) if os.path.isdir(os.path.join(input_dir, d))]
    except os.error:
        print "Failed to read directory %s; error was %s" % (input_dir, sys.exc_info())
        return False

    # Prep the output directory.
    output_dir = os.path.normpath(output_base)
    try:
        #shutil.rmtree(output_dir, True)  # Remove the directory, ignoring errors.
        # Cannot just remove the directory; need the static files!
        os.makedirs(output_dir)
    except os.error:
        print "Failed to construct output directory on web server."
        # Old error data: %s; error was %s" % (output_dir, sys.exc_info())
        print "Assuming the directory already exists and continuing."

    comparisons = {}
    comparisonsusers = {}
    users = []

    # Process each user, keeping track of the best comparisons and users.
    for dir in dirs:
		# SILENTLY PROCEED IF NO ERROR
        #print "Processing user:", dir

        out = transfer_user(input_base, output_base, dir)
        if out is None:
            print "Failed processing user %s; continuing" % dir
            continue

        (next_gen_count, next_total, next_comparisons) = out
        if next_comparisons is None:
            print "Failed processing user %s; continuing" % dir
            continue

        users.append((dir, next_gen_count, next_total))

        for k in next_comparisons.keys():
            if k not in comparisons:
                comparisons[k] = next_comparisons[k]
                comparisonsusers[k] = dict(zip(next_comparisons.keys(), [dir]*len(next_comparisons)))
            for k2 in next_comparisons[k].keys():
                if k2 not in comparisons[k]:
                    comparisons[k][k2] = next_comparisons[k][k2]
                current, test = comparisons[k][k2], next_comparisons[k][k2] # current and test (this user's) scores
                if test > current:
                    comparisons[k][k2] = test
                    comparisonsusers[k][k2] = dir

    # Dump the base comparisons file (dictionary1,dictionary2,confidence).
    dicts = comparisons.keys()
    dicts.sort()
    try:
        with open(os.path.normpath(os.path.join(output_dir, COMPARISONS_FILE)), "w") as out:
            out.write('dictionary1,dictionary2,confidence,name\n')
            for i in range(len(dicts)):
                for j in range(i+1, len(dicts)):
                    # In case of entirely incomparable dictionaries.
                    if dicts[j] in comparisons[dicts[i]]:
                        out.write('%s,%s,%f,%s\n' % (dicts[i], dicts[j], comparisons[dicts[i]][dicts[j]], comparisonsusers[dicts[i]][dicts[j]]))
    except IOError:
        print "Problem writing comparisons.  Error was: %s" % `sys.exc_info()`
        return False

    # Dump the base curves file (static).
    try:
        with open(os.path.normpath(os.path.join(output_dir, CURVES_FILE)), "w") as out:
            out.write(CURVES_CONTENTS)
    except IOError:
        print "Problem writing curves file.  Error was: %s" % `sys.exc_info()`
        return False

    # Dump the base users file (name,generators,total).
    try:
        with open(os.path.normpath(os.path.join(output_dir, USERS_FILE)), "w") as out:
            out.write('name,generators,total\n')
            out.writelines(map(lambda row: "%s,%d,%f\n" % row, users))
    except IOError:
        print "Problem writing users file.  Error was: %s" % `sys.exc_info()`
        return False

    # Dump the base dictionaries file (name).
    try:
        with open(os.path.normpath(os.path.join(output_dir, DICTIONARIES_FILE)), "w") as out:
            out.write('name\n')
            out.writelines(map(lambda row: "%s\n" % row, dicts))
    except IOError:
        print "Problem writing dictionaries file.  Error was: %s" % `sys.exc_info()`
        return False

    analysis.move_file('comparisons')
    analysis.move_file('curves')
    analysis.move_file('dictionaries')
    analysis.move_file('users')
    return True

def usage():
    print "Usage: %s"
    print "(No argument.)"


if __name__ == "__main__":
    if len(sys.argv) != 1:
        print "Incorrect number of arguments given in command: %s" % " ".join(sys.argv)
        usage()
        exit(1)

    if not process_all(DATA_BASE_DIR, FINAL_OUTPUT_BASE_DIR):
        print "Error processing!"

