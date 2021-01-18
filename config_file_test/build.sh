#!/bin/bash

g++ -std=c++11 -o test main.cpp SCAConfig.cpp
./test
rm test
