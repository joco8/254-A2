# Trivial makefile for the calculator scanner/parser.
# Depends on default (built-in) rules for C compilation.

# Note that rule for goal (parse) must be the first one in this file.

parse: parse.cpp
	g++ -std=c++11 -o parse parse.cpp scan.cpp

clean:
	rm *.o parse

test:
	./parse < test1.txt
test2:
	./parse < test2.txt
test3: 
	./parse < test3.txt
test4:
	./parse < test4.txt
test5:
	./parse < test5.txt
test6:
	./parse < test6.txt

parse.o: scan.h
scan.o: scan.h
