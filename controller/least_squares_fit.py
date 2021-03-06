#!/usr/bin/env python

from __future__ import with_statement
from constants import *

#import portalocker # Maybe later?
import sys
import re
import os
import os.path
from array import *
import signal
import subprocess
import threading
import shutil
import math

def func_lgn(value):
	return math.log(value, 2) 


def multiply_vectors(x,y):
	answer = 0
	length = len(x)
	for index in xrange(length):
		answer = answer + x[index]*y[index]
	return answer

assert multiply_vectors([1,2,3],[5,0,-3]) == -4, "multiply_vectors test failed"
	
def low_error_fit(x,y):
	length = len(y) 
	y_prime = []
	const_array = []
	lgx_array = []
	x_array = []
	extra_x_array = []
	extra_xlgx_array = []
	extra_x2_array = []
	
	fits = []
	
	for index in xrange(1,length-1):
		y_prime.append((y[index+1]-y[index])/500)
		
	for index in xrange(1,length-1):
		const_array.append(1)
		lgx_array.append(math.log(x[index]+1)+1)
		x_array.append(x[index])
		
	fits.append(multiply_vectors(y_prime, const_array)/multiply_vectors(const_array, const_array))
	fits.append(multiply_vectors(y_prime, lgx_array)/multiply_vectors(lgx_array, lgx_array))
	fits.append(multiply_vectors(y_prime, x_array)/multiply_vectors(x_array, x_array))
	
	least_squares_sum = 0
	
	for index in xrange(0,length-2):
		least_squares_sum = least_squares_sum + math.fabs(fits[0]*const_array[index] - y_prime[index])
	
	fits[0] = least_squares_sum
	print "x fit: " + str(least_squares_sum)
	least_squares_sum = 0
	
	for index in xrange(0,length-2):
		least_squares_sum = least_squares_sum + math.fabs(fits[1]*lgx_array[index] - y_prime[index])
	
	fits[1] = least_squares_sum
	print "xlgx fit: " + str(least_squares_sum)
	least_squares_sum = 0
	
	for index in xrange(0,length-2):
		least_squares_sum = least_squares_sum + math.fabs(fits[2]*x_array[index] - y_prime[index])
	
	fits[2] = least_squares_sum
	print "x2 fit: " + str(least_squares_sum)
	sum = fits[0] + fits[1] + fits[2]
	
	for index in xrange(0,3):
		fits[index] = 1 - fits[index]/sum
		
	sum = fits[0] + fits[1] + fits[2]
	
	for index in xrange(0,3):
		fits[index] = fits[index]/sum
		
	for index in xrange(length):	
		extra_x_array.append(x[index])
		extra_xlgx_array.append(x[index]*func_lgn(x[index]+1))
		extra_x2_array.append(x[index]*x[index])
		
	fits.append(multiply_vectors(y, extra_x_array)/multiply_vectors(extra_x_array, extra_x_array))
	fits.append(multiply_vectors(y, extra_xlgx_array)/multiply_vectors(extra_xlgx_array, extra_xlgx_array))
	fits.append(multiply_vectors(y, extra_x2_array)/multiply_vectors(extra_x2_array, extra_x2_array))
	
	return fits
	
# Our primary function. It takes in two lists of equal size, list 'x' corrosponding to the times and list 'y'
# corrosponding to the timing values. To do a linear least squares fit, we minimize the following equation: 
# A*x = y, where the A[x][y] = f_y(t_x), ie the yth function (either f(x) = x, xlgx or x^2), applied to the x_th time

# We will do two fits, one with ax+blgx+c to the derivative to migate the error of discontinuities (hash tables) and  
# another with each of the functions (either f(x) = x, xlgx or x^2) individually to print a pretty picture.

