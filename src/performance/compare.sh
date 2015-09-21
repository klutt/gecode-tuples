#!/bin/bash

for command in "./int" "./approx" "./exact" ; do
    echo $command
    mytime="$(time ( $command $1 $2 $3 $4 $5 $6 ) 2>&1 1>/dev/null )"; echo "$mytime"
done
