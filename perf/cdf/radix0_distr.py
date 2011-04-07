import sys, string
input_file = sys.argv[1]
input = open(input_file,"r").xreadlines()
cdf_dict={}


for line in input:
    key = int(line)
    if cdf_dict.has_key(key):
        cdf_dict[key]+=1
    else:
        cdf_dict[key]=1

for k, v in cdf_dict.iteritems():
    print k,",",v
