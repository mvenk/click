#!/bin/sh

# findelements.sh -- script analyzes element source code during build process
# Eddie Kohler
#
# Copyright (c) 1999-2000 Massachusetts Institute of Technology
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, subject to the following
# conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# Further elaboration of this license, including a DISCLAIMER OF ANY
# WARRANTY, EXPRESS OR IMPLIED, is provided in the LICENSE file, which is
# also accessible at http://www.pdos.lcs.mit.edu/click/license.html

# determine mode
prefix=""
all=0
verbose=""
provisions=""
while [ x"$1" != x ]; do
case $1 in
  -p|--pre|--pref|--prefi|--prefix)
     shift 1; prefix="$1/"; shift 1;;
  -p*)
     prefix=`echo "$1" | sed 's/^-p//'`/; shift 1;;
  --pre=*|--pref=*|--prefi=*|--prefix=*)
     prefix=`echo "$1" | sed 's/^[^=]*=//'`/; shift 1;;
  -V|--v|--ve|--ver|--verb|--verbo|--verbos|--verbose)
     verbose=1; shift 1;;
  -a|--a|--al|--all)
     all=1; shift 1;;
  -r|--pro|--prov|--provi|--provid|--provide)
     shift 1; provisions="$1
$provisions"; shift 1;;
  -r*)
     provisions=`echo "$1" | sed 's/^-r//'`"
$provisions"; shift 1;;
  --pro=*|--prov=*|--provi=*|--provid=*|--provide=*)
     provisions=`echo "$1" | sed 's/^[^=]*=//'`"
$provisions"; shift 1;;
  -h|--h|--he|--hel|--help)
     cat <<'EOF' 1>&2
`Findelements.sh' locates valid Click element source code according to a set
of provisions. It starts from a collection of source code, then repeatedly
eliminates files whose requirements are not available until a stable set of
sources is reached. It expects a list of files and directories on standard
input. Directories are searched for .cc source files; any files found are
considered. The initial list of available requirements is the list of
directories given on the standard input, plus the list of EXPORT_ELEMENT()
and ELEMENT_PROVIDES() keywords, plus any extra requirements specified
with `-r'.

Usage: findelements.sh [-a] [-V] [-pPREFIX] < [FILES AND DIRECTORIES]

Options:
  -a, --all                Include all subdirectories of `elements' rather
                           than reading standard input.
  -V, --verbose            Print more information about dependency checking.
  -p, --prefix PREFIX      Preprend PREFIX to every file and/or directory.
  -r, --provide REQ        Mark the requirement REQ as initially available.
  -h, --help               Print this message and exit.

Report bugs to <click@pdos.lcs.mit.edu>.
EOF
     exit 0;;
  *)
     echo "Usage: ./findelements.sh [-a] [-V] [-pPREFIX] < [FILES AND DIRECTORIES]" 1>&2
     exit 1;;
esac
done

if test -n "$verbose" -a -n "$prefix"; then
  echo "Prefix: $verbose" 1>&2
fi

# expand list of files
if test $all = 1; then
  first_files=`cd ${prefix}elements; ls`
else
  first_files=`cat`
fi

files=""
for i in $first_files; do
  pprefix="$prefix"
  if test -d "${prefix}elements/$i" && echo "$i" | grep -v '^\.' >/dev/null; then
    pprefix="${prefix}elements/"
  fi
  if test -d "${pprefix}$i"; then
    provisions=`echo $i | sed 's/^\.\///'`"
$provisions"
    files="$files
"`find ${pprefix}$i \( -name \*.cc -o -name \*.c \) -print | grep -v '/[.,][^/]*$'`
  else
    files="$files
$i"
  fi
done
files=`echo "$files" | sort | uniq | grep .`

# die if no files
if test -z "$files"; then
  echo "no files found" 1>&2
  exit 1
fi

# find a good version of awk
if test -x /usr/bin/gawk; then
  awk=gawk
elif test -x /usr/bin/nawk; then
  awk=nawk
else
  awk=awk
fi

# check dependencies: generate a list of bad files, then remove those files
# from the list of good files
bad_files=''
while true; do
  exports1=`grep '^EXPORT_ELEMENT' $files | sed 's/.*(\(.*\)).*/\1/'`
  exports2=`grep '^ELEMENT_PROVIDES' $files | sed 's/.*(\(.*\)).*/\1/'`
  awk_exports=`echo "$exports1"'
'"$exports2"'
'"$provisions" | sed 's/\(..*\)/dep["\1"]=1;/'`
  new_bad_files=`grep '^ELEMENT_REQUIRES' $files | $awk -F: 'BEGIN {OFS="";'"$awk_exports"'dep["true"]=1; dep["1"]=1;
}
{
  sub(/ELEMENT_REQUIRES\(/, "", $2);
  sub(/\)/, "", $2);
  split($2, deps, / +/);
  for (j in deps) {
    i = deps[j]
    if (!dep[i]) {
      print $1;
      break;
    }
  }
}' | sort | uniq`
  if test -n "$verbose"; then
    echo
    echo "Files: $files" 1>&2
    echo
    echo "Bad files: $bad_files" 1>&2
  fi
  if test -z "$new_bad_files"; then
    break
  else
    files=`echo "$files
$new_bad_files" | sort | uniq -u`
    bad_files="$new_bad_files
$bad_files"
  fi
done

# output files
echo "$files"
