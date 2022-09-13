#!/bin/sh

echo "Generating mboot image ..."
sh ./mboot.sh ../../../../spl/u-boot-spl.bin ../../../../u-boot.bin .

echo "Generating uboot_ram image ..."
if  [ -f ./dtc ]; then
	./dtc -I dts -O dtb ../../../../arch/csky/dts/anole-rgmii-c860.dts > dt.dtb
else
	dtc -I dts -O dtb ../../../../arch/csky/dts/anole-rgmii-c860.dts > dt.dtb
fi
bash ./multi_bin.sh ./uboot_ram.img  ./dt.dtb ./u-boot.bin
