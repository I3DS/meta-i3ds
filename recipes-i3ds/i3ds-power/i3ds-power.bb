DESCRIPTION = "I3DS-power driver"
LICENSE = "CLOSED"

DEPENDS = "i3ds-framework"

inherit pkgconfig cmake

EXTRA_OECMAKE += '-DBUILD_EMBEDDED=OFF '

SRCREV = "${AUTOREV}"
PV = "1.0+gitr${SRCPV}"
SRC_URI = "git://git.code.sintef.no/i3ds/i3ds-power-control.git;protocol=ssh;user=git;branch=master"

S = "${WORKDIR}/git"

BBCLASSEXTEND = "native"
