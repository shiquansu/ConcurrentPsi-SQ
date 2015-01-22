# DO NOT EDIT!!! Changes will be lost. Modify make.pl instead
# This Makefile was written by make.pl
# FreeFermions by G.A.
# Platform: Linux
# MPI: 0

LDFLAGS =         -lm  -lpthread
CPPFLAGS = -Werror -Wall -Wstrict-overflow=5  -frecord-gcc-switches   -I../../PsimagLite -I../../PsimagLite/src -I../src-SQ -I../src-SQ_c++11
CXX = g++ -O3 -DNDEBUG -DUSE_PTHREADS -std=c++11
all: testSimple_threadpool_object  

testSimple_threadpool_object.o: testSimple_threadpool_object.cpp  Makefile  
	$(CXX) $(CPPFLAGS) -c testSimple_threadpool_object.cpp

testSimple_threadpool_object: testSimple_threadpool_object.o
	$(CXX) -o  testSimple_threadpool_object testSimple_threadpool_object.o $(LDFLAGS)
	true testSimple_threadpool_object


Makefile.dep: testSimple_threadpool_object.cpp   
	$(CXX) $(CPPFLAGS) -MM testSimple_threadpool_object.cpp   > Makefile.dep

clean: Makefile.dep
	rm -f core* testSimple_threadpool_object  *.o *.dep  

include Makefile.dep
