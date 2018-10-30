# Pleora eBUS SDK for GigE Vision cameras

SUMMARY = "Pleora eBUS SDK 5.0"
SECTION = "PETALINUX/apps"
LICENSE = "CLOSED"

SRC_URI = "file://${PN}-${PV}.tar.gz"

S = "${WORKDIR}/ebus-sdk"

EBUS_BIN = "${bindir}/ebus"
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

	# Install needed binaries
	install -d ${D}${EBUS_BIN}
	cp -ra --no-preserve=ownership -t ${D}${EBUS_BIN} ${S}/bin/*
}

FILES_${PN} += " ${EBUS_LIB}/*"
