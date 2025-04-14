#!/bin/sh
#   csgo:
#   CBC-SM4, go!

IV="ffffffffffffffffffffffffffffffff"
KEY="ffffffffffffffffffffffffffffffff"

in_file="in.txt"
out_file="out.txt"
mode="encrypt"

make
#gdb --args ./sm4-cbc $mode $in_file $out_file $IV $KEY
./sm4-cbc $mode $in_file $out_file $IV $KEY
