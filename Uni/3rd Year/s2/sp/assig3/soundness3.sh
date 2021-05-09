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


folder=`realpath "$1"`
name=`basename "$folder"`
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

RUNDIR=`mktemp -d  SND-SPA3.XXXXX` || runerror "Cannot create test run directory"
echo "Created test directory `setdisp blue`$RUNDIR`setdisp reset`"
RUNDIR=`realpath "$RUNDIR"` || runerror "Cannot get real path of test run directory"

tar -C "$RUNDIR" $tarflags "$folder" || error "$name: Cannot extract"
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

echo "" | cat info.txt - | sed '/^$/d;s/\r//g' | {
  read Student || exit 1
  echo "Found Student `setdisp blue`$Student`setdisp reset`"
  read ID || exit 1
  test "$ID" = "$USER"|| warning "$FILE: Student ID mismatch"
  read XX && exit 1
  exit 0
} || error "info.txt: Bad format"


hasFile "assignment3.pdf" || error "$FILE: No assignment3.pdf"

message lgreen "Part 1 will be marked."

hasFile "driver.c" || error "$FILE: No driver.c"

mkdir compile
cd compile
cat > spalloc.c <<EOF
#include <stdlib.h>
#include <stdint.h>

void sp_seed(uint32_t seed) {}

uint32_t sp_rand() { return 0; }

void *sp_realloc(void *chunk, size_t size) {
  return realloc(chunk, size);
}
void sp_free(void *chunk) {
  free(chunk);
}

void *sp_calloc(size_t nmemb, size_t size) {
  return calloc(nmemb, size);
}

void *sp_malloc(size_t size) {
  return malloc(size);
}
EOF
cat > spalloc.h << EOF
#ifndef __SPALLOC_H__
#define __SPALLOC_H__ 1

#include <stdint.h>


void sp_seed(uint32_t seed);
uint32_t sp_rand();
void *sp_realloc(void *chunk, uint64_t size);
void sp_free(void *chunk);
void *sp_calloc(size_t nmemb, size_t size);
void *sp_malloc(uint64_t size);


#endif //__SPALLOC_H__
EOF

hasFile "*.o" && error "$FILE: Includes .o file"
cc -I. -o driver ../driver.c spalloc.c -lm || error "Compiling driver.c failed"
cd ..

HASBUGS=NO
for f in 1 2 3 4 5 6 7 8 9; do
  if [ -e bug$f ]; then
    if [ -d bug$f ]; then
      HASBUGS=YES
      if [ ! -f bug$f/input.txt ]; then
	warning No input.txt in bug$f
      fi
    else
      warning bug$f is not a directory
    fi
  fi
done

if [ "$HASBUGS" = "NO" ]; then
  warning No bug directories
fi

setdisp lgreen; echo "Soundness test passed, but check for warnings"; setdisp reset
