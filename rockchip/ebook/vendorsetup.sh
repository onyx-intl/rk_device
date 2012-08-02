
# make rockchip firmware
function rkmkimage()
{
    T=$(gettop)
    if [ ! "$T" ]; then
        echo "Couldn't locate the top of the tree.  Try setting TOP." >&2
        return
    fi
    mkdir -p  $T/rockdev/Image
    setpaths
    mkbootfs $OUT/root | minigzip > $OUT/ramdisk.img && device/rockchip/newton/mkkrnlimg $OUT/ramdisk.img rockdev/Image/boot.img
    chmod -R 777 $OUT/system		
    mkfs.cramfs $OUT/system rockdev/Image/system.img
}
