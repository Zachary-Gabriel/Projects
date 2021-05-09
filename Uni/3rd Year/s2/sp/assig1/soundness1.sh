#!/bin/bash

ulimit -c 0

BASE=`pwd`
SAMPLES=$BASE/samples
DATA=$BASE/data

TIMEOUT=90s

shopt -q nullglob

setdisp() {
  for f; do
    case "$f" in
      "reset")                  param=0;;
      "bold")                   param="$param;1";;
      "nobold")                 param="$param;21";;
      "underscore")             param="$param;4";;
      "blink")                  param="$param;5";;
      "fg-black"|"black")       param="$param;30";;
      "fg-red"|"red")           param="$param;31";;
      "fg-green"|"green")       param="$param;32";;
      "fg-yellow"|"yellow")     param="$param;33";;
      "fg-blue"|"blue")         param="$param;34";;
      "fg-magenta"|"magenta")   param="$param;35";;
      "fg-cyan"|"cyan")         param="$param;36";;
      "fg-gray"|"gray")         param="$param;37";;
      "fg-dark"|"dark")         param="$param;90";;
      "fg-lred"|"lred")         param="$param;91";;
      "fg-lgreen"|"lgreen")     param="$param;92";;
      "fg-lyellow"|"lyellow")   param="$param;93";;
      "fg-lblue"|"lblue")       param="$param;94";;
      "fg-lmagenta"|"lmagenta") param="$param;95";;
      "fg-lcyan"|"lcyan")       param="$param;96";;
      "fg-white"|"white")       param="$param;97";;
      "fg-default")             param="$param;39";;
      "bg-black")               param="$param;40";;
      "bg-red")                 param="$param;41";;
      "bg-green")               param="$param;42";;
      "bg-yellow")              param="$param;43";;
      "bg-blue")                param="$param;44";;
      "bg-magenta")             param="$param;45";;
      "bg-cyan")                param="$param;46";;
      "bg-gray")                param="$param;47";;
      "bg-default")             param="$param;49";;
      *) return;;
    esac
  done
  echo -ne "\e[${param:1}m"
}

message() {
  color="$1"
  message="$2"
  setdisp $color
  echo "$message"
  setdisp reset
}

hasFile() {
  match=`echo $1`
  test -e "$match"
}


log() {
  echo "$*"
}


error() {
  message lred "Error: $*"
  exit 1
}

warning() {
  message yellow "Warning: $*"
  #printf "\e[32m$*\e[0m\n"
}


folder="$1"
name=`basename $folder`
if [ -d logs ]; then
  log=`pwd`/logs/"$name".log
else
  log=`pwd`/"$name".log
fi

echo ""
echo ""
echo ""
message green "------------------------------------------"
message green "+++ starting $name"
message green "------------------------------------------"

case "$name" in
  *.tar) tarflags="-xf";;
  *.tgz) tarflags="-zxf";;
  *) error "$name: bad extension";;
esac

RUNDIR=`mktemp -d  SND-SPA1.XXXXX` || runerror "Cannot create test run directory"
echo "Created test directory `setdisp blue`$RUNDIR`setdisp reset`"
RUNDIR=`realpath "$RUNDIR"` || runerror "Cannot get real path of test run directory"

tar -C "$RUNDIR" $tarflags "$name" || error "$name: Cannot extract"
FILE="$name"

cd "$RUNDIR"

USER=`echo *`
case "$USER" in
  a[0-9][0-9][0-9][0-9][0-9][0-9][0-9]);;
  *) error "$FILE: Contents does not seem to be a student number"
esac

test -h "$USER" && error "$USER: is a symlink"
test -d "$USER" || error "$USER: is not a directory"
chmod -R u+rw "$USER" || error "$USER: could not chmod???"

echo "Found Student ID `setdisp blue`$USER`setdisp reset`"

cd $USER
hasFile "info.txt" || error "$FILE: No info.txt"

test `wc -l info.txt| awk '{print $1}'` = '2' || error "$FILE: info,txt: bad format"
Student=`head -1 info.txt`
ID=`tail -1 info.txt`
echo "Found Student `setdisp blue`$Student`setdisp reset`"
test "$ID" = "$USER"|| error "$FILE: Student ID mismatch"

hasFile "assignment1.pdf" || error "$FILE: No assignment1.pdf"


test -d "part2" || warning "$FILE: No directory named 'part2'."
test -d "part3" || error "$FILE: No directory named 'part3' - Part 3 will not be marked."

chmod -R u+rwx "part3" || error "$FILE: part3: could not chmod???"
cd part3 || error "$FILE: part3: cannot chdir???"

hasFile "*.o" && error "$FILE: Includes .o file"
hasFile "libmalloc.so" && error "$FILE: Includes libmalloc.so"

if [ -f Makefile -o -f makefile ]; then
  make >& make.log
  case $? in
  0) log "--- Make completed";;
  *) error "+++ Make failed";;
  esac
else
   error "+++ Makefile does not exist"
fi

if [ ! -x libmalloc.so ]; then
  error "+++ libmalloc.so not created"
fi

testsymbol() {
  val=`nm libmalloc.so | awk '{if ($3 == "'"$1"'" && $2 == "T") print $1}'`
  test ! -z "$val"
}

testsymbol malloc || error "No malloc in libmalloc.so"
testsymbol calloc || error "No calloc in libmalloc.so"
testsymbol realloc || error "No realloc in libmalloc.so"
testsymbol free || error "No free in libmalloc.so"
testsymbol SP2020_HeapStats || error "No SP2020_HeapStats in libmalloc.so"

cat > helloworld.c <<EOF
#include <stdio.h>
#include <stdlib.h>
int main() {
printf("Hello world!\n");
exit(0);
}
EOF

cat > heapstats.c <<EOF
#include <stdlib.h>
struct heapStats {
  size_t hs_heapBytes;
  size_t hs_allocatedBytes;
  size_t hs_allocatedChunks;
  size_t hs_freeChunks;
  size_t hs_metadataBytes;
};

void SP2020_HeapStats(struct heapStats *stats);
int main() {
  struct heapStats hs = {0, 0, 0, 0, 0};
  char *p = malloc(1);
  SP2020_HeapStats(&hs);
  exit(0);
}
EOF


echo "Hello world!" > ../../hw-output
cc -o helloworld helloworld.c || error "Cannot build helloworld???"
LD_PRELOAD=./libmalloc.so ./helloworld > ../../hw-malloc-output 2> ../../hw-malloc-error || error "+++ Failed using libmalloc.so"

diff ../../hw-output ../../hw-malloc-output || error "$FILE: Output differs"
if [ -s ../../hw-malloc-error ]; then
  error "$FILE: Output to stderr"
fi

cc -o heapstats heapstats.c ./libmalloc.so || error "Cannot use SP2020_HeapStats()"
./heapstats || error "Cannot use SP2020_HeapStats()"
setdisp lgreen; echo "Soundness test passed, but check for warnings"; setdisp reset
