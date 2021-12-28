#!/bin/bash

#thin_core, thin_socket

cd gpu

name="gpu_socket";

if [ -f "$name" ]; then
	rm ${name}.cvs
fi

echo "#np,nx,ny,nz,mlup,elapsed" > ../${name}.csv

for filename in ./${name}*; do
	#cat $(basename "$filename") | grep -v ^\# | grep -v '^$' | sed -r 's/^\s+//;s/\s+/,/g' > $(basename "$filename")
	#"${a} ${b}"
	#errorname="./gpu_error${filename:2}"
	errorname="./gpu_error${filename:6}"

	python ../visualize.py  $(basename "$filename") $(basename "$errorname") >> ../${name}.csv
	#echo  $(basename "$filename" .out).csv
done
