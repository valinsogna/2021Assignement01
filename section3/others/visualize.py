
import sys

file_name=sys.argv[1]
error = sys.argv[2]

in_file = open(file_name)

total = 0

for line in in_file:

	if line.find("MLU")>=0:
		cool_line = line.rstrip()
		total += float(cool_line[-13:])
		
		
mean_ = total/10
	
for lines in open(error):
	
	stripped = lines.rstrip()
	last = ""
	i = 0
	correct = 0
	for l in stripped:
		if last == "d" and l == ":":
			correct = i
		last = l
		i = i+1
	
	cor1=correct+2
	cor2=correct+7
	
	elapsed = lines[cor1:cor2]
	
	break


nodes = file_name.strip("thin_core")
nodes = file_name.strip("thin_socket")
nodes = nodes.replace("_",",")
#nodes = nodes.strip("re,")
#nodes = nodes.strip("de,")

#nodes = nodes.strip("gpu,core,")
nodes = nodes.strip("gpu,socket,")

#print(nodes)

#print (nodes[:-3] + "," + str(mean_) + "," + elapsed)
print (nodes[:-1] + "," + str(mean_) + "," + elapsed)
	
