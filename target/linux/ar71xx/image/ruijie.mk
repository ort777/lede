define Device/ruijie_rg-est310
  SOC := qca9531
  DEVICE_VENDOR := Ruijie
  DEVICE_MODEL := RG-EST310
  DEVICE_PACKAGES := kmod-usb2 kmod-ath10k-ct ath10k-firmware-qca9888-ct \
	-uboot-envtools -swconfig
  IMAGE_SIZE := 16192k
endef
TARGET_DEVICES += ruijie_rg-est310
