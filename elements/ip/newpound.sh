# Creates a new poundradixiplookup element from one which already exists
# Usage: ./newpound.sh <src_n> <dest_n>
# Where <src_n> numeric suffix of the source element and <dest_n>
# is the numeric suffix of the destination.

src_n=$1
if [ $src_n -eq 1 ]; then
    src_n=''
fi
src_cc=poundradixiplookup$src_n.cc
src_hh=poundradixiplookup$src_n.hh

target_n=$2;
target_cc=poundradixiplookup$target_n.cc
target_hh=poundradixiplookup$target_n.hh

cp -n $src_cc $target_cc
cp -n $src_hh $target_hh

perl -i.bak -pe "s/RadixIPLookup$src_n/RadixIPLookup$target_n/g" $target_cc 
perl -i.bak -pe "s/$src_hh/$target_hh/g" $target_cc 
perl -i.bak -pe "s/RadixIPLookup$src_n/RadixIPLookup$target_n/g" $target_hh
perl -i.bak -pe "s/CLICK_POUNDRADIXIPLOOKUP${src_n}_HH/CLICK_POUNDRADIXIPLOOKUP${target_n}_HH/g" $target_hh
perl -i.bak -pe "s/radixiplookup${src_n}.hh/radixiplookup${target_n}.hh/" $target_cc
perl -i.bak -pe "s/radixiplookup${src_n}.hh/radixiplookup${target_n}.hh/" $target_hh
rm *.bak
