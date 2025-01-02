PROJECT_NAME = tlc_switch

BOARD ?= TS0001
VERSION = 5

SDK_DIR := sdk
TOOLCHAIN_DIR := toolchain
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
MAKEFILES_DIR := makefiles
HELPERS_DIR := helper_scripts


# Chip configuration

TEL_CHIP := $(POJECT_DEF) -DMCU_CORE_8258=1 -DROUTER=1 -DMCU_STARTUP_8258=1

LIBS := -ldrivers_8258 -lzb_router

# Board and version defines

ifeq ($(BOARD), TS0012)
	TEL_CHIP := $(POJECT_DEF) -DMCU_CORE_8258=1 -DROUTER=1 -DMCU_STARTUP_8258=1
	LIBS := -ldrivers_8258 -lzb_router

	DEVICE_TYPE := router

	BOARD_DEF := -DBOARD=0x01

	FROM_TUYA_MANUFACTURER_ID := 4417
	FROM_TUYA_IMAGE_TYPE := 54179
	FIRMWARE_IMAGE_TYPE := 43521
endif

ifeq ($(BOARD), TS0012_END_DEVICE)
	TEL_CHIP := $(POJECT_DEF) -DMCU_CORE_8258=1 -DEND_DEVICE=1 -DMCU_STARTUP_8258=1
	LIBS := -ldrivers_8258 -lzb_ed

	DEVICE_TYPE := end_device

	BOARD_DEF := -DBOARD=0x01

	FROM_TUYA_MANUFACTURER_ID := 4417
	FROM_TUYA_IMAGE_TYPE := 54179
	FIRMWARE_IMAGE_TYPE := 43521
endif

ifeq ($(BOARD), TS0001)
	TEL_CHIP := $(POJECT_DEF) -DMCU_CORE_8258=1 -DROUTER=1 -DMCU_STARTUP_8258=1
	LIBS := -ldrivers_8258 -lzb_router

	DEVICE_TYPE := router

	BOARD_DEF := -DBOARD=0x02

	FROM_TUYA_MANUFACTURER_ID := 4107
	FROM_TUYA_IMAGE_TYPE := 524
	FIRMWARE_IMAGE_TYPE := 43522
endif

ifeq ($(BOARD), TS0002)
	TEL_CHIP := $(POJECT_DEF) -DMCU_CORE_8258=1 -DROUTER=1 -DMCU_STARTUP_8258=1
	LIBS := -ldrivers_8258 -lzb_router

	DEVICE_TYPE := router

	BOARD_DEF := -DBOARD=0x03

	FROM_TUYA_MANUFACTURER_ID := 4417
	FROM_TUYA_IMAGE_TYPE := 54179
	FIRMWARE_IMAGE_TYPE := 43523
endif

VERSION_DEF := -DSTACK_BUILD=$(VERSION)

# Make vars

PROJECT_PATH := .

BUILD_PATH :=$(PROJECT_PATH)/$(BUILD_DIR)
BIN_PATH := $(PROJECT_PATH)/$(BIN_DIR)/$(BOARD)
TC32_PATH := $(PROJECT_PATH)/$(TOOLCHAIN_DIR)/tc32/bin
SRC_PATH := $(PROJECT_PATH)/$(SRC_DIR)
SDK_PATH := $(PROJECT_PATH)/$(SDK_DIR)
HELPERS_PATH := $(PROJECT_PATH)/$(HELPERS_DIR)

CC := $(PROJECT_PATH)/$(TOOLCHAIN_DIR)/tc32/bin/tc32-elf-gcc
LD := $(PROJECT_PATH)/$(TOOLCHAIN_DIR)/tc32/bin/tc32-elf-ld

OBJ_SRCS :=
S_SRCS :=
ASM_SRCS :=
C_SRCS :=
S_UPPER_SRCS :=
O_SRCS :=
FLASH_IMAGE :=
ELFS :=
OBJS :=
LST :=
SIZEDUMMY :=
OUT_DIR :=

PYTHON := python3


# Flags

LNK_FLAGS := --gc-sections -nostartfiles

GCC_FLAGS := \
-O2 \
-ffunction-sections \
-fdata-sections \
-Wall \
-fpack-struct \
-fshort-enums \
-finline-small-functions \
-std=gnu99 \
-funsigned-char \
-fshort-wchar \
-fms-extensions \
-nostartfiles \
-nostdlib

