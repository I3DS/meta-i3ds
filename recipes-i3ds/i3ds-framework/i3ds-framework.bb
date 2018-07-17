DESCRIPTION = "I3DS-framework"
LICENSE = "CLOSED"

DEPENDS = "zeromq boost cppzmq"


inherit pkgconfig cmake

EXTRA_OECMAKE += '-DBUILD_EMBEDDED=OFF -DBUILD_TOOLS=ON -DBUILD_TESTS=OFF -DGENERATE_ASN=OFF '

SRCREV = "${AUTOREV}"
PV = "1.0+gitr${SRCPV}"
SRC_URI = "git://git.code.sintef.no/i3ds/i3ds-framework-cpp.git;protocol=ssh;user=git;branch=master"


S = "${WORKDIR}/git"

BBCLASSEXTEND = "native"


