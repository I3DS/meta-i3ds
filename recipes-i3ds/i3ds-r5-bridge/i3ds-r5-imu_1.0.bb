SUMMARY = "I3DS R5 bridge for IMU"

LICENSE = "BSD"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b30cbe0b980e98bfd9759b1e6ba3d107"

DEPENDS = "rpmsg-user-module\
	   i3ds-framework"

EXTRA_OECMAKE += '-DBUILD_EMBEDDED=OFF -DGENERATE_ASN=OFF'

inherit pkgconfig cmake

SRC_URI = "\
	file://LICENSE \
	file://CMakeLists.txt \
	file://include/remoteproc_imu.hpp \
	file://include/remoteproc_bridge.hpp \
	file://src/i3ds_r5_imu.cpp \
	file://src/remoteproc_imu.cpp \
	file://src/remoteproc_bridge.cpp \
	"

S = "${WORKDIR}"

BBCLASSEXTEND = "native"