ASM_FLAGS := \
-fomit-frame-pointer \
-fshort-enums \
-Wall \
-Wpacked \
-Wcast-align \
-fdata-sections \
-ffunction-sections \
-fno-use-cxa-atexit \
-fno-rtti \
-fno-threadsafe-statics

INCLUDE_PATHS := -I$(SRC_PATH) -I$(SRC_PATH)/includes -I$(SRC_PATH)/common  -I$(SRC_PATH)/custom_zcl\
-I$(SDK_PATH)/platform \
-I$(SDK_PATH)/platform/chip_8258 \
-I$(SDK_PATH)/proj \
-I$(SDK_PATH)/proj/common \
-I$(SDK_PATH)/zigbee/af \
-I$(SDK_PATH)/zigbee/include \
-I$(SDK_PATH)/zigbee/bdb/includes \
-I$(SDK_PATH)/zigbee/common/includes \
-I$(SDK_PATH)/zigbee/ota \
-I$(SDK_PATH)/zigbee/zbapi \
-I$(SDK_PATH)/zigbee/zbhci \
-I$(SDK_PATH)/zigbee/zcl \
-I$(SDK_PATH)/zigbee/zdo

GCC_FLAGS += $(TEL_CHIP) $(BOARD_DEF) $(VERSION_DEF)

LS_INCLUDE := -L$(SDK_PATH)/platform/lib -L$(SDK_PATH)/zigbee/lib/tc32 -L$(SDK_PATH)/proj -L$(SDK_PATH)/platform -L$(BUILD_PATH)

LS_FLAGS := $(SRC_PATH)/boot.link

# Inlude sub-makefiles

-include makefiles/src.mk
-include makefiles/platform.mk
-include makefiles/proj.mk
-include makefiles/zigbee.mk


LST_FILE := $(BUILD_PATH)/$(PROJECT_NAME)-$(BOARD).lst
BIN_FILE := $(BIN_PATH)/$(PROJECT_NAME).bin
OTA_FILE := $(BIN_PATH)/$(PROJECT_NAME).zigbee
FROM_TUYA_OTA_FILE := $(BIN_PATH)/$(PROJECT_NAME)-from_tuya.zigbee
FORCE_OTA_FILE:= $(BIN_PATH)/$(PROJECT_NAME)-forced.zigbee
ELF_FILE := $(BUILD_PATH)/$(PROJECT_NAME)-$(BOARD).elf

Z2M_INDEX_FILE := zigbee2mqtt/ota/index_$(DEVICE_TYPE).json

Z2M_FORCE_INDEX_FILE := zigbee2mqtt/ota/index_$(DEVICE_TYPE)-FORCE.json


# Building project targets


all: pre-build main-build

main-build: $(ELF_FILE) secondary-outputs

OBJ_LIST := $(OBJS) $(USER_OBJS)
# Tool invocations
$(ELF_FILE): $(OBJ_LIST)
	@echo 'Building Standard target: $@'
	@$(LD) $(LNK_FLAGS) $(LS_INCLUDE) -T$(LS_FLAGS) -o $(ELF_FILE) $(OBJ_LIST) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

$(LST_FILE): $(ELF_FILE)
	@echo 'Invoking: TC32 Create Extended Listing'
	@$(TC32_PATH)/tc32-elf-objdump -x -D -l -S  $(ELF_FILE)  > $(LST_FILE)
	@echo 'Finished building: $@'
	@echo ' '

$(BIN_FILE): $(ELF_FILE)
	@echo 'Create Flash image (binary format)'
	@echo ' '
	@$(TC32_PATH)/tc32-elf-objcopy -v -O binary $(ELF_FILE) $(BIN_FILE)
	@echo ' '
	@$(PYTHON) $(HELPERS_PATH)/tl_check_fw.py $(BIN_FILE)
	@echo 'Finished building: $@'
	@echo ' '

$(FROM_TUYA_OTA_FILE): $(BIN_FILE)
	@echo 'Create OTA image to convert from stock firmware'
	@echo ' '
	@$(PYTHON) $(HELPERS_PATH)/zb_bin_ota.py $(BIN_FILE) $(FROM_TUYA_OTA_FILE) --header_string "Telink Zigbee OTA" --manufacturer_id $(FROM_TUYA_MANUFACTURER_ID) --image_type $(FROM_TUYA_IMAGE_TYPE) --file_version 0xFFFFFFFF
	@echo ' '

