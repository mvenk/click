# Generates 2**20 routes to be put into the routing table
def hton(add):
    ip1 = str((add & 0xff000000) >> 24);
    ip2 = str((add & 0x00ff0000) >> 16);
    ip3 = str((add & 0x0000ff00) >> 8);
    ip4 = str((add & 0x000000ff));
    ip = ip1+"."+ip2+"."+ip3+"."+ip4
    return ip

def get_ip():
    for ip in range(1, 2**20):
        yield hton(ip << 12)

print "ControlSocket(TCP, 4444);"
print "rt::RadixIPLookup(";
print "0.0.0.0/0 1,"
for ip in get_ip():
    print ip + "/32 1,"
print ");"
print "FromIPSummaryDump(\"5m32.dump\", STOP true) -> rt;"
print "rt[0] -> ToIPSummaryDump(\"out.dump\");"
print "rt[1]->Discard();"
