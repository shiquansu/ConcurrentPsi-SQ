# DO NOT EDIT!!! Changes will be lost. Modify make.pl instead
# This Makefile was written by make.pl
# FreeFermions by G.A.
# Platform: Linux
# MPI: 0

LDFLAGS =         -lm  -lpthread
CPPFLAGS = -Werror -Wall -Wstrict-overflow=5  -frecord-gcc-switches   -I../../PsimagLite  -I../../PsimagLite/src -I../src
CXX = g++ -O3 -DNDEBUG -DUSE_PTHREADS -std=c++11
all: test1 test2 test3 test4 test5 test6 test7 test7-SQ  

test1.o: test1.cpp  Makefile  
	$(CXX) $(CPPFLAGS) -c test1.cpp

test1: test1.o
	$(CXX) -o  test1 test1.o $(LDFLAGS)
	true test1

test2.o: test2.cpp  Makefile  
	$(CXX) $(CPPFLAGS) -c test2.cpp

test2: test2.o
	$(CXX) -o  test2 test2.o $(LDFLAGS)
	true test2

test3.o: test3.cpp  Makefile  
	$(CXX) $(CPPFLAGS) -c test3.cpp

test3: test3.o
	$(CXX) -o  test3 test3.o $(LDFLAGS)
	true test3

test4.o: test4.cpp  Makefile  
	$(CXX) $(CPPFLAGS) -c test4.cpp

test4: test4.o
	$(CXX) -o  test4 test4.o $(LDFLAGS)
	true test4

test5.o: test5.cpp  Makefile  
	$(CXX) $(CPPFLAGS) -c test5.cpp

test5: test5.o
	$(CXX) -o  test5 test5.o $(LDFLAGS)
	true test5

test6.o: test6.cpp  Makefile  
	$(CXX) $(CPPFLAGS) -c test6.cpp

test6: test6.o
	$(CXX) -o  test6 test6.o $(LDFLAGS)
	true test6

test7.o: test7.cpp  Makefile  
	$(CXX) $(CPPFLAGS) -c test7.cpp

test7: test7.o
	$(CXX) -o  test7 test7.o $(LDFLAGS)
	true test7

test7-SQ.o: test7-SQ.cpp  Makefile  
	$(CXX) $(CPPFLAGS) -c test7-SQ.cpp

test7-SQ: test7-SQ.o
	$(CXX) -o  test7-SQ test7-SQ.o $(LDFLAGS)
	true test7-SQ


Makefile.dep: test1.cpp test2.cpp test3.cpp test4.cpp test5.cpp test6.cpp test7.cpp test7-SQ.cpp   
	$(CXX) $(CPPFLAGS) -MM test1.cpp test2.cpp test3.cpp test4.cpp test5.cpp test6.cpp test7.cpp test7-SQ.cpp   > Makefile.dep

clean: Makefile.dep
	rm -f core* test1 test2 test3 test4 test5 test6 test7 test7-SQ  *.o *.dep  

include Makefile.dep
