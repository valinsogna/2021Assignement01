#!/bin/bash

for filename in ./*.out; do
	python formatter2.py  $(basename "$filename") > $(basename "$filename").csv
	echo  $(basename "$filename" .out).csv
done

