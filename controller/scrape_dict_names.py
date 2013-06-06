from __future__ import with_statement
from constants import *
import sys
import re
import os
import os.path
import signal
import subprocess
import threading
import shutil

def scrape_dict_names():
    p = None
    try:
        cmd = [DICT_RUNNER]
               
        p = subprocess.Popen(cmd, stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE, env={})
    except:
        print "Unable to run the dict_runner \"%s\" for undetermined reasons." % DICT_RUNNER
        print "Error was: %s" % (sys.exc_info(),)
        print "Therefore, unable to determine which dictionaries to test on!"
        raise "unable to determine active dictionaries"
    
    if p is None:
        print "Unable to run the dict_runner \"%s\" for undetermined reasons." % DICT_RUNNER
        print "subprocess.Popen returned None"
        print "Therefore, unable to determine which dictionaries to test on!"
        raise "unable to determine active dictionaries"

    stdout, stderr = p.communicate()
    result = re.search("""^%s [^[]*\[(?P<flags>[^]]*)\]""" % DICT_RUNNER, stdout, re.MULTILINE)
    if result is None:
        print "Unable to find the flags available to the dict_runner in its output"
        print "Therefore, unable to determine which dictionaries to test on!"
        raise "unable to determine active dictionaries"

    if 'flags' not in result.groupdict() or result.group('flags') is None:
        print "Unable to locate the flags available to the dict_runner in its output"
        print "Therefore, unable to determine which dictionaries to test on!"
        raise "unable to determine active dictionaries"

    return [flag[1:] for flag in result.group('flags').split("|")]
