THISDIR=`dirname $0`
HOST_DIR=.
TOOLDIR=${HOST_DIR}/opt/dh_mkimgtool
SPL_BIN=$1
UBOOT_BIN=$2
OUT_DIR=$3

echo "prepare scripts ..."
mkdir -p ${TOOLDIR}/scripts/dhc/dh8000/emmc/
cp bin ${TOOLDIR} -r
cp ${THISDIR}/mkimg_emmc.sh ${TOOLDIR}/scripts/dhc/dh8000/emmc

echo "prepare config files ..."
mkdir -p ${TOOLDIR}/config/dhc/dh8000/emmc/
cp ${THISDIR}/emmc_cfg.c ${TOOLDIR}/config/dhc/dh8000/emmc

echo "prepare input files ..."
mkdir -p ${TOOLDIR}/input/dhc/dh8000/emmc/
cp ${SPL_BIN} ${TOOLDIR}/input/dhc/dh8000/emmc/dh8000_mboot.bin
cp ${UBOOT_BIN} ${TOOLDIR}/input/dhc/dh8000/emmc/u-boot.bin

echo "make image for mboot ..."
mkdir -p ${TOOLDIR}/output/dhc/dh8000/emmc/
cd ${TOOLDIR}
bash ./scripts/dhc/dh8000/emmc/mkimg_emmc.sh mboot
cd -

echo "copy mboot image to ${OUT_DIR}/mboot.fimg ..."
cp ${TOOLDIR}/output/dhc/dh8000/emmc/mboot.fimg ${OUT_DIR}/mboot.fimg
