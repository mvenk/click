import sys, string
input_file = sys.argv[1]
input = open(input_file,"r").readlines()
cdf_dict={}

def contains(theString, theQueryValue):
  return theString.find(theQueryValue) > -1

#i=0
for line in input:
   # print i
   # i=i+1
    address= line.split(' ')[0]
    if contains(address,'/'):
        length = address.split('/')[1]
    else:
        length = "32"
    if cdf_dict.has_key(length):
        cdf_dict[length]+=1
    else:
        cdf_dict[length]=1
for k, v in cdf_dict.iteritems():
    print k,",",v
