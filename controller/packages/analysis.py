#!/usr/bin/env python

from __future__ import with_statement
from constants import *

#import portalocker # Maybe later?
import sys
import re
import os
import os.path
import signal
import subprocess
import threading
import shutil
import numpy
from scipy.stats import f as scipy_stats_f
from scipy.optimize import leastsq as scipy_optimize_leastsq

def neval(x, p):
    return p[0]*x + p[1]

def nlgneval(x, p):
    return p[0]*x*numpy.log2(x+1) + p[1]

def n2eval(x, p):
    return p[0]*x*x + p[1]

# def neval(x, p):
#     return p[0]*x + p[1]

# def nlgneval(x, p):
#     return p[0]*x*numpy.log2(x) + p[1]*x + p[2]

# def n2eval(x, p):
#     return p[0]*x*x + p[1]*x*numpy.log2(x) + p[2]*x + p[3]

def residuals_fn(f, x, y):
    return lambda p: y - f(x, p)

CURVE_FUNS = {"n" : neval,
              "nlgn" : nlgneval,
              "n2" : n2eval}

CURVE_P0S = {"n" : [0.001, 0.001],
             "nlgn" : [0.001, 0.001],
             "n2" : [0.001, 0.001]}

# TODO!!! switch to *10^ notation.  Not sure if e notation will work.
# CURVE_GLOSSES = {"n" : "%.14f*x + %.14f",
#                  "nlgn" : "%.14f*x*log2(x) + %.14f",
#                  "n2" : "%.14f*x^2 + %.14f"}

CURVE_GLOSSES = {"n" : "%e*x + %e",
                 "nlgn" : "%e*x*log2(x) + %e",
                 "n2" : "%e*x^2 + %e"}

# CURVE_P0S = {"n" : [0.001, 0.001],
#              "nlgn" : [0.001, 0.001, 0.001],
#              "n2" : [0.001, 0.001, 0.001, 0.001]}

# CURVE_GLOSSES = {"n" : "%f*x + %f",
#                  "nlgn" : "%f*x*log2(x) + %f*x + %f",
#                  "n2" : "%f*x^2 + %f*x*log2(x) + %f*x + %f"}

# Initiate from the p0 lists.
CURVES = CURVE_P0S.keys()

# Initiate from the p0 lists.
CURVE_PARAM_COUNTS = dict(map(lambda (curve, p0): (curve, len(p0)), CURVE_P0S.items()))

def string_is_float(s):
    try:
        float(s)
        return True
    except ValueError:
        return False

def string_is_int(s):
    try:
        int(s)
        return True
    except ValueError:
        return False

def load_data(filename):
    # Open the file
    values = []
    try:
        with open(filename, "r") as input:
            values = input.readlines()
    except IOError:
        print "Problem reading input from %s.  Error was: %s" % \
            (os.path.basename(filename), sys.exc_info())
        return None
    
    if len(values) == 0:
        print "Empty file while loading %s!" % os.path.basename(filename)
        return None

    # Remove an empty final line, if it exists.
    if values[-1].strip() == '':
        values = values[:-1]

    if len(values) == 0:
        print "Empty file while loading %s!" % os.path.basename(filename)
        return None

    array = map(lambda s: s.split(), values)

    if max(map(len, array)) > 2:
        print "Too many items in some row while loading %s." % os.path.basename(filename)
        return None

    if min(map(len, array)) < 2:
        print "Too few items in some row while loading %s." % os.path.basename(filename)
        return None

    if not reduce(lambda a,b: a and b, map(lambda a: string_is_int(a[0]) and string_is_float(a[1]), array)):
        print "Not all items in the array are appropriate numbers while loading %s." % os.path.basename(filename)
        return None

    # TODO: consider, now that we're using a better clock, ignoring
    # the first few entries to "reach" asymptotic behaviour?

    ## PREVIOUSLY: Stripped off the first quarter of entries to ignore
    ## early behaviour.  NOW: only reporting when the time increases
    ## by a tick, anyway.  Should quickly bypass early behaviour.
    #
    # array = array[(len(array) / 4) : ]

    # Fudge down by estimated clock tick rate.
    array = map(lambda a: [int(a[0]), float(a[1])/ESTIMATED_CLK_TICKS_PER_SEC], array)
    return array