# z is unimplemented.  Use the default value for now.
def ls_fit(x, y, z = 0):
	length = len(y) 
	
	y_prime = []
	const_array = []
	lgx_array = []
	x_array = []
	xlgx_array = []
	x2_array = []
	
	# Computing b
	
	if (z == 0):
		for index in xrange(1,length-1):
			y_prime.append((y[index+1]-y[index])/(x[index+1]-x[index]))
	else:
		for index in xrange(1,length-1):
			y_prime.append(y[index])
	
	
	# Computing A
        # A is [ const_array lgx_array x_array ]
	if (z == 0):
		for index in xrange(1,length-1):
			const_array.append(1)
			lgx_array.append(func_lgn(x[index]+1))
			x_array.append(x[index])
	else:
		for index in xrange(1,length-1):
			x_array.append(x[index])
			xlgx_array.append(x[index]*func_lgn(x[index]+1))
			x2_array.append(x[index]*x[index])
		
	
	# Computing A^t*A
	
	column_1 = [multiply_vectors(const_array,const_array), 
		    multiply_vectors(const_array,lgx_array),
		    multiply_vectors(const_array,x_array)]
	column_2 = [multiply_vectors(lgx_array,const_array),
		    multiply_vectors(lgx_array,lgx_array),
		    multiply_vectors(lgx_array,x_array)]
	column_3 = [multiply_vectors(x_array,const_array),
		    multiply_vectors(x_array,lgx_array),
		    multiply_vectors(x_array,x_array)]
	
	# Inverting A^t*A (the annoying part [THIS WILL BE UGLY (That's what inverses are btw [UGLY])])
	# Doing this via http://en.wikipedia.org/wiki/Invertible_matrix#Inversion_of_3.C3.973_matrices
	
	a = column_1[0]
	b = column_2[0]
	c = column_3[0]
	
	d = column_1[1]
	e = column_2[1]
	f = column_3[1]
	
	g = column_1[2]
	h = column_2[2]
	k = column_3[2]
	
	# [ a b c ]
	# [ d e f ]
	# [ g h k ]

	det = a*(e*k - f*h) - b*(k*d - f*g) + c*(d*h - e*g)
	
	rec_det = 1/det

	# A D G
	inv_row_1 = [rec_det*(e*k - f*h),
		     rec_det*(c*h - b*k),
		     rec_det*(b*f - c*e)]
	
	# B E H
	inv_row_2 = [rec_det*(f*g - d*k),
		     rec_det*(a*k - c*g),
		     rec_det*(c*d - a*f)]
	
	# C F K
	inv_row_3 = [rec_det*(d*h - e*g),
		     rec_det*(g*b - a*h),
		     rec_det*(a*e - b*d)]
	
	# Computing A^t*b
	
	At_b = [multiply_vectors(const_array,y_prime),
		multiply_vectors(lgx_array,y_prime),
		multiply_vectors(x_array,y_prime)]
	
	# And now we can get our fit! (our answer, pt.1)
	# fits = (A^t*A)^-1 * A^t*b  
	
	fits = [multiply_vectors(inv_row_1,At_b),
		multiply_vectors(inv_row_2,At_b),
		multiply_vectors(inv_row_3,At_b)]
	
	
	# Part 2 (Just-for-looks fits [This is waaaaay easier])
	
	extra_x_array = []
	extra_xlgx_array = []
	extra_x2_array = []
	
	for index in xrange(length):	
		extra_x_array.append(x[index])
		extra_xlgx_array.append(x[index]*func_lgn(x[index]+1))
		extra_x2_array.append(x[index]*x[index])
		
	
	x_A = multiply_vectors(extra_x_array,extra_x_array)
	x_b = multiply_vectors(extra_x_array, y)
	fits.append(x_b/x_A)
	
	xlgx_A = multiply_vectors(extra_xlgx_array,extra_xlgx_array)
	xlgx_b = multiply_vectors(extra_xlgx_array, y)
	fits.append(xlgx_b/xlgx_A)
	
	x2_A = multiply_vectors(extra_x2_array,extra_x2_array)
	x2_b = multiply_vectors(extra_x2_array, y)
	fits.append(x2_b/x2_A)
	
	# Now the first three elements are the derivative fits of (x, xlgx, x^2) and the next 3 are the "solo" fits of the three functions in the same order. Now to analyze.
	
	greatest_n = x[-1]
	#print "greatest_n is",greatest_n
	
	# We need to "undifferentiate" the fitting constants.
	fit_x    = (fits[0]-fits[1]*func_lgn(math.e))
	fit_xlgx = fits[1]
	fit_x2   = fits[2]/2

	#print fit_x
	#print fit_xlgx
	#print fit_x2
	
	max_x = greatest_n*fit_x
	max_xlgx = greatest_n*func_lgn(greatest_n+1)*fit_xlgx
	max_x2 = greatest_n*greatest_n*fit_x2

	# THIS MUST BE >= 1. If a negative appears, this is the scaling factor applied to the added value. 
	fit_dilute = 1
	if (max_x < 0 or max_xlgx < 0 or max_x2 < 0):
		if (max_x < max_xlgx and max_x < max_x2):
			max_xlgx = max_xlgx - fit_dilute*max_x
			max_x2 = max_x2 - fit_dilute*max_x
			max_x = (1 - fit_dilute)*max_x
		if (max_xlgx < max_x and max_xlgx < max_x2):
			max_x = max_x - fit_dilute*max_xlgx 
			max_x2 = max_x2 - fit_dilute*max_xlgx 
			max_xlgx = (1 - fit_dilute)*max_xlgx 
		if (max_x2 < max_xlgx and max_x2 < max_x):
			max_xlgx = max_xlgx - fit_dilute*max_x2
			max_x = max_x - fit_dilute*max_x2
			max_x2 = (1 - fit_dilute)*max_x2
	
	#print "Max values:"
	#print max_x
	#print max_xlgx
	#print max_x2
	#print

	sum = max_x + max_xlgx + max_x2
	
	if sum == 0:
		fits = [1.0/3.0]*3
        else:
		fits[0] = max_x / sum
		fits[1] = max_xlgx / sum
		fits[2] = max_x2 / sum
	
	#print "FITS:"
	#print fits
	#print
	
	return fits

assert reduce(lambda x,(y,z): x+math.fabs(y-z),zip(ls_fit(range(10000), [x for x in range(10000)])[:3], [1, 0, 0]),0) < 0.01, "%s[:3] does not match %s" % (ls_fit(range(10000), [x for x in range(10000)]), [1, 0, 0])
assert reduce(lambda x,(y,z): x+math.fabs(y-z),zip(ls_fit(range(10000), [(x+1)*func_lgn(x+1) for x in range(10000)])[:3], [0, 1, 0]),0) < 0.01, "%s[:3] does not match %s" % (ls_fit(range(10000), [(x+1)*func_lgn(x+1) for x in range(10000)]), [0, 1, 0])
assert reduce(lambda x,(y,z): x+math.fabs(y-z),zip(ls_fit(range(10000), [x**2 for x in range(10000)])[:3], [0, 0, 1]),0) < 0.01, "%s[:3] does not match %s" % (ls_fit(range(10000), [x**2 for x in range(10000)]), [0, 0, 1])
