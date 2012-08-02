#!/bin/sh

#if you add lib or bin in this file, you must point out two points below
#first:
#please confirm this lib or bin is relay by make recoveryimage in build/core/Makefile such as #$(toolbox_binary) \ 你必须在buil/core/Makefile 里面增加依赖关系
#second:
#you copy path must is the place like below


OUT_DIR=$1
RECOVERY_DIR=${OUT_DIR}/recovery
TARGET_DIR=${RECOVERY_DIR}/root
OUT_DIR_LIB=${OUT_DIR}/obj/SHARED_LIBRARIES
OUT_DIR_BIN=${OUT_DIR}/obj/EXECUTABLES

mkdir -p ${TARGET_DIR}/lib
mkdir -p ${TARGET_DIR}/sbin
mkdir -p ${TARGET_DIR}/system/bin
mkdir -p ${TARGET_DIR}/system/lib

cp -r ${OUT_DIR_LIB}/libc_intermediates/LINKED/libc.so ${TARGET_DIR}/lib/
cp -r ${OUT_DIR_LIB}/libcutils_intermediates/LINKED/libcutils.so ${TARGET_DIR}/lib/
cp -r ${OUT_DIR_LIB}/libdl_intermediates/LINKED/libdl.so ${TARGET_DIR}/lib/
cp -r ${OUT_DIR_LIB}/liblog_intermediates/LINKED/liblog.so ${TARGET_DIR}/lib/
cp -r ${OUT_DIR_LIB}/libm_intermediates/LINKED/libm.so ${TARGET_DIR}/lib/
cp -r ${OUT_DIR_LIB}/libstdc++_intermediates/LINKED/libstdc++.so ${TARGET_DIR}/lib/


cp -r ${OUT_DIR_BIN}/linker_intermediates/linker ${TARGET_DIR}/system/bin/
cp -r ${OUT_DIR_BIN}/logcat_intermediates/logcat ${TARGET_DIR}/system/bin/
cp -r ${OUT_DIR_BIN}/sh_intermediates/sh ${TARGET_DIR}/system/bin/

cp ${OUT_DIR_BIN}/toolbox_intermediates/toolbox ${TARGET_DIR}/system/bin/
ln -sf toolbox ${TARGET_DIR}/system/bin/mkdosfs

cp -r ${OUT_DIR}/system/bin/busybox ${TARGET_DIR}/sbin/
ln -sf busybox ${TARGET_DIR}/sbin/ls
ln -sf busybox ${TARGET_DIR}/sbin/cat
ln -sf busybox ${TARGET_DIR}/sbin/chmod
ln -sf busybox ${TARGET_DIR}/sbin/chown
ln -sf busybox ${TARGET_DIR}/sbin/date
ln -sf busybox ${TARGET_DIR}/sbin/dd
ln -sf busybox ${TARGET_DIR}/sbin/df
ln -sf busybox ${TARGET_DIR}/sbin/dmesg
ln -sf busybox ${TARGET_DIR}/sbin/ifconfig
ln -sf busybox ${TARGET_DIR}/sbin/kill
ln -sf busybox ${TARGET_DIR}/sbin/mkdir
ln -sf busybox ${TARGET_DIR}/sbin/mount
ln -sf busybox ${TARGET_DIR}/sbin/reboot
ln -sf busybox ${TARGET_DIR}/sbin/rm
ln -sf busybox ${TARGET_DIR}/sbin/umount
ln -sf busybox ${TARGET_DIR}/sbin/insmod
ln -sf busybox ${TARGET_DIR}/sbin/lsmod
ln -sf busybox ${TARGET_DIR}/sbin/rmmod
ln -sf busybox ${TARGET_DIR}/sbin/mv
ln -sf busybox ${TARGET_DIR}/sbin/ps
ln -sf busybox ${TARGET_DIR}/sbin/cp

chmod 777 $TARGET_DIR -R

echo " ==>end "

