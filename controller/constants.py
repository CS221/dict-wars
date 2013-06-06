## KEY constants contained herein: MYSTERY_DICT_COUNT (the number of
## dictionaries we're working with), NUM_OPERATIONS (the number of
## operations/input lines each generator should produce), BASE_DIR and
## FINAL_OUTPUT_BASE_DIR (where input comes from and goes to),
## MAX_RUNS (the number of input generators from a student that we'll
## use), MAX_CPU_SECS_PER_ALG and MAX_EXECUTION_SECS and
## COMPILE_TIME_LIMIT (limits on the length of various stages of the
## process).  Also, perhaps, ESTIMATED_CLK_TICKS_PER_SEC (just to make
## the output something like seconds rather than an arbitrary number).

import os
import os.path
import sys

# NEEDS TO BE CONFIGURED TO DIRECT DATA TO WEB SERVER

#HOST = "ec2-54-242-199-187.compute-1.amazonaws.com"
HOST = "remote.ugrad.cs.ubc.ca"
PORT_NUM = 22
#USERNAME = "ec2-user"
USERNAME = "CS221"
PASSWORD = "#M0nd@y#"
#SSH_KEY = os.path.normpath(os.path.join(BASE_DIR, r"ssh-keys", r"steve-kuba-pair.pem"))
#LOCAL_DIR = '/home/ec2-user/public_html/data'
LOCAL_DIR = '/home/c/cs221/public_html/2012W2/fun/data/'

# CAN BE CONFIGURED TO CHANGE NUMBER OF GENERATORS OR DICTIONARIES

### RUN_ALL ###
NUM_OPERATIONS = 100000
MYSTERY_DICT_COUNT = 8
MYSTERY_DICTS = ["mys%02u" % (i + 1) for i in range(MYSTERY_DICT_COUNT)]

MAX_RUNS=10
MAX_CPU_SECS_PER_ALG=0.5
MAX_EXECUTION_SECS=MAX_CPU_SECS_PER_ALG * MYSTERY_DICT_COUNT

# LIKELY SHOULD NOT BE CHANGED

### COMPILE ###
COMPILE_TIME_LIMIT = 10

### ANALYSIS ###
ESTIMATED_CLK_TICKS_PER_SEC = 1000000

# Allow locally isntalled Paramiko if needed.
sys.path.append('../lib/python2.7/site-packages/paramiko-1.7.7.1-py2.7.egg')

# BASE_DIR=os.path.expanduser(os.path.normpath(r"~cs221/proj3-2011S"))
# FINAL_OUTPUT_BASE_DIR=os.path.expanduser(r"~cs221/public_html/proj3-2011S/data")

# Use the current working directory to set these:
BASE_DIR=os.path.normpath(os.path.join(os.getcwd(), ".."))
FINAL_OUTPUT_BASE_DIR=os.path.normpath(os.path.join(BASE_DIR, "web-output"))
RUNS_DIR=os.path.normpath(os.path.join(BASE_DIR, "runs"))

EXE_BASE_DIR=os.path.normpath(os.path.join(RUNS_DIR, r"exes"))
TIMINGS_BASE_DIR=os.path.normpath(os.path.join(RUNS_DIR, r"raw_data"))
DATA_BASE_DIR=os.path.normpath(os.path.join(RUNS_DIR, r"data"))
SOURCE_BASE_DIR=os.path.normpath(os.path.join(RUNS_DIR, r"source"))

### SUMMARY (and analysis) ###
CURVES_CONTENTS = """name,human,latex
n,"n","n"
nlgn,"n lg n","n \lg n"
n2,"n-squared","n^2"
"""

CURVES_FILE = "curves"
USERS_FILE = "users"
CONFIDENCES_FILE = "confidences"
COMPARISONS_FILE = "comparisons"
GENERATORS_FILE = "generators"
DICTIONARIES_FILE = "dictionaries"

REMOTE_DIR = FINAL_OUTPUT_BASE_DIR

DICT_RUNNER = os.path.normpath(os.path.join(BASE_DIR, r"dict_runner/dict_runner"))

