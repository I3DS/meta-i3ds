DESCRIPTION = "I3DS driver for the Basler camera"
LICENSE = "CLOSED"

DEPENDS = "i3ds-framework pylon-sdk"

inherit pkgconfig cmake

EXTRA_OECMAKE += '-DBUILD_PETALINUX=ON '

SRCREV = "${AUTOREV}"
PV = "1.0+gitr${SRCPV}"
SRC_URI = "git://git.code.sintef.no/i3ds/i3ds-basler.git;protocol=ssh;user=git;branch=master"

S = "${WORKDIR}/git"

BBCLASSEXTEND = "native"
