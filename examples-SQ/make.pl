#!/usr/bin/perl

use strict;
use warnings;

use lib '../../PsimagLite/scripts';
use Make;

#my @drivers = ("push_object_into_a_queue", "pthreads_operate_threadsafe_queue", "tasks_dependency_threadsafe_queue", "stall_enable_threadsafe_queue", "testSimple_threadpool", "testObjectMemberFunction_threadpool", "testSimpleObj_threadpool", "testMultiQs_threadpool", "testMultiQsObj_threadpool", "test_threadpool", "testNoC++11_threadppol");
#my @drivers = ("testSimpleObj_threadpool", "testMultiQsObj_threadpool");
#my @drivers = ("test_threadpool");
my @drivers = ("testNoC++11_threadppol");
#my @drivers = ("test1","test2","test3","test4","test5","test6","test7","test7-SQ");

backupMakefile();
writeMakefile();
make();

sub make
{
	system("make");
}

sub backupMakefile
{
	system("cp Makefile Makefile.bak") if (-r "Makefile");
	print "Backup of Makefile in Makefile.bak\n";
}

sub writeMakefile
{
	open(my $fh,">Makefile") or die "Cannot open Makefile for writing: $!\n";

	my $cppflags = " -I../../PsimagLite";
	$cppflags .= " -I../../PsimagLite/src -I../src-SQ";
	my $cxx="g++ -O3 -DNDEBUG -DUSE_PTHREADS";
	my $lapack = " ";

	# C++11 suppport
	#$cppflags .= " -I../src-SQ_c++11";
	$cxx .=" -std=c++11";

	Make::make($fh,\@drivers,"FreeFermions","Linux",0,
	"$lapack    -lm  -lpthread",$cxx,$cppflags,"true"," "," ");

	close($fh);
	print "$0: Done writing Makefile\n";
}
