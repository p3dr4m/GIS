#!/usr/bin/env bash

# run make
make

mv ./GIS.exe ./data

# loop through all the files that are named script*.txt or Script*.txt
for file in ./data/script*.txt ./data/Script*.txt
do
    # run the file
    i=$((i+1))
    ./data/GIS.exe "db_test${i}.txt "$file" "log_test${i}.txt"
done

