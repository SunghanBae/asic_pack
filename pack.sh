#!/bin/bash
#brips=1193
#eurica=1408
<<<<<<< HEAD
aida=1121

for((i=0;i<5;i++))
=======
aida=1111

for((i=0;i<8;i++))
>>>>>>> 079cd85d2b7ebdc830da287210fd6d8a5f1c7e9e
do

name=R${aida}_${i}_packed

if [ -e $AIDADIR/$name.root ]; then
        echo "$name.root exist"
else

./asicpacker $aida $i
fi

done

