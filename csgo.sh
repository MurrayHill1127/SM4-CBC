#!/bin/sh
#   csgo:
#   CBC-SM4, go!

IV="ffffffffffffffffffffffffffffffff"
KEY="ffffffffffffffffffffffffffffffff"

BINDIR="./bin"

in_file="in.txt"
out_file="out.txt"
back_file="back.txt"

mode1="encrypt"
mode2="decrypt"

make

cd $BINDIR
./sm4-cbc $mode1 $in_file $out_file $IV $KEY
./sm4-cbc $mode2 $out_file $back_file $IV $KEY


#gdb --args ./sm4-cbc $mode $in_file $out_file $IV $KEY
