#!/bin/sh

#sh dh8000_mkimg.sh mboot/uboot/kernel/kernel_s/rootfs/rootfs_s/all snor/snand/pnand/emmc/ flashconfig fs

#fixed config
vendor=dhc
cputype=dh8000
bindir=./bin
flasht=emmc
flashc=emmc
fstype=jffs2
verify=crc

specdir=${vendor}/${cputype}
#shroot=$(pwd)
shroot=.

cfgdir=${shroot}/config/${specdir}/${flasht}
inputdir=${shroot}/input/${specdir}/${flasht}
outdir=${shroot}/output/${specdir}/${flasht}
scproot="isp@10.35.180.1:/tftpboot/anole"
incdir=./config/dhc/dh8000/include/

keydir=${shroot}/key/anole
keyname=dhc_anole 

mkbimg=$bindir/mkimg
mkuimg=$bindir/mkuimage
mkjffs=$bindir/mkfs.jffs2
mkbimgSign=$bindir/mkimgSign

if [ "${release}" != "" ]; then 
scpen=y; \
scpdir=${scproot}/${release}/${flasht}/${verify}; \
echo ${scpdir}
else 
echo "release is null!"; \
scpen=n
fi

mkdir -p ${outdir}/${flasht}/

#compiler the config
${CROSS_CC_COMPILER}gcc -I${incdir} -c ${cfgdir}/${flashc}_cfg.c -o ${cfgdir}/${flashc}_cfg.o
${CROSS_CC_COMPILER}objcopy -O binary ${cfgdir}/${flashc}_cfg.o ${cfgdir}/${flashc}.cfg
rm -f ${cfgdir}/${flashc}_cfg.o

#${inputdir}/rootfs_dh8000 elf ${keydir} ${keyname}
function getDirVerify()
{
	for file in $1/*
	do 
		keydir=$3
		keyname=$4
		if [ -d $file ]
		then
			getDirVerify $file elf $keydir $keyname
		else 
			echo $file
			${mkbimgSign} $file $file elf $keydir $keyname
		fi
	done
}

#gen mboot
function img_mboot()
{
	echo "gen mboot"
	para="v3 ${verify} ${cfgdir}/${flashc}.cfg ${outdir}/mboot.fimg ${inputdir}/dh8000_mboot.bin ${inputdir}/u-boot.bin 0 ${flasht}"
	echo "miniboot para = ${para}"
	${mkbimg} ${para}
	if [ "$scpen" = "y" ]; then
	scp ${outdir}/mboot.fimg ${scpdir}
	fi
}


#gen uboot
function img_uboot()
{
	echo "gen uboot"
	if [ "$verify" = "crc" ]; then
		para="-A sag -O u-boot -T kernel -C none -a 0x100000 -e 0x00800000 -n u-boot-${flasht} -d ${inputdir}/u-boot.bin ${outdir}/u-boot.uimg"
		echo "uboot para = ${para}"
		${mkuimg} ${para}
	else
		para="${inputdir}/u-boot.bin ${outdir}/u-boot.uimg uboot ${keydir} ${keyname}"
		echo "uboot para = ${para}"
		${mkbimgSign} ${para}
	fi
	
	if [ "$scpen" = "y" ]; then
	scp ${outdir}/${flasht}/u-boot.uimg ${scpdir}
	fi
}
#gen linux_host
function img_kernel()
{
	echo "gen linux_host"
	para="-A sag -O linux -T kernel -C none -a 0x80000000 -e 0x80000000 -n linux-${flasht} -d ${inputdir}/linux.img ${outdir}/linux.uimg"
	echo "linux host para = ${para}"
	${mkuimg} ${para}
	
	if [ "$verify" = "sign" ]; then
		para="${outdir}/${flasht}/linux.uimg ${outdir}/linux.uimg kernel ${keydir} ${keyname}"
		echo "linux host para = ${para}"
		${mkbimgSign} ${para}
	fi
	
	if [ "$scpen" = "y" ]; then
	scp ${outdir}/linux.uimg ${scpdir}
	fi
}
#gen linux_slaver
function img_skernel()
{
	echo "gen linux_slaver"
	para="-A sag -O linux -T kernel -C none -a 0x82000000 -e 0x82000000 -n linux-${flasht} -d ${inputdir}/${flasht}/linux_slaver.img ${outdir}/${flasht}/linux_slaver.uimg"
	echo "linux slaver para = ${para}"
	${mkuimg} ${para}
	if [ "$scpen" = "y" ]; then
	scp ${outdir}/${flasht}/linux_slaver.uimg ${scpdir}
	fi
}
#gen rootfs
function img_rootfs
{
	echo "gen rootfs_host; format jffs2"
	
	if [ "$verify" = "sign" ]; then
		para="${inputdir}/rootfs_dh8000 elf ${keydir} ${keyname}"
		getDirVerify ${para}
	fi
	
	para="-r ${inputdir}/rootfs_dh8000/ -o ${outdir}/rootfs_jffs2.fimg -p 0x300000 -e 0x2000 -s 256 -n"
	echo "mkfs.jffs2 para = ${para}"
	${mkjffs} ${para}
	
	if [ "$scpen" = "y" ]; then
	scp ${outdir}/rootfs_jffs2.fimg ${scpdir}
	fi
}

function img_all
{
	img_mboot
	img_uboot
	img_kernel
	img_skernel
	img_rootfs
}

#call function
img_${1}
