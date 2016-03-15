VARS=(
#    "states"
#    "symbols"
#    "cost"
    "steps"
    )

for VAR in ${VARS[@]}
do
    ./run.sh $VAR.par $VAR.txt
    ./makegraph.sh $VAR
#    cp $VAR.tex ../../doc/
done
