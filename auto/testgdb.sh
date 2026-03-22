#
# Copyright (c) 2026 Gaël Fortier <gael.fortier.1@ens.etsmtl.ca>
#

if [ $# -ne 1 ]; then
  echo "syntax: ./auto/test.sh <name of module>"
  exit 1
fi

file=$1

if [ ! -d "src" ]; then
  echo "Source folder missing."
  exit 1
fi

if [ ! -d "test" ]; then
  echo "Test folder missing."
  exit 2
fi

if [ ! -d "test/$file" ]; then
  echo "Module $file test folder is missing."
  exit 3
fi

if [ ! -f "test/$file/$file.c" ]; then
  echo "Module $file test source is missing."
  exit 4
fi

if [ ! -f "test/$file/requires.txt" ]; then
  echo "Module $file test requirement (requires.txt) file is missing."
  exit 5
fi

sourcelist=""
requiresfile=$(cat "test/$file/requires.txt")
for f in ${requiresfile[@]}; do
  if [ ! -f "src/$f.c" ]; then
    echo "Module $f in requirements is missing."
    exit 6
  fi
  sourcelist="$sourcelist src/$f.c"
done

cmd="gcc "test/$file/$file.c" $sourcelist  -o "test/$file/$file.elf" -ggdb -static"
echo $cmd
$cmd
if [ $? -ne 0 ]; then
  exit 7
fi

chmod +x  "test/$file/$file.elf"
gdb test/$file/$file.elf

