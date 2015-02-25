# DO NOT EDIT!!! Changes will be lost. Modify make.pl instead
# This Makefile was written by make.pl
# FreeFermions by G.A.
# Platform: Linux
# MPI: 0

LDFLAGS =         -lm  -lpthread
CPPFLAGS = -Werror -Wall -Wstrict-overflow=5  -frecord-gcc-switches   -I../../PsimagLite -I../../PsimagLite/src -I../src-SQ
CXX = g++ -O3 -DNDEBUG -DUSE_PTHREADS -std=c++11
all: testNoC++11_threadppol  

testNoC++11_threadppol.o: testNoC++11_threadppol.cpp  Makefile  
	$(CXX) $(CPPFLAGS) -c testNoC++11_threadppol.cpp

testNoC++11_threadppol: testNoC++11_threadppol.o
	$(CXX) -o  testNoC++11_threadppol testNoC++11_threadppol.o $(LDFLAGS)
	true testNoC++11_threadppol


Makefile.dep: testNoC++11_threadppol.cpp   
	$(CXX) $(CPPFLAGS) -MM testNoC++11_threadppol.cpp   > Makefile.dep

clean: Makefile.dep
	rm -f core* testNoC++11_threadppol  *.o *.dep  

include Makefile.dep
