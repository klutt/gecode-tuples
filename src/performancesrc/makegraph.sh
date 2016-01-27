DATAER=$(cat $1 | head -3 | tail -1)
DATAAR=$(cat $1 | head -6 | tail -1)
DATAR=$(cat $1 | head -9 | tail -1)
DATAEN=$(cat $1 | head -4 | tail -1)
DATAAN=$(cat $1 | head -7 | tail -1)
DATAN=$(cat $1 | head -10 | tail -1)
XMIN=$(echo $DATAER | cut -f2 -d '(' | cut -f1 -d ',')
cat graphtemplate.tex | sed "s/DATAEN/$DATAEN/g" | sed "s/DATAAN/$DATAAN/g" |sed "s/DATAN/$DATAN/g" |sed "s/DATAER/$DATAER/g" |sed "s/DATAAR/$DATAAR/g" |sed "s/DATAR/$DATAR/g" | sed "s/XMIN/$XMIN/g"
