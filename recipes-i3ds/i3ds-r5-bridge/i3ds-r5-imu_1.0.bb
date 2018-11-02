SUMMARY = "I3DS R5 bridge for IMU"

LICENSE = "BSD"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b30cbe0b980e98bfd9759b1e6ba3d107"

SRC_URI = "\
	file://LICENSE \
	file://Makefile \
	file://i3ds_r5_imu.c \
	"

S = "${WORKDIR}"

DEPENDS = "rpmsg-user-module"

FILES_${PN} = "\
	/usr/bin/i3ds_r5_imu \
"

do_install () {
	install -d ${D}/usr/bin
	install -m 0755 i3ds_r5_imu ${D}/usr/bin/i3ds_r5_imu
}
