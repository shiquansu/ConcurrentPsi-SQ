#!/bin/bash
module load gcc/4.8.2
perl make.pl
pushd /lustre/medusa/shiquan1/
cp ~/c++framework/ConcurrentPsi-SQ/examples/test7-SQ .
./test7-SQ 3 2 3 8
pushd
