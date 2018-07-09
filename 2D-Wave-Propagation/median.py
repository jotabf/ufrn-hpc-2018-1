import statistics
import sys

for line in open(sys.argv[1]):
    array = line.split('\t')
    _mean = []    
    array = array
    for i in array:
    	if i != "\n":
        	_mean.append(float(i))
    print(statistics.median(array))