# file to generate test files based on an input configuration
# 
import getopt
import sys

version = '1.0'
verbose = False
output_filename = 'default.out'


options, remainder = getopt.getopt(sys.argv[1:], "r:w:h", ["help", "sync=","type="])
print 'used options   :', options

readers=0
writers=0
test_type='m'
sync_type="non"

for opt, arg in options:
    if opt in ('-r'):
        readers = int(arg)
    elif opt in ('-w', '--output'):
        writers = int(arg)
    elif opt == '--type':
        if arg in ('m','M'):
            test_type = arg
        else:
            print "invalid -t"
    elif opt == '--sync':
        if(arg in ('rwl','non','rcu')):
               sync_type = arg
        else:
               print "invalid sync_type"
    else:
        print "-r readers -w writers --type (m/M) --sync=(rwl/rcu/non)"
        sys.exit()

element = "RadixIPLookup"
element_no = ""

if(sync_type == 'rwl'):
               element_no ="107"
if(sync_type == 'rcu'):
               element_no ="106"

rw_suffix=''
filename=""
router_info="Idle -> rt :: "+element

if(test_type =='M'):
               file = open("167k_table_click.txt","r")
               router_info += element_no
               router_info += file.read()
               filename+='167k_test'
               rw_suffix ='M'
else:
               router_info += element_no
               router_info += "(\n\
                1.1.1.0/32  8.1.1.1 0,\n \
                0.0.0.0/0   8.1.1.1 0,\n \
                ) \n \
 -> Idle;"
               filename+='test'

filename += '_'
filename += sync_type
filename += '_' + str(readers) +'r_'+ str(writers) +'w.click'

body1 ="\n"
body2 ="\nStaticThreadSched(\n"

thread_count =0
for i in range(0,readers,1):
               body1 += "reader"+str(i)+ "::"+"Read"+element+rw_suffix+element_no+ "(rt);\n"
               body2 += "reader" + str(i) + " "+str(thread_count) + ",\n"
               thread_count = thread_count+1

for i in range(0,writers,1):
               body1 += "updater"+str(i)+ "::"+"Pound"+element+rw_suffix+element_no+ "(rt);\n"
               body2 += "updater" + str(i) + " "+ str(thread_count) + ",\n"
               thread_count = thread_count+1

body2 += ");\n"
body2 += "DriverManager(stop);\n"
outputfile = open(filename,'w')
outputfile.write(router_info)
outputfile.write(body1)
outputfile.write(body2)
outputfile.close()
       
