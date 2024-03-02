#!/bin/bash

cd ~
mkdir script_exercise

for ((i=1;i<=5;i++))
do
    date > "file$i.txt"
    echo "file$i.txt successfully created"
done

echo "Printing Files Information"

for ((i=1;i<=5;i++))
do

    cat "file$i.txt"

done