def dump_data(array, filename):
    try:
        with open(filename, "w") as out:
            out.write('n,seconds\n')
            out.writelines(map(lambda a: '%d,%f\n' % (a[0], a[1]), array))
        return True
    except IOError:
        print "Problem writing output to %s.  Error was: %s" % \
            (os.path.basename(filename), sys.exc_info())
        return False

def nestedf(n, mser, msec, parmsr, parmsc):
    v1 = (parmsc - parmsr)
    v2 = (n - parmsc - 1)
    fval = (((mser * n) - (msec * n)) / v1) / ((msec * n) / v2)
    return (fval, v1, v2)

def fcdf(fval, v1, v2):
    #print "fval,v1,v2:", fval, v1, v2
    #print "fcdf of these:",scipy_stats_f.cdf(fval, v1, v2)
    return scipy_stats_f.cdf(fval, v1, v2)

# Returns: [(curve,params,confidence),...]
# Order should be the "correct" order.
def fit_all(array):
    nparray = numpy.array(array)

    x = nparray[:,0]
    y = nparray[:,1]

    results = {}
    for curve in CURVES:
        lsq_fit = scipy_optimize_leastsq(residuals_fn(CURVE_FUNS[curve], x, y),
                                         CURVE_P0S[curve], full_output = True)
        (params, cov_x, infodict, mesg, ier) = lsq_fit
        if ier < 1 or ier > 4:
            # No solution was found.
            print "No solution found for curve %s.  Reason was %s.  Failing all fits." % (curve, mesg)
            return None
        
        mse = numpy.mean(residuals_fn(CURVE_FUNS[curve], x, y)(params)**2)
        def basecalc(mse):
            return max(0, (2-mse)/2)

        baseconf = basecalc(mse)
        #print "curve, baseconf:", curve, baseconf
        if params[0] <= 0:
            baseconf = 0
        results[curve] = (params, mse, baseconf)

    msen=results['n'][1]
    msenlgn=results['nlgn'][1]
    msen2=results['n2'][1]

    basen=results['n'][2]
    basenlgn=results['nlgn'][2]
    basen2=results['n2'][2]

#     adjn = numpy.ceil(min(numpy.log2(len(x)) + 5, len(x)))
#     #adjn = len(x)

#     nvnlgn, v1nvlgn, v2nvlgn = nestedf(adjn, msen, msenlgn, CURVE_PARAM_COUNTS['n'], CURVE_PARAM_COUNTS['nlgn'])
#     nlgnvn2, v1nlgnvn2, v2nlgnvn2 = nestedf(adjn, msenlgn, msen2, CURVE_PARAM_COUNTS['nlgn'], 
#                                             CURVE_PARAM_COUNTS['n2'])
#     nvn2, v1nvn2, v2nvn2 = nestedf(adjn, msen, msen2, CURVE_PARAM_COUNTS['n'], CURVE_PARAM_COUNTS['nlgn'])

#     pnlgn = fcdf(nvnlgn, v1nvlgn, v2nvlgn)
#     pn2a = fcdf(nlgnvn2, v1nlgnvn2, v2nlgnvn2)
#     pn2b = fcdf(nvn2, v1nvn2, v2nvn2)
#     pn2 = min(pn2a, pn2b)
    
#     print "prels:", pnlgn, pn2a, pn2b

    #print "mses:", msen, msenlgn, msen2
    nconf = basen * min(1, msen2 / msen, msenlgn / msen)
    nlgnconf = basenlgn * min(1, msen / msenlgn, msen2 / msenlgn)
    n2conf = basen2 * min(1, msen / msen2, msenlgn / msen2)

    # Just for fun, normalize the confidences.
    denom = max(1, nconf + nlgnconf + n2conf)
    nconf = nconf / denom
    nlgnconf = nlgnconf / denom
    n2conf = n2conf / denom

    # Attempt at using nestedf results.
