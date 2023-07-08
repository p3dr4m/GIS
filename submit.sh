#!/usr/bin/env bash
mkdir -p ./submit

cp -r ./src ./submit
cp GIS.cpp ./submit
cp Makefile ./submit
cp class_diagram.png ./submit
cp ./doc/GIS_System_Algo_Report.pdf ./submit

# Navigate into the 'submit' directory
cd ./submit || exit

# Create the zip from within the 'submit' directory
zip -r ../submit.zip ./*

# Navigate back out of the 'submit' directory
cd ..

# Remove the 'submit' directory
rm -rf ./submit
