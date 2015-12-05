#!/bin/bash

cp run.sh ../performancesrc/

PARAMS=(
    "15 5 3 12 6"
    "15 6 3 12 6"
    "15 7 3 12 6"
    "15 8 3 12 6"

    "20 5 3 12 6"
    "30 5 3 12 6"
    "40 5 3 12 6"
    "50 5 3 12 6"

    "20 5 2 7 7"
    "20 5 4 21 7"
    "20 5 6 35 7"
    "20 5 8 49 7"
    "20 5 15 119 7"

    "20 5 4 21 7"
    "20 5 4 30 8"
    "20 5 4 36 9"
    "20 5 4 42 10"
)

EXECS=(
    exact
    approx
    intopt
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
	RUNTIME=$(echo $OUTPUT | cut -f8 -d ' ' | sed 's/(//g')
	SOLUTIONS[$SOL]=$(echo $OUTPUT | cut -f11 -d ' ')
	let SOL=SOL+1
	NODES=$(echo $OUTPUT | cut -f15 -d ' ')
#	MEM=$(echo $OUTPUT | cut -f87 -d ' ')
	echo $EXEC $RUNTIME $NODES
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

