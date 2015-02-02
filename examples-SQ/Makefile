# DO NOT EDIT!!! Changes will be lost. Modify make.pl instead
# This Makefile was written by make.pl
# FreeFermions by G.A.
# Platform: Linux
# MPI: 0

LDFLAGS =         -lm  -lpthread
CPPFLAGS = -Werror -Wall -Wstrict-overflow=5  -frecord-gcc-switches   -I../../PsimagLite -I../../PsimagLite/src -I../src-SQ -I../src-SQ_c++11
CXX = g++ -O3 -DNDEBUG -DUSE_PTHREADS -std=c++11
all: test_threadpool  

test_threadpool.o: test_threadpool.cpp  Makefile  
	$(CXX) $(CPPFLAGS) -c test_threadpool.cpp

test_threadpool: test_threadpool.o
	$(CXX) -o  test_threadpool test_threadpool.o $(LDFLAGS)
	true test_threadpool


Makefile.dep: test_threadpool.cpp   
	$(CXX) $(CPPFLAGS) -MM test_threadpool.cpp   > Makefile.dep

clean: Makefile.dep
	rm -f core* test_threadpool  *.o *.dep  

include Makefile.dep
