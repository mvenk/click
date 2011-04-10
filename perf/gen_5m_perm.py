# Generates 2**20 routes to be put into the routing table
def hton(add):
    ip1 = str((add & 0xff000000) >> 24);
    ip2 = str((add & 0x00ff0000) >> 16);
    ip3 = str((add & 0x0000ff00) >> 8);
    ip4 = str((add & 0x000000ff));
    ip = ip1+"."+ip2+"."+ip3+"."+ip4
    return ip

def get_ip():
    import random
    for ip in range(1, 5*1000000):
        yield hton(random.randint(1, 2**20-1)<<12)

print "!data ip_dst"
for i in get_ip():
    print i
