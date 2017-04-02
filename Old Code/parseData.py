inputFile = open("dataSet1.txt", "r")
dataLines = inputFile.readlines()
inputFile.close()

for i in xrange(len(dataLines)):
	dataLines[i] = dataLines[i].split()
	
outputFile = open("table1.inc", "w")

outputFile.write("{\n")
for line in dataLines:
	outputFile.write("\t{{")
	for i in xrange(9):
		outputFile.write(str(line[i]))
		if i != 8:
			outputFile.write(", ")
		else:
			outputFile.write("},\n\t{")
	for i in xrange(9, 18):
		outputFile.write(str(line[i]))
		if i != 17:
			outputFile.write(", ")
		else:
			outputFile.write("}}")
			if line != dataLines[-1]:
				outputFile.write(",\n\n")
			else:
				outputFile.write("\n};")
				
outputFile.close()

	
# output looks like this:
#{
#	{{21, 11, 21, 32, 1, 52, 21},
#	{0, 0, 0, 0, 1, 1, 0, 1, 0}}
	
#	{{21, 11, 21, 32, 1, 52, 21},
#	{0, 0, 0, 0, 1, 1, 0, 1, 0}}
		
#	{{21, 11, 21, 32, 1, 52, 21},
#	{0, 0, 0, 0, 1, 1, 0, 1, 0}}
#}