#!/bin/bash

cp run.sh ../performancesrc/
echo using $1 and writes to $2
source $1

EXECS=(
    exact
    approx
    intopt
)

echo > $2

INDEX=0
SOLS=(0 0 0)
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
	    SOL=$(echo $OUTPUT | cut -f11 -d ' ')
	    let TOTRUN=$TOTRUN+$RUNTIME
	    let TOTNODES=$TOTNODES+$NODES
	    printf "$RNDDFA "
	    TOTSOL="${SOLS["$INDEX"]}"
	    let TOTSOL=$TOTSOL+$SOL
	    SOLS[$INDEX]=$TOTSOL
	done
	echo
	echo $EXEC $TOTRUN $TOTNODES
	STRNODES="$STRNODES ($XVAR,$TOTNODES)"
	STRRUNTIME="$STRRUNTIME ($XVAR, $TOTRUN)"
    done
    let INDEX=1+$INDEX
    echo $STRNODES
    echo $STRRUNTIME
    echo $EXEC >> $2
    echo $STRRUNTIME >> $2
    echo $STRNODES >> $2
     

done

if [ "${SOLS[0]}" -ne "${SOLS[1]}" ] || [ "${SOLS[1]}" -ne "${SOLS[2]}" ]
then
    echo "     ERROR! Number of solutions mismatch"
fi

#echo ${DATA[0]} \& ${DATA[1]} \& ${DATA[2]}
#echo ${DATA[*]}x		      

