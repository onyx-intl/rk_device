#
# Copyright (C) 2008 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

DEVICE_PACKAGE_OVERLAYS := device/htc/dream/overlay

PRODUCT_PACKAGES := \
    sensors.trout

# proprietary side of the device
$(call inherit-product-if-exists, vendor/htc/dream/device_dream-vendor.mk)

# bits that are shared between dream and sapphire
$(call inherit-product, device/htc/dream-sapphire/device_dream_sapphire.mk)

# stuff common to all HTC phones
$(call inherit-product, device/htc/common/common_small.mk)

