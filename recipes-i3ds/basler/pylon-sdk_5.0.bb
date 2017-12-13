# Pylon5 SDK for Basler cameras

SUMMARY = "Basler Pylon5 SDK"
SECTION = "PETALINUX/apps"
LICENSE = "Proprietary"
LIC_FILES_CHKSUM = "file://share/pylon/License.html;md5=b966523debb4b5dc8429530d00d2e8be"

SRC_URI = "file://${PN}-${PV}.tar.gz"

S = "${WORKDIR}/pylon5"

PYLON_LIB = "${libdir}/pylon5"
PYLON_INC = "${includedir}/pylon5"

# Flags to avoid QA error because of stripped binaries
INSANE_SKIP_${PN} = "ldflags"
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
INHIBIT_PACKAGE_STRIP = "1"

# Flags to avoid QA error because of unversioned so symlinks
INSANE_SKIP_${PN} = "dev-so"

do_install () {
	# Install needed libraries
	install -d ${D}/${PYLON_LIB}
	cp -ra --no-preserve=ownership -t ${D}/${PYLON_LIB} ${S}/lib64/*

	# Install needed headers
	install -d ${D}/${PYLON_INC}
	cp -ra --no-preserve=ownership -t ${D}/${PYLON_INC} ${S}/include/*
}

FILES_${PN} += " ${PYLON_LIB}/*"
