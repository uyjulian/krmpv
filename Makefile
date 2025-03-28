#############################################
##                                         ##
##    Copyright (C) 2020-2022 Julian Uy    ##
##  https://sites.google.com/site/awertyb  ##
##                                         ##
##   See details of license at "LICENSE"   ##
##                                         ##
#############################################

SOURCES += main.cpp CharacterSet.cpp
PROJECT_BASENAME = krmpv

RC_URL ?= https://github.com/uyjulian/$(PROJECT_BASENAME)
RC_FILEDESCRIPTION ?= libmpv video player plugin for TVP(KIRIKIRI) (2/Z)
RC_LEGALCOPYRIGHT ?= Copyright (C) 2020-2022 Julian Uy; This product is licensed under the LGPL2.1 or later license.
RC_PRODUCTNAME ?= libmpv video player plugin for TVP(KIRIKIRI) (2/Z)

include external/ncbind/Rules.lib.make

ifeq (xarm32,x$(TARGET_ARCH))
    OPENGL_LIB ?=
endif

ifeq (xarm64,x$(TARGET_ARCH))
    OPENGL_LIB ?=
endif

OPENGL_LIB ?= -lopengl32

LDLIBS += -luuid -lbcrypt $(OPENGL_LIB) -lgdi32 -lole32 -loleaut32 -ldwmapi -lwinmm -lavrt -lsetupapi -limm32 -lversion
