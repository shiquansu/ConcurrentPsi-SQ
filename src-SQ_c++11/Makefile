# DO NOT EDIT!!! Changes will be lost. Modify ../examples-SQ/make.pl instead
# This Makefile was written by ../examples-SQ/make.pl
# FreeFermions by G.A.
# Platform: Linux
# MPI: 0

LDFLAGS =         -lm  -lpthread
CPPFLAGS = -Werror -Wall -Wstrict-overflow=5  -frecord-gcc-switches   -I../../PsimagLite -I../../PsimagLite/src -I../src-SQ -I../src-SQ_c++11
CXX = g++ -O3 -DNDEBUG -DUSE_PTHREADS -std=c++11
all: testSimple_threadpool  

testSimple_threadpool.o: testSimple_threadpool.cpp  Makefile  
	$(CXX) $(CPPFLAGS) -c testSimple_threadpool.cpp

testSimple_threadpool: testSimple_threadpool.o
	$(CXX) -o  testSimple_threadpool testSimple_threadpool.o $(LDFLAGS)
	true testSimple_threadpool


Makefile.dep: testSimple_threadpool.cpp   
	$(CXX) $(CPPFLAGS) -MM testSimple_threadpool.cpp   > Makefile.dep

clean: Makefile.dep
	rm -f core* testSimple_threadpool  *.o *.dep  

include Makefile.dep
