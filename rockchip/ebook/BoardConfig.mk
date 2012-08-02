#
# Product-specific compile-time definitions.
#

TARGET_BOARD_PLATFORM := rk28
TARGET_NO_BOOTLOADER := true
# IMPORTANT: 
# For RK platform, we always use our own RK kernel image, not the 
# standard linux kernel image. Therefore, TARGET_NO_KERNEL should 
# always be set to false (other than the recovery will not be 
# built any more) and TARGET_USE_RK_KERNEL must be set to true.
# Also you should notice that with this configuration the recovery
# image built by android will be shipped with RK kernel image 
# which should have been standard kernel image (hence the recovery
# image is unusable on any platform). Instead, you should always 
# use the shell script which can be found under android source 
# tree root, namely 'mk_flash_dev' to bulid the usable recovery 
# image on RK platform.
TARGET_NO_KERNEL := false
TARGET_USE_RK_KERNEL := true
TARGET_CPU_ABI := armeabi
TARGET_BOOTLOADER_BOARD_NAME := ebook
TARGET_PROVIDES_INIT_RC := true

BOARD_USES_GENERIC_AUDIO := false
TARGET_HARDWARE_3D := false
USE_CAMERA_STUB := true

#for wifi
BOARD_WPA_SUPPLICANT_DRIVER := WEXT
BOARD_WLAN_DEVICE           := bcm4329
WPA_SUPPLICANT_VERSION      := VER_0_6_X

#build with bluetooth
BOARD_HAVE_BLUETOOTH := false

JS_ENGINE = v8