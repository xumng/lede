include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk
 
PKG_NAME:=gobinet
PKG_RELEASE:=1
 
include $(INCLUDE_DIR)/package.mk
 
define KernelPackage/gobinet
	SUBMENU:=Other modules
	TITLE:=Support Module for gobinet
	DEPENDS:=+kmod-usb-core +kmod-usb-net
	FILES:=$(PKG_BUILD_DIR)/GobiNet.ko
	AUTOLOAD:=$(call AutoLoad,81,GobiNet)
endef
 
define KernelPackage/gobinet/description
	This is a gobinet drivers
endef
 
MAKE_OPTS:=ARCH="$(LINUX_KARCH)" \
	CROSS_COMPILE="$(TARGET_CROSS)" \
	SUBDIRS="$(PKG_BUILD_DIR)"
	
define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)/
	$(CP) -R ./src/* $(PKG_BUILD_DIR)/
endef
 
define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" $(MAKE_OPTS) modules
endef
 
$(eval $(call KernelPackage,gobinet))