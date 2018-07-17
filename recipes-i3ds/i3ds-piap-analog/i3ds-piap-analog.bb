DESCRIPTION = "I3DS-piap-analog ADC driver"
LICENSE = "CLOSED"

DEPENDS = "i3ds-framework"

inherit pkgconfig cmake

SRCREV = "${AUTOREV}"
PV = "1.0+gitr${SRCPV}"
SRC_URI = "git://git.code.sintef.no/i3ds/i3ds-piap-analog.git;protocol=ssh;user=git;branch=master"

S = "${WORKDIR}/git"

BBCLASSEXTEND = "native"
