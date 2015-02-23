#!/bin/bash

cd /lustre/medusa/$USER

aprun -n 1 /nics/b/home/shiquan1/c++framework/ConcurrentPsi-SQ/examples-SQ/$1 $2 $3

cd ~/c++framework/ConcurrentPsi-SQ/examples-SQ 
