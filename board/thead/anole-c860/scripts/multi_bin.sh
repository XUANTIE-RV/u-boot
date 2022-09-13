#!/bin/bash

OUT_IMG=$1
BIN_1=$2
BIN_COUNT=1
[ x"$3" = x"" ] || { BIN_2=$3; BIN_COUNT=2; }
[ x"$4" = x"" ] || { BIN_3=$4; BIN_COUNT=3; }
[ x"$5" = x"" ] || { BIN_4=$5; BIN_COUNT=4; }
[ x"$6" = x"" ] || { BIN_5=$6; BIN_COUNT=5; }
[ x"$7" = x"" ] || { BIN_6=$7; BIN_COUNT=6; }
[ x"$8" = x"" ] || { BIN_7=$8; BIN_COUNT=7; }
[ x"$9" = x"" ] || { BIN_8=$9; BIN_COUNT=8; }

BLOCK_SIZE=16

get_file_length() {
    wc -c $1 | cut -d' ' -f1
}

u8_to_hex() {
    echo -e -n "\x`printf "%02x" $1`"
}

u32_to_hex() {
    u8_to_hex `expr $1 '%' 256`
    u8_to_hex `expr $1 '/' 256 '%' 256`
    u8_to_hex `expr $1 '/' 65536 '%' 256`
    u8_to_hex `expr $1 '/' 65536 '/' 256`
}

get_aligned_size() {
    expr '(' $1 '+' $2 '-' 1 ')' '/' $2 '*' $2
}

# (16 + 16 * 8 bytes) image header
# 0-7:      magic
# 8:        count
# 9-15:     reserved, must be zero
# 16-31:    bin1 info
# 32-48:    bin2 info
# ...
# 128-143:  bin8 info
echo -e -n "MBIN\x20\x20\x03\x30" > ${OUT_IMG}
echo -e -n "\x${BIN_COUNT}" >> ${OUT_IMG}
echo -e -n "\x0\x0\x0\x0\x0\x0\x0" >> ${OUT_IMG}

# append (16*8 bytes) bin infos
CUR_BIN_INFO_OFFSET=16
CUR_BIN_DATA_OFFSET=`expr 16 '+' 16 '*' 8` # (16*8 bytes) bin infos
for i in `seq ${BIN_COUNT}`; do
    eval BIN_FILE='$BIN_'$i
    if [ -f "${BIN_FILE}" ]; then
        echo "----"
        echo "BIN_FILE=${BIN_FILE}"
        length=`get_file_length ${BIN_FILE}`
        offset=${CUR_BIN_DATA_OFFSET}
        echo ">> bin[$i] info: offset($offset) length($length)"

        # (16 bytes) add info of bin
        # 0-3:  offset
        # 4-7:  length
        # 8-15: reserved, must be zero
        u32_to_hex $offset >> ${OUT_IMG}
        u32_to_hex $length >> ${OUT_IMG}
        echo -e -n "\x0\x0\x0\x0\x0\x0\x0\x0" >> ${OUT_IMG}

        aligned_length=`get_aligned_size $length ${BLOCK_SIZE}`
        eval OFFSET_$i=$offset
        eval ALIGNED_LENGTH_$i=$aligned_length

        CUR_BIN_INFO_OFFSET=`expr ${CUR_BIN_INFO_OFFSET} '+' 16`
        CUR_BIN_DATA_OFFSET=`expr ${CUR_BIN_DATA_OFFSET} '+' $aligned_length`
    fi
done

# append bin datas
for i in `seq ${BIN_COUNT}`; do
    eval BIN_FILE='$BIN_'$i
    if [ -f "${BIN_FILE}" ]; then
        echo "----"
        echo "BIN_FILE=${BIN_FILE}"
        eval offset='$OFFSET_'$i
        eval aligned_length='$ALIGNED_LENGTH_'$i

        # add data of bin
        echo ">> append bin[$i] offset($offset) aligned_length($aligned_length)"
        seek=`expr $offset '/' ${BLOCK_SIZE}`
        dd if=${BIN_FILE} of=${OUT_IMG} bs=${BLOCK_SIZE} seek=$seek
    fi
done

echo "make image finish!"
