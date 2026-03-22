#
# Copyright (c) 2026 Gaël Fortier <gael.fortier.1@ens.etsmtl.ca>
#

if [ $# -ne 1 ]; then
  echo "syntax: ./auto/remove.sh <name of module>"
  exit 1
fi

file=$1

if [ ! -d "src" ]; then
  echo "Source folder missing."
  exit 1
fi

if [ ! -f "src/$file.h" ]; then
  echo "Header of module $file missing."
  exit 2
fi

if [ ! -f "src/$file.c" ]; then
  echo "Source of module $file missing."
  exit 3
fi

if [ ! -d "test" ]; then
  echo "Test folder missing."
  exit 4
fi

if [ ! -d "test/$file" ]; then
  echo "Module $file test folder is missing."
  exit 5
fi

if [ ! -f "test/$file/$file.c" ]; then
  echo "Module $file test source is missing."
  exit 6
fi

rm "src/$file.h"
rm "src/$file.c"
rm -rf test/$file
