#
# Copyright (c) 2026 Gaël Fortier <gael.fortier.1@ens.etsmtl.ca>
#

if [ $# -ne 1 ]; then
  echo "syntax: ./auto/create.sh <name of module>"
  exit 1
fi

file=$1

if [ ! -d "src" ]; then
  mkdir "src"
fi

if [[ -f "src/$file.h" || -f "src/$file.c" ]]; then
  echo "Module $file exists in /src."
  exit 2
fi

if [ ! -d "test" ]; then
  mkdir "test"
fi

if [[ -f "test/$file" ]]; then
  echo "Module $file exists in /test."
  exit 3
fi

if [[ -f "test/$file/$file.c" ]]; then
  echo "Module $file exists in /test."
  exit 4
fi

mkdir test/$file
srctxt=$(cat auto/data/source-template.txt | sed "s/#file#/${file}.h/")
testtxt=$(cat auto/data/source-template.txt | sed "s/#file#/\.\.\/\.\.\/src\/${file}.h/")

echo "$(cat auto/data/header-template.txt)" > src/$file.h
echo "$srctxt" > src/$file.c
echo "$testtxt" > test/$file/$file.c
echo "src/$file.c" > test/$file/requires.txt
