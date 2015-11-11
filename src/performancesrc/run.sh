#!/bin/bash

PARAMS=(
    "10 3 3 3 3"
    
    "15 5 2 7 7"
    "15 7 2 7 7"
    "15 9 2 7 7"
    "15 11 2 7 7"

    "20 5 2 7 7"
    "30 5 2 7 7"
    "40 5 2 7 7"
    "50 5 2 7 7"

    "20 5 2 7 7"
    "20 5 4 21 7"
    "20 5 6 35 7"
    "20 5 8 49 7"
    "20 5 15 119 7"

    "20 5 4 21 7"
    "20 5 4 30 10"
    "20 5 4 36 12"
    "20 5 4 42 14"
    "20 5 4 60 20"
)

EXECS=(
    int
    intopt
    exact
    approx
)

for DATA in "${PARAMS[@]}"
do
    STATES=$(echo $DATA | cut -f1 -d ' ')
    TOKENS=$(echo $DATA | cut -f2 -d ' ')
    MCOST=$(echo $DATA | cut -f3 -d ' ')
    MTOTCOST=$(echo $DATA | cut -f4 -d ' ')
    STEPS=$(echo $DATA | cut -f5 -d ' ')
    echo States $STATES Tokens $TOKENS Maxcost $MCOST Maxtotalcost $MTOTCOST Steps $STEPS
    SOL=0
    declare -a SOLUTIONS
    for EXEC in "${EXECS[@]}"
    do
	OUTPUT=$(/usr/bin/time -v ./$EXEC d d 1 $DATA 2>&1)
	RUNTIME=$(echo $OUTPUT | cut -f7 -d ' ')
	SOLUTIONS[$SOL]=$(echo $OUTPUT | cut -f11 -d ' ')
	let SOL=SOL+1
	NODES=$(echo $OUTPUT | cut -f15 -d ' ')
	MEM=$(echo $OUTPUT | cut -f87 -d ' ')
	echo $EXEC \& $RUNTIME \& $NODES \& $MEM \\\\
    done

    echo
    for VAL in ${SOLUTIONS[@]}
    do
	if [ $VAL != ${SOLUTIONS[0]} ]
	then
	   echo ERROR!!!
	   exit
	fi
    done

done
#echo ${DATA[0]} \& ${DATA[1]} \& ${DATA[2]}
#echo ${DATA[*]}x		      

