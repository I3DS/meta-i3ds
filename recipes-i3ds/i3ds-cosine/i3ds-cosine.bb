DESCRIPTION = "I3DS driver for the Cosine camera"
LICENSE = "CLOSED"

DEPENDS = "i3ds-framework ebus-sdk"

inherit pkgconfig cmake

EXTRA_OECMAKE += '-DBUILD_PETALINUX=ON -DEBUS_INCLUDE_DIR="/usr/include/ebus"'

SRCREV = "${AUTOREV}"
PV = "1.0+gitr${SRCPV}"
SRC_URI = "git://git.code.sintef.no/i3ds/i3ds-cosine.git;protocol=ssh;user=git;branch=master"

S = "${WORKDIR}/git"

BBCLASSEXTEND = "native"
