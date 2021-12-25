#!/bin/bash
#g++ -std=c++11 result.cpp -o result.x
#chmod +x result.sh

for i in {1..24}; do
	./result2.x  np${i}.csv
done