#     nconf = basen * msen2 / msen
#     nlgnconf = basenlgn * msen2 / msenlgn
#     n2conf = basen2 * pn2

    #print "confs:", nconf, nlgnconf, n2conf

    return [('n',results['n'][0], nconf),
            ('nlgn',results['nlgn'][0], nlgnconf),
            ('n2',results['n2'][0], n2conf)]

def comparison_confidence(fitnesses1, fitnesses2):
    confidence = 0
    for (curve1, params1, conf1) in fitnesses1:
        other_prob = 0
        for (curve2, params2, conf2) in fitnesses2:
            if curve1 == curve2:
                continue
            other_prob = other_prob + conf2
        conf = conf1 * other_prob
        confidence = confidence + conf

    ## PREVIOUSLY: Comparing two 0.33 0.33 0.33 results gives, 0.67,
    ## which should be pretty bad.  So, adjusting this downward by
    ## squaring it.
    #
    # I still agree that this is a high number, but any means besides
    # returning the plain value is too confusing for users!
    return confidence

def as_muParser(curve, params):
    params = tuple(params)
    if curve in CURVE_GLOSSES:
        if len(params) == CURVE_PARAM_COUNTS[curve]:
            return CURVE_GLOSSES[curve] % params
        else:
            raise "Error: curve %s requires %d parameters; parameters given were %s" % \
                (`curve`, CURVE_PARAM_COUNTS[curve], `params`)
    else:
        raise "Error: unknown curve %s" % `curve`

# Dumps out all needed files into the ig directory.
# Returns the matrix (dictionary) of comparisons on success and None on failure.
def process_user_ig(input_base, output_base, user, ig):
    # Find the files.
    input_dir = os.path.normpath(os.path.join(input_base, user, ig))
    try:
        files = os.listdir(input_dir)
    except os.error:
        print "Failed getting raw data for %s; error was %s" % (user, sys.exc_info())
        return None

    # Prepare output directories.
    output_dir = os.path.normpath(os.path.join(output_base, user, ig))
    try:
        shutil.rmtree(output_dir, True)  # Remove the directory, ignoring errors.
        os.makedirs(output_dir)
    except os.error:
        print "Failed to construct results directory for %s; error was %s" % (user, sys.exc_info())
        return None

    # Cross-dictionary data.
    fitnesses = {}

    # For each file in the user's IG directory:
    for file in files:
        # Load it as an array.
        array = load_data(os.path.normpath(os.path.join(input_dir, file)))
        if array is None:
            print "Failed to parse file %s; continuing to others." % file
            continue

        # Dump it out.
        if not dump_data(array, os.path.normpath(os.path.join(output_dir, file))):
            print "Failed to dump data from file %s; continuing to others." % file
        
        # Calculate its fitness for all curves.
        #print "Dict:", file
        fitness_info = fit_all(array)
        if fitness_info is None:
            print "Failed to compute fitness for %s; continuing." % file
            continue
        
        fitnesses[file] = fitness_info

    if len(fitnesses) == 0:
        print "No results found for %s; generating no output." % user
        return None

    results = fitnesses.items()
    results.sort()

    # Dump fitness file: dictionary,curve,function,confidence
    try:
        with open(os.path.normpath(os.path.join(output_dir, CONFIDENCES_FILE)), "w") as out:
            out.write('dictionary,curve,function,confidence\n')
            out.writelines(["%s,%s,%s,%s\n" % (dict, curve, as_muParser(curve, params), fitness)
                            for (dict, curve_fitnesses) in results
                            for (curve, params, fitness) in curve_fitnesses])
    except IOError:
        print "Problem writing confidences for %s.  Error was: %s" % \
            (user, sys.exc_info())
        return None

    # Calculate the probability for each pair of dictionaries that
    # they are asymptotically distinct, assuming that the fitnesses
    # represent probabilities (and various independence assumptions,
    # etc.).
    comparisons = {}
    for dict1 in fitnesses.keys():
        comparisons[dict1] = {}
        for dict2 in fitnesses.keys():
            if dict1 == dict2:
                comparisons[dict1][dict2] = 0
            else:
                comparisons[dict1][dict2] = comparison_confidence(fitnesses[dict1], fitnesses[dict2])
    
    # Dump comparison file: dictionary1,dictionary2,confidence
    try:
        with open(os.path.normpath(os.path.join(output_dir, COMPARISONS_FILE)), "w") as out:
            out.write('dictionary1,dictionary2,confidence\n')
            for i in range(len(results)):
                for j in range(i+1, len(results)):
                    dict1 = results[i][0]
                    dict2 = results[j][0]
                    out.write('%s,%s,%f\n' % (dict1, dict2, comparisons[dict1][dict2]))
    except IOError:
        print "Problem writing comparisons for %s.  Error was: %s" % \
            (user, sys.exc_info())
        return None


    # Return comparisons if successful. 
    return comparisons

