#!/bin/bash

g++ -std=c++11 -o test main.cpp ArgOpt.cpp
./test
rm test
