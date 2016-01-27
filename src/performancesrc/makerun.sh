VARS=(
    "states"
    "symbols"
    "cost"
    "steps"
    )

for VAR in ${VARS[@]}
do
    ./run.sh $VAR.par $VAR.txt
    ./makegraph.sh $VAR.txt > $VAR.tex
done

cp *.tex ../../doc
