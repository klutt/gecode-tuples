#!/bin/bash

cp run.sh ../performancesrc/
echo using $1 and writes to $2
source $1
#PARAMS=(
#    "5 7 15 20 7"
#    "5 8 15 20 7"
#    "5 9 15 20 7"
#    "5 10 15 20 7"
    
#    "20 5 3 6 6"
#    "30 5 3 6 6"
#    "40 5 3 6 6"
#    "50 5 3 6 6"

#    "20 5 2 4 7"
#    "20 5 4 11 7"
#    "20 5 6 17 7"
#    "20 5 8 25 7"
#    "20 5 15 60 7"

#    "20 5 4 11 7"
#    "20 5 4 15 8"
#    "20 5 4 18 9"
#    "20 5 4 21 10"
#)


EXECS=(
    exact
    approx
    intopt
)

echo > $2

for EXEC in "${EXECS[@]}"
do
    STRNODES=""
    STRRUNTIME=""
    for DATA in "${PARAMS[@]}"
    do
    TOTRUN=0
    TOTNODES=0
	STATES=$(echo $DATA | cut -f1 -d ' ')
	TOKENS=$(echo $DATA | cut -f2 -d ' ')
	MCOST=$(echo $DATA | cut -f3 -d ' ')
	MTOTCOST=$(echo $DATA | cut -f4 -d ' ')
	STEPS=$(echo $DATA | cut -f5 -d ' ')
	
	XVAR=$(echo $DATA | cut -f$XAUX -d ' ')
	echo Exec $EXEC States $STATES Tokens $TOKENS Maxcost $MCOST Maxtotalcost $MTOTCOST Steps $STEPS
	printf "DFA: "
	for RNDDFA in $(seq 10)
	do
	    OUTPUT=$(/usr/bin/time -v ./$EXEC d d $RNDDFA $DATA 2>&1)
	    RUNTIME=$(echo $OUTPUT | cut -f8 -d ' ' | sed 's/(//g' | cut -f1 -d '.')
	    NODES=$(echo $OUTPUT | cut -f15 -d ' ')
	    let TOTRUN=$TOTRUN+$RUNTIME
	    let TOTNODES=$TOTNODES+$NODES
	    printf "$RNDDFA "
	done
	echo
	echo $EXEC $TOTRUN $TOTNODES
	STRNODES="$STRNODES ($XVAR,$TOTNODES)"
	STRRUNTIME="$STRRUNTIME ($XVAR, $TOTRUN)"
    done
    	echo $STRNODES
	echo $STRRUNTIME
	echo $EXEC >> $2
	echo $STRNODES >> $2
	echo $STRRUNTIME >> $2

done
#echo ${DATA[0]} \& ${DATA[1]} \& ${DATA[2]}
#echo ${DATA[*]}x		      

