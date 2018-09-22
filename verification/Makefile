# The location of the Vigor verification project
ifndef VNDS_PATH
$(error "VNDS_PATH must be set to the location of the Vigor verification \
         project.")
endif

# Binary name
APP := WireGuard

# Get current directory
SELF_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

# Path to WireGuard source files
WG_SRC := $(abspath $(SELF_DIR)/../src)

# Runtime and verification files
# TODO: Re-use list from main WireGuard Makefile?
NF_FILES := $(WG_SRC)/*.c

# Defines for verification
# Defines taken from regular kernel module build
# TODO: Investigate post-build steps used for Retpoline
NF_VERIF_DEFS := \
  -D__KERNEL__ \
  -DCONFIG_X86_X32_ABI \
  -DCONFIG_AS_CFI=1 \
  -DCONFIG_AS_CFI_SIGNAL_FRAME=1 \
  -DCONFIG_AS_CFI_SECTIONS=1 \
  -DCONFIG_AS_FXSAVEQ=1 \
  -DCONFIG_AS_SSSE3=1 \
  -DCONFIG_AS_CRC32=1 \
  -DCONFIG_AS_AVX=1 \
  -DCONFIG_AS_AVX2=1 \
  -DCONFIG_AS_AVX512=1 \
  -DCONFIG_AS_SHA1_NI=1 \
  -DCONFIG_AS_SHA256_NI=1 \
  -DRETPOLINE \
  -DCC_USING_FENTRY \
  -DMODULE \
  -DKBUILD_BASENAME='"netlink"' \
  -DKBUILD_MODNAME='"wireguard"'

# Disabled defines that conflict with Clang
# TODO: Measure performance difference caused by these changes
# -DCC_HAVE_ASM_GOTO: Triggers incompatible asm via jump_label.h

# Disable features through defines that conflict with Clang
# TODO: Measure performance difference caused by these changes
# NF_VERIF_DEFS += \

# Start by disabling default include directories since we want precise control
NF_VERIF_INCLUDES := -nostdinc
NF_VERIF_INCLUDES += -isystem /home/jryans/Projects/llvm-project/build-for-vigor/bin/../lib/clang/3.4.2/include

# Add include directories for stubs by default
ENABLE_STUBS ?= no

# Include directories for stubs
# TODO: Write more detailed explanation
ifeq ($(ENABLE_STUBS),yes)
  NF_VERIF_INCLUDES += -I $(SELF_DIR)/stubs/linux-arch-x86
  NF_VERIF_INCLUDES += -I $(SELF_DIR)/stubs/linux
  NF_VERIF_INCLUDES += -I $(SELF_DIR)/stubs/linux-uapi
endif

# Include directories that reuse unmodified header files
# Tested with 4.15.0-33-generic kernel for Ubuntu 18.04
# Include directory order taken from regular kernel module build
KERNEL_HEADERS ?= clang

# Approach A
# Each include root below is represented as separate directory tree of symbolic
# links for easier navigation and comprehension.
ifeq ($(KERNEL_HEADERS),subset)
  # -I./arch/x86/include
  NF_VERIF_INCLUDES += -I $(SELF_DIR)/includes/linux-arch-x86
  # -I./arch/x86/include/generated
  # -I./include
  NF_VERIF_INCLUDES += -I $(SELF_DIR)/includes/linux
  # -I./arch/x86/include/uapi
  NF_VERIF_INCLUDES += -I $(SELF_DIR)/includes/linux-arch-x86-uapi
  # -I./arch/x86/include/generated/uapi
  # -I./include/uapi
  NF_VERIF_INCLUDES += -I $(SELF_DIR)/includes/linux-uapi
  # -I./include/generated/uapi
  NF_VERIF_INCLUDES += -I $(SELF_DIR)/includes/linux-generated-uapi
  # -include ./include/linux/kconfig.h
  NF_VERIF_INCLUDES += -include $(SELF_DIR)/includes/linux/linux/kconfig.h
endif

# Approach B
# Include full kernel header directories
ifeq ($(KERNEL_HEADERS),full)
  KERNEL := $(shell uname -r)
  KERNEL_BUILD := /lib/modules/$(KERNEL)/build
  NF_VERIF_INCLUDES += \
    -I $(KERNEL_BUILD)/arch/x86/include \
    -I $(KERNEL_BUILD)/arch/x86/include/generated \
    -I $(KERNEL_BUILD)/include \
    -I $(KERNEL_BUILD)/arch/x86/include/uapi \
    -I $(KERNEL_BUILD)/arch/x86/include/generated/uapi \
    -I $(KERNEL_BUILD)/include/uapi \
    -I $(KERNEL_BUILD)/include/generated/uapi \
    -include $(KERNEL_BUILD)/include/linux/kconfig.h
endif

# Approach C
# Include full kernel header directories modified for Clang
ifeq ($(KERNEL_HEADERS),clang)
  KERNEL := $(shell uname -r)
  KERNEL_BUILD := /home/jryans/Projects/linux-bionic-headers/linux-headers-$(KERNEL)
  NF_VERIF_INCLUDES += \
    -I $(KERNEL_BUILD)/arch/x86/include \
    -I $(KERNEL_BUILD)/arch/x86/include/generated \
    -I $(KERNEL_BUILD)/include \
    -I $(KERNEL_BUILD)/arch/x86/include/uapi \
    -I $(KERNEL_BUILD)/arch/x86/include/generated/uapi \
    -I $(KERNEL_BUILD)/include/uapi \
    -I $(KERNEL_BUILD)/include/generated/uapi \
    -include $(KERNEL_BUILD)/include/linux/kconfig.h
endif

# Include directories from WireGuard
NF_VERIF_INCLUDES += -I $(WG_SRC)/crypto/include

# Include directories from WireGuard kernal compat
# TODO: Exclude as much as possible by using the latest Linux kernel for
# verification. Some files are still needed with recent kernels; investigate.
NF_VERIF_INCLUDES += -include $(WG_SRC)/compat/compat.h
NF_VERIF_INCLUDES += -I $(WG_SRC)/compat/simd/include

# Verification files
NF_VERIF_FILES :=

# Verification arguments
NF_VERIF_ARGS :=

# Include the Vigor build pipeline
include $(VNDS_PATH)/nf/Makefile

# Reset VERIF_FILES for now
# TODO: Remove this when the boundaries are better understood
VERIF_FILES := $(NF_FILES)

# Additional flags from regular kernel module build
VERIF_FLAGS += -std=gnu89

# Debugging (TODO: Remove)
# VERIF_FLAGS += -E -v -
# VERIF_FLAGS += -H
# VERIF_FLAGS += -ferror-limit=60
# VERIF_FLAGS += -E

# Verification using stub kernel APIs
verify-with-stub-kernel:
	$(CLEAN_COMMAND)
	$(COMPILE_COMMAND) $(VERIF_DEFS) -DVIGOR_STUB_KERNEL $(VERIF_INCLUDES) $(VERIF_FILES) $(VERIF_FLAGS)
	$(LINK_COMMAND)
	$(OPT_COMMAND)
	$(VERIF_COMMAND) $(NF_VERIF_BASE_ARGS) $(NF_VERIF_ARGS)
	$(CLEAN_COMMAND)
