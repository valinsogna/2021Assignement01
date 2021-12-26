#!/bin/bash

for filename in ./*.out; do
	python3 parser.py  $(basename "$filename") > $(basename "$filename").csv
	echo  $(basename "$filename" .out).csv
done

