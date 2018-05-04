
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm

LOCAL_MODULE    := Dexvmp

LOCAL_SRC_FILES := ioapi.c \
				   unzip.c \
				   Globals.cpp \
				   avmp.cpp \
				   BitConvert.cpp \
				   InterpC.cpp \
				   io.cpp \
				   Utils.cpp \
				   YcFile.cpp\
				   InterpDir/Interp.cpp

LOCAL_SRC_FILES += DexOpcodes.cpp \
				   Exception.cpp \

LOCAL_LDLIBS := -llog -lz

include $(BUILD_SHARED_LIBRARY)
