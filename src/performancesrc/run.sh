#!/bin/bash

for DATA in "10 3 2 10 3" "10 3 2 10 5"
do
    STATES=$(echo $DATA | cut -f1 -d ' ')
    TOKENS=$(echo $DATA | cut -f2 -d ' ')
    MCOST=$(echo $DATA | cut -f3 -d ' ')
    MTOTCOST=$(echo $DATA | cut -f4 -d ' ')
    STEPS=$(echo $DATA | cut -f5 -d ' ')
    echo States $STATES Tokens $TOKENS Maxcost $MCOST Maxtotalcost $MTOTCOST Steps $STEPS
    for EXEC in int exact approx
    do
	OUTPUT=$(./$EXEC d d 1 $DATA)
	RUNTIME=$(echo $OUTPUT | cut -f7 -d ' ')
	NODES=$(echo $OUTPUT | cut -f15 -d ' ')
	echo $EXEC \& $RUNTIME \& $NODES
    done
    echo
done
#echo ${DATA[0]} \& ${DATA[1]} \& ${DATA[2]}
#echo ${DATA[*]}x		      

