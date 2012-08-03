LOCAL_PATH := $(dir $(lastword $(MAKEFILE_LIST)))

# Additional settings used in all AOSP builds
PRODUCT_PROPERTY_OVERRIDES := \
    keyguard.no_require_sim=true \
    ro.com.android.dateformat=MM-dd-yyyy

PRODUCT_PROPERTY_OVERRIDES += \
    media.stagefright.enable-player=true \
    media.stagefright.enable-meta=true   \
    media.stagefright.enable-scan=true   \
    media.stagefright.enable-http=true\
    wifi.interface=wlan0 \
    wifi.supplicant_scan_interval=15 \
    ro.rgb565-2-luma=0 \
    ro.pixel_format_l_8=0 \
    persist.sys.idle-delay=10000 \
    ro.statusbar.use-default=1 \
    ro.tether.denied=true 

PRODUCT_COPY_FILES := \
    development/data/etc/apns-conf.xml:system/etc/apns-conf.xml \
    development/data/etc/vold.conf:system/etc/vold.conf

PRODUCT_LOCALES := zh_CN zh_TW en_US en_GB fr_FR it_IT de_DE es_ES

PRODUCT_POLICY := android.policy_phone

PRODUCT_PACKAGES := \
    AccountAndSyncSettings \
    CertInstaller \
    DrmProvider \
    LatinIME \
    Settings \
    framework-res \
    ApplicationsProvider \
    DownloadProvider \
    MediaProvider \
    SettingsProvider \
    UserDictionaryProvider \
    PackageInstaller \
    DefaultContainerService \
    EinkHome \
    Gallery \
    Music \
    Explorer \
    Browser

PRODUCT_NAME := ebook
PRODUCT_BRAND := rockchip
PRODUCT_DEVICE := ebook
PRODUCT_MODEL := ebook

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/init.rc:root/init.rc \
    $(LOCAL_PATH)/busybox:system/bin/busybox \
    $(LOCAL_PATH)/shutdown:system/bin/shutdown \
    $(LOCAL_PATH)/asound.conf:system/etc/asound.conf \
    $(LOCAL_PATH)/vold.fstab:system/etc/vold.fstab \
    $(LOCAL_PATH)/mkswap:system/bin/mkswap \
    $(LOCAL_PATH)/EBookReader/libereader.so:system/lib/libereader.so \
    $(LOCAL_PATH)/EBookReader/librm_crengine.so:system/lib/librm_crengine.so \
    $(LOCAL_PATH)/EBookReader/RkEReaderPlatform.apk:system/app/RkEReaderPlatform.apk 



    
proprietary := $(LOCAL_PATH)/proprietary
#########################################################
# rockchips proprietary code: opencore lib
#########################################################
targetFile := $(shell ls $(proprietary)/opencore/libopencore)
PRODUCT_COPY_FILES += $(foreach file, $(targetFile), $(proprietary)/opencore/libopencore/$(file):system/lib/$(file))
PRODUCT_COPY_FILES += $(proprietary)/opencore/pvplayer.cfg:system/etc/pvplayer.cfg


PRODUCT_COPY_FILES +=\
	$(proprietary)/rkAudio/lib_rk_flacdec.so:system/lib/lib_rk_flacdec.so  \
	$(proprietary)/rkAudio/lib_rk_flacdec.so:obj/lib/lib_rk_flacdec.so \
        $(proprietary)/rkAudio/lib_rk_wmadec.so:system/lib/lib_rk_wmadec.so  \
        $(proprietary)/rkAudio/lib_rk_wmadec.so:obj/lib/lib_rk_wmadec.so


 
