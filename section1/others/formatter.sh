#!/bin/bash
#g++ -std=c++11 formatter.cpp -o formatter.x
#chmod +x formatter.sh
#./formatter.sh

for i in {1..24}; do
	./formatter.x  np${i}.csv
done