#
# Copyright (c) 2026 Gaël Fortier <gael.fortier.1@ens.etsmtl.ca>
#
#
# Copyright (c) 2026 Gaël Fortier <gael.fortier.1@ens.etsmtl.ca>
#

cc="gcc"
if [ $# -eq 1 ]; then cc=$1; fi

target="$($cc -print-multiarch)"
if [ $? -ne 0 ]; then exit 1; fi

name=$(basename $(pwd))
files="src/cli.c"
output="out/$name-$target.o"
ccargs="-r -ggdb -static"
command="$cc $files -o $output $ccargs"

echo "[ build ] Compiling $name"
echo "[ build ] ==> $command"
$command
if [ $? -ne 0 ]; then exit 5; fi
if [ ! -f $output ]; then
  echo "$output file is missing."
  exit 6
fi

echo "[ build ] ==> Generated $output"
exit 0
