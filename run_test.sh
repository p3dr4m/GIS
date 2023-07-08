#!/usr/bin/env bash

# run make
make

mv ./GIS.exe ./data
i=2

cd ./data || exit
# loop through all the files that are named script*.txt or Script*.txt
for file in ./Script*.txt
do
    # run the GIS.exe program with arg1 as db_test{i},  arg2 as $file, arg3 as log_test{i}.txt
    ./GIS.exe "./db_test${i}.txt" "$file" "./log_test${i}.txt"
    # increment i
    ((i++))
done