$(FORCE_OTA_FILE): $(BIN_FILE)
	@echo 'Create OTA image to convert from stock firmware'
	@echo ' '
	@$(PYTHON) $(HELPERS_PATH)/zb_bin_ota.py $(BIN_FILE) $(FORCE_OTA_FILE) --header_string "Telink Zigbee OTA" --manufacturer_id 4417 --image_type $(FIRMWARE_IMAGE_TYPE) --file_version 0xFFFFFFFF
	@echo ' '

$(OTA_FILE): $(BIN_FILE)
	@echo 'Create OTA image'
	@echo ' '
	@$(PYTHON) $(HELPERS_PATH)/zigbee_ota.py $(BIN_FILE) -p $(BIN_PATH) -n $(PROJECT_NAME)
	@echo ' '

z2m_index: $(FROM_TUYA_OTA_FILE) $(OTA_FILE)
	@echo 'Updating z2m index'
	@echo ' '
	@$(eval OTA_REAL_FILE := $(shell find $(BIN_PATH) -maxdepth 1 -type f -regex ".*$(PROJECT_NAME).zigbee"))
	$(PYTHON) $(HELPERS_PATH)/make_z2m_ota_index.py $(OTA_REAL_FILE) $(Z2M_INDEX_FILE)
	@$(eval OTA_FROM_TUYA_REAL_FILE := $(shell find $(BIN_PATH) -maxdepth 1 -type f -regex ".*$(PROJECT_NAME)-from_tuya.zigbee"))
	$(PYTHON) $(HELPERS_PATH)/make_z2m_ota_index.py $(OTA_FROM_TUYA_REAL_FILE) $(Z2M_INDEX_FILE) --board $(BOARD)
	@echo ' '


z2m_index_force: $(FORCE_OTA_FILE)
	@echo 'Updating z2m force index'
	@echo ' '
	@$(eval OTA_FORCE_FILE := $(shell find $(BIN_PATH) -maxdepth 1 -type f -regex ".*$(PROJECT_NAME)-forced.zigbee"))
	$(PYTHON) $(HELPERS_PATH)/make_z2m_ota_index.py $(OTA_FORCE_FILE) $(Z2M_FORCE_INDEX_FILE) --board $(BOARD)
	@echo ' '

sizedummy: $(ELF_FILE)
	@$(PYTHON) $(HELPERS_PATH)/TlsrMemInfo.py -t $(TC32_PATH)/tc32-elf-nm $(ELF_FILE)
	@echo ' '

pre-build:
ifneq ($(SDK_FLAGS), $(wildcard $(SDK_FLAGS)))
	$(error "Please check SDK_Path")
endif
	@mkdir -p $(foreach s,$(OUT_DIR),$(BUILD_PATH)$(s))
	@mkdir -p $(BIN_PATH)
	@echo ' '


clean:
	@$(RM) $(FLASH_IMAGE) $(ELFS) $(OBJS) $(LST) sizedummy $(ELF_FILE) $(BIN_FILE) $(LST_FILE)
	@$(RM) $(BIN_PATH)/*.zigbee
	@echo 'Clean ...'
	@echo ' '


clean_z2m_index:
	@$(RM) zigbee2mqtt/ota/*



secondary-outputs: $(BIN_FILE) $(OTA_FILE) $(LST_FILE) z2m_index z2m_index_force sizedummy 



# SDK & Tools download targets

SDK_VERSION := 3.6.8.6

sdk:
	mkdir -p $(SDK_DIR)
	wget -P $(SDK_DIR) https://github.com/telink-semi/telink_zigbee_sdk/archive/refs/tags/V$(SDK_VERSION).zip
	unzip -o $(SDK_DIR)/V$(SDK_VERSION).zip 'telink_zigbee_sdk-$(SDK_VERSION)/tl_zigbee_sdk/*' -d $(SDK_DIR)
	mv $(SDK_DIR)/telink_zigbee_sdk-$(SDK_VERSION)/tl_zigbee_sdk/* $(SDK_DIR)/
	rm -r $(SDK_DIR)/telink_zigbee_sdk-$(SDK_VERSION)


toolchain:
	mkdir -p $(TOOLCHAIN_DIR)
	wget -P $(TOOLCHAIN_DIR) http://shyboy.oss-cn-shenzhen.aliyuncs.com/readonly/tc32_gcc_v2.0.tar.bz2 
	tar -xvjf $(TOOLCHAIN_DIR)/tc32_gcc_v2.0.tar.bz2 -C $(TOOLCHAIN_DIR)


install: sdk toolchain


clean_sdk:
	rm -rf $(SDK_DIR)


clean_toolchain:
	rm -rf $(TOOLCHAIN_DIR)


clean_install: clean_sdk clean_toolchain