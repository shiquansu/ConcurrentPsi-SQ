#!/usr/bin/perl

use strict;
use warnings;

use lib '../../PsimagLite/scripts';
use Make;

my @drivers = ("test1","test2","test3","test4","test5","test6","test7","test7-SQ");

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

	my $cppflags = " -I../../PsimagLite ";
	$cppflags .= " -I../../PsimagLite/src -I../src";
	my $cxx="g++ -O3 -DNDEBUG -DUSE_PTHREADS";
	my $lapack = " ";

	Make::make($fh,\@drivers,"FreeFermions","Linux",0,
	"$lapack    -lm  -lpthread",$cxx,$cppflags,"true"," "," ");

	close($fh);
	print "$0: Done writing Makefile\n";
}
