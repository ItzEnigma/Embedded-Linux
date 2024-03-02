#!/bin/bash

echo "Running Assignment 1 Script"

cd ~
mkdir myDirectory && cd $_
mkdir secondDirectory && cd $_
touch myNotePaper
cd ..
cp secondDirectory/myNotePaper ./myOldNotePaper

