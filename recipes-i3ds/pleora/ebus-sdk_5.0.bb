# Pleora eBUS SDK for GigE Vision cameras

SUMMARY = "Pleora eBUS SDK 5.0"
SECTION = "PETALINUX/apps"
LICENSE = "Proprietary"
LIC_FILES_CHKSUM = "file://share/doc/third_party_licenses/NOTICE.txt;md5=b7b40bf71d2647bfc5f2d6eb61f84c82"

SRC_URI = "file://${PN}-${PV}.tar.gz"

S = "${WORKDIR}/ebus-sdk"

EBUS_LIB = "${libdir}/ebus"
EBUS_INC = "${includedir}/ebus"

# Flags to avoid QA error because of unversioned so symlinks
INSANE_SKIP_${PN} = "dev-so"

do_install () {
	# Install needed libraries
	install -d ${D}${EBUS_LIB}
	cp -ra --no-preserve=ownership -t ${D}${EBUS_LIB} ${S}/lib/*

	# Install needed headers
	install -d ${D}${EBUS_INC}
	cp -ra --no-preserve=ownership -t ${D}${EBUS_INC} ${S}/include/*
}

FILES_${PN} += " ${EBUS_LIB}/*"
