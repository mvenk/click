# Generates a 5 * [1..2^20] output file containing 1 to 2^20 routes.
import random
initial=[]
l1=[]
def hton(add):
    ip1 = str((add & 0xff000000) >> 24);
    ip2 = str((add & 0x00ff0000) >> 16);
    ip3 = str((add & 0x0000ff00) >> 8);
    ip4 = str((add & 0x000000ff));
    ip = ip1+"."+ip2+"."+ip3+"."+ip4
    return ip

def get_ip():
    for ip in range(0, 2**20):
        yield hton(ip << 12)

for ip in get_ip():
    initial.append(ip);

for i in range(0,5):
    random.shuffle(initial)
    l1.extend(initial)

print "!data ip_dst"
for item in l1:
    print item
