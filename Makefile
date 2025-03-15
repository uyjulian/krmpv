#############################################
##                                         ##
##    Copyright (C) 2020-2022 Julian Uy    ##
##  https://sites.google.com/site/awertyb  ##
##                                         ##
##   See details of license at "LICENSE"   ##
##                                         ##
#############################################

BASESOURCES += main.cpp CharacterSet.cpp
SOURCES += $(BASESOURCES)
PROJECT_BASENAME = krmpv

RC_URL ?= https://github.com/uyjulian/$(PROJECT_BASENAME)
RC_FILEDESCRIPTION ?= libmpv video player plugin for TVP(KIRIKIRI) (2/Z)
RC_LEGALCOPYRIGHT ?= Copyright (C) 2020-2022 Julian Uy; This product is licensed under the LGPL2.1 or later license.
RC_PRODUCTNAME ?= libmpv video player plugin for TVP(KIRIKIRI) (2/Z)

include external/ncbind/Rules.lib.make

ifeq (xarm32,x$(TARGET_ARCH))
    OPENGL_LIB ?=
    MPV_ENABLEOPENGL ?= --disable-gl-win32
endif

ifeq (xarm64,x$(TARGET_ARCH))
    OPENGL_LIB ?=
    MPV_ENABLEOPENGL ?= --disable-gl-win32
endif

OPENGL_LIB ?= -lopengl32
MPV_ENABLEOPENGL ?= --enable-gl-win32

DEPENDENCY_BUILD_DIRECTORY_THIRD_PARTY_CMAKE := $(DEPENDENCY_BUILD_DIRECTORY)/third_party_cmake

EXTLIBS += $(DEPENDENCY_BUILD_DIRECTORY_THIRD_PARTY_CMAKE)/build-libraries/lib/libthird_party_cmake.a
SOURCES += $(EXTLIBS)
OBJECTS += $(EXTLIBS)
LDLIBS += $(EXTLIBS)
LDLIBS += -luuid -lbcrypt $(OPENGL_LIB) -lgdi32 -lole32 -loleaut32 -ldwmapi -lwinmm -lavrt -lsetupapi -limm32 -lversion

INCFLAGS += -I$(DEPENDENCY_BUILD_DIRECTORY_THIRD_PARTY_CMAKE)/build-libraries/include

$(DEPENDENCY_BUILD_DIRECTORY_THIRD_PARTY_CMAKE)/build-libraries/lib/libthird_party_cmake.a:
	cmake \
		-S third_party_cmake \
		-B $(DEPENDENCY_BUILD_DIRECTORY_THIRD_PARTY_CMAKE) \
		-DCMAKE_SYSTEM_NAME=Windows \
		-DCMAKE_SYSTEM_PROCESSOR=$(TARGET_CMAKE_SYSTEM_PROCESSOR) \
		-DCMAKE_FIND_ROOT_PATH=/dev/null \
		-DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER \
		-DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
		-DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY \
		-DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ONLY \
		-DCMAKE_DISABLE_FIND_PACKAGE_PkgConfig=TRUE \
		-DCMAKE_C_COMPILER=$(CC) \
		-DCMAKE_CXX_COMPILER=$(CXX) \
		-DCMAKE_RC_COMPILER=$(WINDRES) \
		-DCMAKE_BUILD_TYPE=Release \
		-DMPV_ENABLEOPENGL=$(MPV_ENABLEOPENGL) \
		&& \
	cmake --build $(DEPENDENCY_BUILD_DIRECTORY_THIRD_PARTY_CMAKE)

$(BASESOURCES): $(EXTLIBS)
