#!/bin/sh

# Names for the library and the compiler driver

COMP=./acc



if [ "$1" = '' ]

then

echo "Usage: $0 basename output_name" 1>&2

echo "Will compile basename to output_name via basename.s" 1>&2

exit 1

fi

$COMP <$1 >$1.s
rm $1.out
cc -o $2 $1.s 

