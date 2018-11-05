DESCRIPTION = "I3DS firmware"
LICENSE = "CLOSED"

INSANE_SKIP_${PN} = "arch"

OPENAMP_FW_NAME = "i3ds_imu_r5.elf"

SRC_URI = "file://i3ds_imu_r5.elf \
	"

FW_LIB_DIR = "/lib/firmware"

S = "${WORKDIR}"

do_install() {
	install -d ${D}${FW_LIB_DIR}
	src=${OPENAMP_FW_NAME}
	dest=${D}${FW_LIB_DIR}/${OPENAMP_FW_NAME}
	install -m 0644 ${src} ${dest}
}

FILES_${PN} = "${FW_LIB_DIR}/${OPENAMP_FW_NAME}"

