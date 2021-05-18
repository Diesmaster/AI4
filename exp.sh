#!/bin/bash

echo "type, error"

for (( counter=30; counter>0; counter-- ))
do
  ./nnskelet 3 8 100000 1 $RANDOM
done
for (( counter=30; counter>0; counter-- ))
do
  ./nnskelet 3 8 100000 2 $RANDOM
done
for (( counter=30; counter>0; counter-- ))
do
  ./nnskelet 3 8 100000 3 $RANDOM
done
