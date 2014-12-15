# DO NOT EDIT!!! Changes will be lost. Modify make.pl instead
# This Makefile was written by make.pl
# FreeFermions by G.A.
# Platform: Linux
# MPI: 0

LDFLAGS =         -lm  -lpthread
CPPFLAGS = -Werror -Wall -Wstrict-overflow=5  -frecord-gcc-switches   -I../../PsimagLite -I../../PsimagLite/src -I../src-SQ -I../src-SQ_c++11
CXX = g++ -O3 -DNDEBUG -DUSE_PTHREADS -std=c++11
all: push_object_into_a_queue pthreads_operate_threadsafe_queue  

push_object_into_a_queue.o: push_object_into_a_queue.cpp  Makefile  
	$(CXX) $(CPPFLAGS) -c push_object_into_a_queue.cpp

push_object_into_a_queue: push_object_into_a_queue.o
	$(CXX) -o  push_object_into_a_queue push_object_into_a_queue.o $(LDFLAGS)
	true push_object_into_a_queue

pthreads_operate_threadsafe_queue.o: pthreads_operate_threadsafe_queue.cpp  Makefile  
	$(CXX) $(CPPFLAGS) -c pthreads_operate_threadsafe_queue.cpp

pthreads_operate_threadsafe_queue: pthreads_operate_threadsafe_queue.o
	$(CXX) -o  pthreads_operate_threadsafe_queue pthreads_operate_threadsafe_queue.o $(LDFLAGS)
	true pthreads_operate_threadsafe_queue


Makefile.dep: push_object_into_a_queue.cpp pthreads_operate_threadsafe_queue.cpp   
	$(CXX) $(CPPFLAGS) -MM push_object_into_a_queue.cpp pthreads_operate_threadsafe_queue.cpp   > Makefile.dep

clean: Makefile.dep
	rm -f core* push_object_into_a_queue pthreads_operate_threadsafe_queue  *.o *.dep  

include Makefile.dep
