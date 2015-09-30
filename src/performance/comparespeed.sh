#!/bin/bash

for i in "int" "exact" "approx"; do
    echo $i
    ./$i -mode stat $1 $2 $3 $4 $5 $6 | grep -E "nodes|runtime"
done
