#!/bin/sh
wget http://sqlite.org/2017/sqlite-amalgamation-3190300.zip
unzip sqlite-amalgamation-3190300.zip
cp sqlite-amalgamation-3190300/sqlite3.h .
cp sqlite-amalgamation-3190300/sqlite3.c .
rm -rf sqlite-amalgamation-3190300
rm sqlite-amalgamation-3190300.zip
git clone https://github.com/USCiLab/cereal.git

mkdir build && cd build && cmake -G Ninja .. && ninja


