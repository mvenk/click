# Creates a new radixiplookup element from one which already exists
# Usage: ./newradix.sh <src_n> <dest_n>
# Where <src_n> numeric suffix of the source element and <dest_n>
# is the numeric suffix of the destination.

src_n=$1
if [ $src_n -eq 1 ]; then
    src_n=''
fi
src_cc=radixiplookup$src_n.cc
src_hh=radixiplookup$src_n.hh

target_n=$2;
target_cc=radixiplookup$target_n.cc
target_hh=radixiplookup$target_n.hh

cp -n $src_cc $target_cc
cp -n $src_hh $target_hh

perl -i.bak -pe "s/RadixIPLookup$src_n/RadixIPLookup$target_n/" $target_cc 
perl -i.bak -pe "s/$src_hh/$target_hh/" $target_cc 
perl -i.bak -pe "s/RadixIPLookup$src_n/RadixIPLookup$target_n/" $target_hh
perl -i.bak -pe "s/CLICK_RADIXIPLOOKUP${src_n}_HH/CLICK_RADIXIPLOOKUP${target_n}_HH/" $target_hh
rm *.bak

