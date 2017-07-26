#!/bin/bash
#brips=1193
#eurica=1408
aida=1111

for((i=0;i<8;i++))
do

name=R${aida}_${i}_packed

if [ -e $AIDADIR/$name.root ]; then
        echo "$name.root exist"
else

./asicpacker $aida $i
fi

done

