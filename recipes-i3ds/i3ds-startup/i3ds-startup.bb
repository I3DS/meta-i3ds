#
# This file is the i3ds-startup recipe.
#

SUMMARY = "Simple i3ds-startup application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://i3ds-startup \
	   file://restarting.sh \
	   file://peta_addresses.csv \
	   file://ebusd \
	   file://i3ds-r5-startup \
	  "

S = "${WORKDIR}"

INSANE_SKIP_${PN} = "arch"
FW_LIB_DIR = "/lib/firmware"

do_install() {
     install -d ${D}/etc/
     install -m 0644 peta_addresses.csv ${D}/etc/

     install -d ${D}/usr/bin/
     install -m 0755 restarting.sh ${D}/usr/bin/

     install -d ${D}/etc/init.d/
     install -m 0755 i3ds-startup ${D}/etc/init.d/
     install -m 0755 i3ds-r5-startup ${D}/etc/init.d/
     install -m 0755 ebusd ${D}/etc/init.d/

     install -d ${D}/etc/rc5.d/
     ln -r -s ${D}/etc/init.d/i3ds-startup ${D}/etc/rc5.d/S99i3ds-startup
     ln -r -s ${D}/etc/init.d/i3ds-r5-startup ${D}/etc/rc5.d/S99i3ds-r5-startup
     ln -r -s ${D}/etc/init.d/ebusd ${D}/etc/rc5.d/S70ebusd
}
