#!/usr/bin/perl

use strict;
use warnings;

use lib '../../PsimagLite/scripts';
use Make;

my @drivers = ("test1");

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
	#my $cxx="g++ -O3 -DNDEBUG";
	my $cxx="g++ -O3 -DNDEBUG -DUSE_PTHREADS";
	#my $lapack = Make::findLapack();
        my $lapack = "/nics/b/home/shiquan1/lapack-3.4.1/liblapack.a";

	Make::make($fh,\@drivers,"FreeFermions","Linux",0,
	"$lapack    -lm  -lpthread",$cxx,$cppflags,"true"," "," ");

	close($fh);
	print "$0: Done writing Makefile\n";
}
