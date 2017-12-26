# We know about rtc
MACHINE_FEATURES_BACKFILL_CONSIDERED = "rtc" 

MACHINE_KERNEL_PR = "r0"

MACHINE_ESSENTIAL_EXTRA_RDEPENDS = "\
	amiko-dvb-modules-${MACHINE} \
	"

MACHINE_ESSENTIAL_EXTRA_RRECOMMENDS = "\
	kernel-module-cdfs \
	"

KERNEL_MODULE_AUTOLOAD = "xfs"

MACHINE_EXTRA_RRECOMMENDS = " \
	gst-plugin-dvbmediasink \
	ntfs-3g \
	"

DVBMEDIASINK_CONFIG = "--with-wma --with-wmv --with-pcm --with-dts --with-eac3 \
"

TARGET_ARCH = "mipsel"
DEFAULTTUNE ?= "mips32el"

PREFERRED_PROVIDER_virtual/kernel = "linux-amiko"

KERNEL_IMAGETYPE = "vmlinux"
KERNEL_OUTPUT = "${KERNEL_IMAGETYPE}"
KERNEL_CONSOLE = "null"

IMAGE_FSTYPES ?= "ubi"

UBI_VOLNAME = "rootfs"

IMAGEDIR ?= "\
${@bb.utils.contains('MACHINE', 'vipert2c', 'vipert2c' , '', d)}\
${@bb.utils.contains('MACHINE', 'vipercombo', 'vipercombo' , '', d)}\
${@bb.utils.contains('MACHINE', 'vipercombohdd', 'vipercombohdd' , '', d)}\
"

IMGDEPLOYDIR ?= "${DEPLOY_DIR_IMAGE}"
IMAGEVERSION := "${DISTRO_NAME}-${DISTRO_VERSION}-${DATE}"
IMAGEVERSION[vardepsexclude] = "DATE"

IMAGE_CMD_ubi_append = " \
	mkdir -p ${IMGDEPLOYDIR}/${IMAGEDIR}; \
	cp ${IMGDEPLOYDIR}/${IMAGE_NAME}.rootfs.ubi ${IMGDEPLOYDIR}/${IMAGEDIR}/rootfs.bin; \
	gzip -9c ${DEPLOY_DIR_IMAGE}/vmlinux-${MACHINE}.bin > ${IMGDEPLOYDIR}/${IMAGEDIR}/kernel.bin; \
	echo "${IMAGEVERSION}" > ${IMGDEPLOYDIR}/${IMAGEDIR}/imageversion; \
    ${@bb.utils.contains('MACHINE', 'vipert2c', \
    'echo "dont change this file" > ${IMGDEPLOYDIR}/${IMAGEDIR}/force;' , \
    'echo "rename this file to force to force an update without confirmation" > ${IMGDEPLOYDIR}/${IMAGEDIR}/noforce;', d)} \
	cd ${IMGDEPLOYDIR}; \
	zip ${DISTRO_NAME}-${DISTRO_VERSION}-${MACHINE}_usb.zip ${IMAGEDIR}/*; \
	rm -Rf ${IMAGEDIR}; \
	"

IMAGE_CMD_ubi_prepend = " \
	rm -Rf ${IMAGE_ROOTFS}/tmp/*; \
	"

MACHINE_FEATURES += "alsa usbhost wlan 3dtv switchoff osdposition hdmicec"

require conf/machine/include/tune-mips32.inc
require conf/machine/include/amiko-wifi.inc
