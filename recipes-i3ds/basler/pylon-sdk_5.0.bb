# Recipe for Basler Pylon5 SDK

SUMMARY = "Basler Pylon5 SDK"
SECTION = "PETALINUX/apps"
LICENSE = "Proprietary"
LIC_FILES_CHKSUM = "file://share/pylon/License.html;md5=b966523debb4b5dc8429530d00d2e8be"

SRC_URI = "file://${PN}-${PV}.tar.gz"

S = "${WORKDIR}/pylon5"

# Flags to avoid QA error because of stripped binaries
INSANE_SKIP_${PN} = "ldflags"
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
INHIBIT_PACKAGE_STRIP = "1"

do_install () {
	# Install needed libraries under /usr/lib/pylon5
	install -d ${D}/usr/lib/pylon5
	install ${S}/lib64/libpylonbase-5.0.11.so ${D}/usr/lib/pylon5/
	install ${S}/lib64/libpylonutility-5.0.11.so ${D}/usr/lib/pylon5/
	install ${S}/lib64/libpylonc-5.0.11.so ${D}/usr/lib/pylon5/
	install ${S}/lib64/libLog_gcc_v3_0_Basler_pylon_v5_0.so ${D}/usr/lib/pylon5/
	install ${S}/lib64/libGenApi_gcc_v3_0_Basler_pylon_v5_0.so ${D}/usr/lib/pylon5/
	install ${S}/lib64/libXmlParser_gcc_v3_0_Basler_pylon_v5_0.so ${D}/usr/lib/pylon5/
	install ${S}/lib64/libFirmwareUpdate_gcc_v3_0_Basler_pylon_v5_0.so ${D}/usr/lib/pylon5/
	install ${S}/lib64/libGCBase_gcc_v3_0_Basler_pylon_v5_0.so ${D}/usr/lib/pylon5/
	install ${S}/lib64/liblog4cpp_gcc_v3_0_Basler_pylon_v5_0.so ${D}/usr/lib/pylon5/
	install ${S}/lib64/libLog_gcc_v3_0_Basler_pylon_v5_0.so ${D}/usr/lib/pylon5/
	install ${S}/lib64/libMathParser_gcc_v3_0_Basler_pylon_v5_0.so ${D}/usr/lib/pylon5/
	install ${S}/lib64/libNodeMapData_gcc_v3_0_Basler_pylon_v5_0.so ${D}/usr/lib/pylon5/
}

FILES_{$PN} += " /usr/lib/pylon5/*"