# Dumps out all needed files for this user, including the "comparisons" and "generators" files.
def process_user(input_base, output_base, user):
    # Find all the input generators available.
    input_dir = os.path.normpath(os.path.join(input_base, user))
    try:
        # Get only the directories (in case there's anything else there!).
        dirs = [d for d in os.listdir(input_dir) if os.path.isdir(os.path.join(input_dir, d))]
    except os.error:
        print "Failed to find input generators for %s; error was %s" % (user, sys.exc_info())
        return False

    

    # Prep the output directory.
    output_dir = os.path.normpath(os.path.join(output_base, user))
    try:
        shutil.rmtree(output_dir, True)  # Remove the directory, ignoring errors.
        os.makedirs(output_dir)
    except os.error:
        print "Failed to construct output data directory for %s; error was %s" % (user, sys.exc_info())
        return False


    comparisons = {}

    # Run each one, keeping track of top confidence for each comparison.
    for dir in dirs:
        next_comparisons = process_user_ig(input_base, output_base, user, dir)
        if next_comparisons is None:
            print "Failed processing input generator %s; continuing" % dir
            continue

        for k in next_comparisons.keys():
            if k not in comparisons:
                comparisons[k] = next_comparisons[k]
            for k2 in next_comparisons[k].keys():
                if k2 not in comparisons[k]:
                    comparisons[k][k2] = next_comparisons[k][k2]
                comparisons[k][k2] = max(comparisons[k][k2], next_comparisons[k][k2])

    # Dump the base comparisons file (dictionary1,dictionary2,confidence).
    dicts = comparisons.keys()
    dicts.sort()
    try:
        with open(os.path.normpath(os.path.join(output_dir, COMPARISONS_FILE)), "w") as out:
            out.write('dictionary1,dictionary2,confidence\n')
            for i in range(len(dicts)):
                for j in range(i+1, len(dicts)):
                    out.write('%s,%s,%f\n' % (dicts[i], dicts[j], comparisons[dicts[i]][dicts[j]]))
    except IOError:
        print "Problem writing comparisons for %s.  Error was: %s" % \
            (dir, sys.exc_info())
        return False

    # Dump the base generators file (name).
    try:
        with open(os.path.normpath(os.path.join(output_dir, GENERATORS_FILE)), "w") as out:
            out.write('name\n')
            out.writelines(map(lambda dir: "%s\n" % dir, dirs))
    except IOError:
        print "Problem writing generators file for %s.  Error was: %s" % \
            (dir, sys.exc_info())
        return False
	try:
			shutil.rmtree(SITE_DIR, True)  # Remove the directory, ignoring errors.
			shutil.copytree(DATA_BASE_DIR, SITE_DIR, symlinks=False,ignore=abc)  # Remove the directory, ignoring errors.
	except os.error:
			print "Failed to construct output data directory for %s; error was %s" % (user, sys.exc_info())
			return False
    return True

def usage():
    print "Usage: %s <user_id>" % sys.argv[0]


if __name__ == "__main__":
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

    if not process_user(TIMINGS_BASE_DIR, DATA_BASE_DIR, user_id):
        print "Error processing user %s." % user_id

