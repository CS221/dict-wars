
#2010W2/2011W2 note: does not work on RedBlackTree as it does not compile: just remove it manually after

rm test_contains.sh
for i in *.h; do if grep Dictionary $i>/dev/null; then echo "#include \"$i\"" >> test_contains.h; fi; done

#grep -v RedBlack test_contains.h >> test_contains.h.temp
#mv test_contains.h.temp test_contains.h
