#############################################
##                                         ##
##    Copyright (C) 2020-2021 Julian Uy    ##
##  https://sites.google.com/site/awertyb  ##
##                                         ##
##   See details of license at "LICENSE"   ##
##                                         ##
#############################################

BASESOURCES += main.cpp CharacterSet.cpp
SOURCES += $(BASESOURCES)
PROJECT_BASENAME = krmpv

RC_FILEDESCRIPTION ?= libmpv video player plugin for TVP(KIRIKIRI) (2/Z)
RC_LEGALCOPYRIGHT ?= Copyright (C) 2020-2021 Julian Uy; This product is licensed under the MIT license.
RC_PRODUCTNAME ?= libmpv video player plugin for TVP(KIRIKIRI) (2/Z)

include external/ncbind/Rules.lib.make

DEPENDENCY_SOURCE_DIRECTORY := $(abspath build-source)
DEPENDENCY_SOURCE_DIRECTORY_ICONV := $(DEPENDENCY_SOURCE_DIRECTORY)/iconv
DEPENDENCY_SOURCE_DIRECTORY_ZLIB := $(DEPENDENCY_SOURCE_DIRECTORY)/zlib
DEPENDENCY_SOURCE_DIRECTORY_FFMPEG := $(DEPENDENCY_SOURCE_DIRECTORY)/ffmpeg
DEPENDENCY_SOURCE_DIRECTORY_FRIBIDI := $(DEPENDENCY_SOURCE_DIRECTORY)/fribidi
DEPENDENCY_SOURCE_DIRECTORY_BROTLI := $(DEPENDENCY_SOURCE_DIRECTORY)/brotli
DEPENDENCY_SOURCE_DIRECTORY_FREETYPE2 := $(DEPENDENCY_SOURCE_DIRECTORY)/freetype2
DEPENDENCY_SOURCE_DIRECTORY_HARFBUZZ := $(DEPENDENCY_SOURCE_DIRECTORY)/harfbuzz
DEPENDENCY_SOURCE_DIRECTORY_LIBASS := $(DEPENDENCY_SOURCE_DIRECTORY)/libass
DEPENDENCY_SOURCE_DIRECTORY_MPV := $(DEPENDENCY_SOURCE_DIRECTORY)/mpv-$(TARGET_ARCH)

DEPENDENCY_SOURCE_FILE_ICONV := $(DEPENDENCY_SOURCE_DIRECTORY)/iconv.tar.gz
DEPENDENCY_SOURCE_FILE_ZLIB := $(DEPENDENCY_SOURCE_DIRECTORY)/zlib.tar.xz
DEPENDENCY_SOURCE_FILE_FFMPEG := $(DEPENDENCY_SOURCE_DIRECTORY)/ffmpeg.tar.xz
DEPENDENCY_SOURCE_FILE_FRIBIDI := $(DEPENDENCY_SOURCE_DIRECTORY)/fribidi.tar.xz
DEPENDENCY_SOURCE_FILE_BROTLI := $(DEPENDENCY_SOURCE_DIRECTORY)/brotli.tar.gz
DEPENDENCY_SOURCE_FILE_FREETYPE2 := $(DEPENDENCY_SOURCE_DIRECTORY)/freetype2.tar.xz
DEPENDENCY_SOURCE_FILE_HARFBUZZ := $(DEPENDENCY_SOURCE_DIRECTORY)/harfbuzz.tar.xz
DEPENDENCY_SOURCE_FILE_LIBASS := $(DEPENDENCY_SOURCE_DIRECTORY)/libass.tar.xz
DEPENDENCY_SOURCE_FILE_MPV := $(DEPENDENCY_SOURCE_DIRECTORY)/mpv.tar.gz

DEPENDENCY_SOURCE_URL_ICONV := https://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.16.tar.gz
DEPENDENCY_SOURCE_URL_ZLIB := https://zlib.net/zlib-1.2.11.tar.xz
DEPENDENCY_SOURCE_URL_FFMPEG := https://ffmpeg.org/releases/ffmpeg-4.4.1.tar.xz
DEPENDENCY_SOURCE_URL_FRIBIDI := https://github.com/fribidi/fribidi/releases/download/v1.0.11/fribidi-1.0.11.tar.xz
DEPENDENCY_SOURCE_URL_BROTLI := https://github.com/google/brotli/archive/refs/tags/v1.0.9.tar.gz
DEPENDENCY_SOURCE_URL_FREETYPE2 := https://download.savannah.gnu.org/releases/freetype/freetype-2.11.0.tar.xz
DEPENDENCY_SOURCE_URL_HARFBUZZ := https://github.com/harfbuzz/harfbuzz/releases/download/3.1.2/harfbuzz-3.1.2.tar.xz
DEPENDENCY_SOURCE_URL_LIBASS := https://github.com/libass/libass/releases/download/0.15.2/libass-0.15.2.tar.xz
DEPENDENCY_SOURCE_URL_MPV := https://github.com/mpv-player/mpv/archive/refs/tags/v0.34.1.tar.gz

$(DEPENDENCY_SOURCE_DIRECTORY):
	mkdir -p $@

$(DEPENDENCY_SOURCE_FILE_ICONV): | $(DEPENDENCY_SOURCE_DIRECTORY)
	curl --location --output $@ $(DEPENDENCY_SOURCE_URL_ICONV)

$(DEPENDENCY_SOURCE_FILE_ZLIB): | $(DEPENDENCY_SOURCE_DIRECTORY)
	curl --location --output $@ $(DEPENDENCY_SOURCE_URL_ZLIB)

$(DEPENDENCY_SOURCE_FILE_FFMPEG): | $(DEPENDENCY_SOURCE_DIRECTORY)
	curl --location --output $@ $(DEPENDENCY_SOURCE_URL_FFMPEG)

$(DEPENDENCY_SOURCE_FILE_FRIBIDI): | $(DEPENDENCY_SOURCE_DIRECTORY)
	curl --location --output $@ $(DEPENDENCY_SOURCE_URL_FRIBIDI)

$(DEPENDENCY_SOURCE_FILE_BROTLI): | $(DEPENDENCY_SOURCE_DIRECTORY)
	curl --location --output $@ $(DEPENDENCY_SOURCE_URL_BROTLI)

$(DEPENDENCY_SOURCE_FILE_FREETYPE2): | $(DEPENDENCY_SOURCE_DIRECTORY)
	curl --location --output $@ $(DEPENDENCY_SOURCE_URL_FREETYPE2)

$(DEPENDENCY_SOURCE_FILE_HARFBUZZ): | $(DEPENDENCY_SOURCE_DIRECTORY)
	curl --location --output $@ $(DEPENDENCY_SOURCE_URL_HARFBUZZ)

$(DEPENDENCY_SOURCE_FILE_LIBASS): | $(DEPENDENCY_SOURCE_DIRECTORY)
	curl --location --output $@ $(DEPENDENCY_SOURCE_URL_LIBASS)

$(DEPENDENCY_SOURCE_FILE_MPV): | $(DEPENDENCY_SOURCE_DIRECTORY)
	curl --location --output $@ $(DEPENDENCY_SOURCE_URL_MPV)

$(DEPENDENCY_SOURCE_DIRECTORY_ICONV): $(DEPENDENCY_SOURCE_FILE_ICONV)
	mkdir -p $@
	tar -x -f $< -C $@ --strip-components 1

$(DEPENDENCY_SOURCE_DIRECTORY_ZLIB): $(DEPENDENCY_SOURCE_FILE_ZLIB)
	mkdir -p $@
	tar -x -f $< -C $@ --strip-components 1

$(DEPENDENCY_SOURCE_DIRECTORY_FFMPEG): $(DEPENDENCY_SOURCE_FILE_FFMPEG)
	mkdir -p $@
	tar -x -f $< -C $@ --strip-components 1

$(DEPENDENCY_SOURCE_DIRECTORY_FRIBIDI): $(DEPENDENCY_SOURCE_FILE_FRIBIDI)
	mkdir -p $@
	tar -x -f $< -C $@ --strip-components 1

$(DEPENDENCY_SOURCE_DIRECTORY_BROTLI): $(DEPENDENCY_SOURCE_FILE_BROTLI)
	mkdir -p $@
	tar -x -f $< -C $@ --strip-components 1

$(DEPENDENCY_SOURCE_DIRECTORY_FREETYPE2): $(DEPENDENCY_SOURCE_FILE_FREETYPE2)
	mkdir -p $@
	tar -x -f $< -C $@ --strip-components 1

$(DEPENDENCY_SOURCE_DIRECTORY_HARFBUZZ): $(DEPENDENCY_SOURCE_FILE_HARFBUZZ)
	mkdir -p $@
	tar -x -f $< -C $@ --strip-components 1

$(DEPENDENCY_SOURCE_DIRECTORY_LIBASS): $(DEPENDENCY_SOURCE_FILE_LIBASS)
	mkdir -p $@
	tar -x -f $< -C $@ --strip-components 1

$(DEPENDENCY_SOURCE_DIRECTORY_MPV): $(DEPENDENCY_SOURCE_FILE_MPV)
	mkdir -p $@
	tar -x -f $< -C $@ --strip-components 1

DEPENDENCY_BUILD_DIRECTORY := $(abspath build-$(TARGET_ARCH))
DEPENDENCY_BUILD_DIRECTORY_ICONV := $(DEPENDENCY_BUILD_DIRECTORY)/iconv
DEPENDENCY_BUILD_DIRECTORY_ZLIB := $(DEPENDENCY_BUILD_DIRECTORY)/zlib
DEPENDENCY_BUILD_DIRECTORY_FFMPEG := $(DEPENDENCY_BUILD_DIRECTORY)/ffmpeg
DEPENDENCY_BUILD_DIRECTORY_FRIBIDI := $(DEPENDENCY_BUILD_DIRECTORY)/fribidi
DEPENDENCY_BUILD_DIRECTORY_BROTLI := $(DEPENDENCY_BUILD_DIRECTORY)/brotli
DEPENDENCY_BUILD_DIRECTORY_FREETYPE2_NOHB := $(DEPENDENCY_BUILD_DIRECTORY)/freetype2_nohb
DEPENDENCY_BUILD_DIRECTORY_FREETYPE2 := $(DEPENDENCY_BUILD_DIRECTORY)/freetype2
DEPENDENCY_BUILD_DIRECTORY_HARFBUZZ := $(DEPENDENCY_BUILD_DIRECTORY)/harfbuzz
DEPENDENCY_BUILD_DIRECTORY_LIBASS := $(DEPENDENCY_BUILD_DIRECTORY)/libass

DEPENDENCY_OUTPUT_DIRECTORY := $(abspath build-libraries)-$(TARGET_ARCH)
DEPENDENCY_OUTPUT_DIRECTORY_FREETYPE2_NOHB := $(abspath $(DEPENDENCY_BUILD_DIRECTORY))/freetype2_nohb_output

FFMPEG_LIBS := $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libavformat.a $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libavcodec.a $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libavfilter.a $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libswresample.a $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libswscale.a $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libavutil.a

ifeq (xintel32,x$(TARGET_ARCH))
    FFMPEG_ARCH := x86
endif

ifeq (xintel64,x$(TARGET_ARCH))
    FFMPEG_ARCH := x86_64
endif

ifeq (xarm32,x$(TARGET_ARCH))
    FFMPEG_ARCH := arm
endif

ifeq (xarm64,x$(TARGET_ARCH))
    FFMPEG_ARCH := aarch64
endif

FFMPEG_ARCH ?= x86

EXTLIBS += $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libiconv.a $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libz.a $(FFMPEG_LIBS) $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libass.a $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libfribidi.a $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libharfbuzz.a $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libfreetype.a $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libbrotlidec.a $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libbrotlicommon.a $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libmpv.a
SOURCES += $(EXTLIBS)
OBJECTS += $(EXTLIBS)
LDLIBS += $(EXTLIBS) $(EXTLIBS) $(EXTLIBS) -luuid -lbcrypt -lopengl32 -lgdi32 -lole32 -ldwmapi -lwinmm -lavrt

INCFLAGS += -I$(DEPENDENCY_OUTPUT_DIRECTORY)/include

$(BASESOURCES): $(EXTLIBS)

$(DEPENDENCY_OUTPUT_DIRECTORY):
	mkdir -p $@

$(DEPENDENCY_OUTPUT_DIRECTORY_FREETYPE2_NOHB):
	mkdir -p $@

$(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libiconv.a: | $(DEPENDENCY_SOURCE_DIRECTORY_ICONV) $(DEPENDENCY_OUTPUT_DIRECTORY)
	mkdir -p $(DEPENDENCY_BUILD_DIRECTORY_ICONV) && \
	cd $(DEPENDENCY_BUILD_DIRECTORY_ICONV) && \
	PKG_CONFIG_PATH=$(DEPENDENCY_OUTPUT_DIRECTORY)/lib/pkgconfig \
	$(DEPENDENCY_SOURCE_DIRECTORY_ICONV)/configure \
		CFLAGS="-O2" \
		--prefix="$(DEPENDENCY_OUTPUT_DIRECTORY)" \
		--host=$(patsubst %-,%,$(TOOL_TRIPLET_PREFIX)) \
		--enable-static \
		--disable-shared \
	&& \
	$(MAKE) && \
	$(MAKE) install

$(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libz.a: | $(DEPENDENCY_SOURCE_DIRECTORY_ZLIB) $(DEPENDENCY_OUTPUT_DIRECTORY)
	mkdir -p $(DEPENDENCY_BUILD_DIRECTORY_ZLIB) && \
	cd $(DEPENDENCY_BUILD_DIRECTORY_ZLIB) && \
	PKG_CONFIG_PATH=$(DEPENDENCY_OUTPUT_DIRECTORY)/lib/pkgconfig \
	CFLAGS="-O2" \
	CROSS_PREFIX="$(TOOL_TRIPLET_PREFIX)" \
	$(DEPENDENCY_SOURCE_DIRECTORY_ZLIB)/configure \
		--prefix="$(DEPENDENCY_OUTPUT_DIRECTORY)" \
		--static \
	&& \
	$(MAKE) && \
	$(MAKE) install

$(DEPENDENCY_BUILD_DIRECTORY_FFMPEG): | $(DEPENDENCY_SOURCE_DIRECTORY_FFMPEG) $(DEPENDENCY_OUTPUT_DIRECTORY)
	mkdir -p $(DEPENDENCY_BUILD_DIRECTORY_FFMPEG) && \
	cd $(DEPENDENCY_BUILD_DIRECTORY_FFMPEG) && \
	$(DEPENDENCY_SOURCE_DIRECTORY_FFMPEG)/configure \
		--prefix="$(DEPENDENCY_OUTPUT_DIRECTORY)" \
		--enable-optimizations \
		--disable-avdevice \
		--disable-cuda \
		--disable-cuvid \
		--disable-debug \
		--disable-doc \
		--disable-nvenc \
		--disable-postproc \
		--disable-programs \
		--disable-pthreads \
		--disable-schannel \
		--disable-shared \
		--disable-audiotoolbox \
		--disable-d3d11va \
		--disable-dxva2 \
		--enable-swresample \
		--enable-runtime-cpudetect \
		--enable-static \
		--enable-w32threads \
		--disable-everything \
		--disable-protocols \
		--disable-network \
		--disable-devices \
		--enable-decoders \
		--enable-demuxers \
		--disable-decoder=imm5,rawvideo,wrapped_avframe,xface,yop,012v,4xm,8bps,aasc,agm,alias_pix,amv,anm,ansi,arbc,argo,asv1,asv2,aura,aura2,av1,avrn,avrp,avs,avui,ayuv,bethsoftvid,bfi,binkvideo,bintext,bmp,bmv_video,brender_pix,c93,camstudio,camtasia,cavs,cdgraphics,cdtoons,cdxl,cinepak,clearvideo,cljr,cpia,cyuv,dfa,dpx,dsicinvideo,dxa,eacmv,eamad,eatgq,eatgv,eatqi,escape124,escape130,fits,flashsv,flashsv2,flic,fmvc,frwu,g2m,gdv,gif,h261,hnm4video,hq_hqa,idcinvideo,idf,iff,imm4,indeo2,indeo3,indeo4,indeo5,interplayvideo,ipu,jpegls,jv,kgv1,kmvc,loco,lscr,m101,mjpeg,mjpegb,mmvideo,mobiclip,motionpixels,msa1,mscc,msp2,msrle,mss1,mss2,msvideo1,mts2,mv30,mvc1,mvc2,mvdv,mvha,mwsc,mxpeg,nuv,paf_video,pam,pbm,pcx,pfm,pgm,pgmyuv,pgx,pictor,ppm,prosumer,ptx,qdraw,qpeg,qtrle,r10k,r210,rasc,rl2,roqvideo,rpza,rscc,rv10,rv20,sanm,scpr,screenpresso,sga,sgi,sgirle,simbiosis_imx,smackvid,smc,smvjpeg,snow,sp5x,speedhq,srgc,sunrast,svq1,targa,targa_y216,tdsc,thp,tiertexseqvideo,tmv,truemotion1,truemotion2,truemotion2rt,tscc2,txd,ultimotion,v210x,v308,v408,vb,vc1,vc1image,vcr1,vmdvideo,vmnc,vp5,vp6,vp6f,vp7,vqavideo,wcmv,wmv3,wmv3image,wnv1,xan_wc3,xan_wc4,xbin,xbm,xl,xpm,xwd,y41p,yuv4,zerocodec,zmbv,flv,h263,h263i,h263p,msmpeg4,msmpeg4v1,msmpeg4v2,svq3,wmv1,wmv2,bitpacked,dds,dirac,fic,vp6a,mpeg1video,mpeg2video,mpegvideo,aic,apng,cfhd,cllc,cri,dxtory,fraps,lagarith,mdec,mimic,mszh,notchlc,photocd,pixlet,png,psd,rv30,rv40,sheervideo,tiff,utvideo,vble,webp,ylc,ffvhuff,huffyuv,hymt,mpeg4,theora,vp3,vp4,dnxhd,dvvideo,dxv,exr,ffv1,h264,hap,hevc,hqx,jpeg2000,magicyuv,prores,v210,v410,vp8,vp9 \
		--arch=$(FFMPEG_ARCH) \
		--enable-cross-compile \
		--cross-prefix=$(TOOL_TRIPLET_PREFIX) \
		--target-os=mingw64

$(DEPENDENCY_OUTPUT_DIRECTORY)/lib/pkgconfig/libavformat.pc: | $(DEPENDENCY_BUILD_DIRECTORY_FFMPEG)
	cd $(DEPENDENCY_BUILD_DIRECTORY_FFMPEG) && \
	$(MAKE) && \
	$(MAKE) install

$(FFMPEG_LIBS): $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/pkgconfig/libavformat.pc

$(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libfribidi.a: | $(DEPENDENCY_SOURCE_DIRECTORY_FRIBIDI) $(DEPENDENCY_OUTPUT_DIRECTORY)
	mkdir -p $(DEPENDENCY_BUILD_DIRECTORY_FRIBIDI) && \
	cd $(DEPENDENCY_BUILD_DIRECTORY_FRIBIDI) && \
	PKG_CONFIG_PATH=$(DEPENDENCY_OUTPUT_DIRECTORY)/lib/pkgconfig \
	$(DEPENDENCY_SOURCE_DIRECTORY_FRIBIDI)/configure \
		CFLAGS="-O2" \
		--prefix="$(DEPENDENCY_OUTPUT_DIRECTORY)" \
		--host=$(patsubst %-,%,$(TOOL_TRIPLET_PREFIX)) \
		--enable-static \
		--disable-shared \
		--disable-dependency-tracking \
		--disable-debug \
	&& \
	$(MAKE) && \
	$(MAKE) install

$(DEPENDENCY_SOURCE_DIRECTORY_BROTLI)/aclocal.m4: | $(DEPENDENCY_SOURCE_DIRECTORY_BROTLI)
	cd $(DEPENDENCY_SOURCE_DIRECTORY_BROTLI) && \
	NOCONFIGURE=1 ./bootstrap

$(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libbrotlicommon.la: | $(DEPENDENCY_SOURCE_DIRECTORY_BROTLI)/aclocal.m4 $(DEPENDENCY_OUTPUT_DIRECTORY)
	mkdir -p $(DEPENDENCY_BUILD_DIRECTORY_BROTLI) && \
	cd $(DEPENDENCY_BUILD_DIRECTORY_BROTLI) && \
	PKG_CONFIG_PATH=$(DEPENDENCY_OUTPUT_DIRECTORY)/lib/pkgconfig \
	$(DEPENDENCY_SOURCE_DIRECTORY_BROTLI)/configure \
		CFLAGS="-O2" \
		CXXFLAGS="-O2" \
		--prefix="$(DEPENDENCY_OUTPUT_DIRECTORY)" \
		--host=$(patsubst %-,%,$(TOOL_TRIPLET_PREFIX)) \
		--enable-static \
		--disable-shared \
		--disable-dependency-tracking \
	&& \
	$(MAKE) && \
	$(MAKE) install

$(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libbrotlidec.a $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libbrotlicommon.a: $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libbrotlicommon.la

$(DEPENDENCY_OUTPUT_DIRECTORY_FREETYPE2_NOHB)/lib/libfreetype.a: | $(DEPENDENCY_SOURCE_DIRECTORY_FREETYPE2) $(DEPENDENCY_OUTPUT_DIRECTORY_FREETYPE2_NOHB)
	mkdir -p $(DEPENDENCY_BUILD_DIRECTORY_FREETYPE2) && \
	cd $(DEPENDENCY_BUILD_DIRECTORY_FREETYPE2) && \
	PKG_CONFIG_PATH=$(DEPENDENCY_OUTPUT_DIRECTORY_FREETYPE2_NOHB)/lib/pkgconfig \
	$(DEPENDENCY_SOURCE_DIRECTORY_FREETYPE2)/configure \
		CFLAGS="-O2" \
		--prefix="$(DEPENDENCY_OUTPUT_DIRECTORY_FREETYPE2_NOHB)" \
		--host=$(patsubst %-,%,$(TOOL_TRIPLET_PREFIX)) \
		--enable-static \
		--disable-shared \
		\
		--without-brotli \
		--without-zlib \
		--without-bzip2 \
		--without-png \
		--without-harfbuzz \
	&& \
	$(MAKE) && \
	$(MAKE) install

$(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libharfbuzz.a: $(DEPENDENCY_OUTPUT_DIRECTORY_FREETYPE2_NOHB)/lib/libfreetype.a | $(DEPENDENCY_SOURCE_DIRECTORY_HARFBUZZ) $(DEPENDENCY_OUTPUT_DIRECTORY)
	mkdir -p $(DEPENDENCY_BUILD_DIRECTORY_HARFBUZZ) && \
	cd $(DEPENDENCY_BUILD_DIRECTORY_HARFBUZZ) && \
	PKG_CONFIG_PATH=$(DEPENDENCY_OUTPUT_DIRECTORY_FREETYPE2_NOHB)/lib/pkgconfig \
	$(DEPENDENCY_SOURCE_DIRECTORY_HARFBUZZ)/configure \
		CFLAGS="-O2 -DHB_NO_MT" \
		CXXFLAGS="-O2 -DHB_NO_MT" \
		--prefix="$(DEPENDENCY_OUTPUT_DIRECTORY)" \
		--host=$(patsubst %-,%,$(TOOL_TRIPLET_PREFIX)) \
		--enable-static \
		--disable-shared \
		--disable-dependency-tracking \
		\
		--without-cairo \
		--without-fontconfig \
		--without-icu \
		--with-freetype \
		--without-glib \
	&& \
	$(MAKE) && \
	$(MAKE) install

$(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libfreetype.a: $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libharfbuzz.a $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libbrotlidec.a $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libbrotlicommon.a | $(DEPENDENCY_SOURCE_DIRECTORY_FREETYPE2) $(DEPENDENCY_OUTPUT_DIRECTORY)
	mkdir -p $(DEPENDENCY_BUILD_DIRECTORY_FREETYPE2) && \
	cd $(DEPENDENCY_BUILD_DIRECTORY_FREETYPE2) && \
	PKG_CONFIG_PATH=$(DEPENDENCY_OUTPUT_DIRECTORY)/lib/pkgconfig \
	$(DEPENDENCY_SOURCE_DIRECTORY_FREETYPE2)/configure \
		CFLAGS="-O2" \
		--prefix="$(DEPENDENCY_OUTPUT_DIRECTORY)" \
		--host=$(patsubst %-,%,$(TOOL_TRIPLET_PREFIX)) \
		--enable-static \
		--disable-shared \
		\
		--with-brotli \
		--without-zlib \
		--without-bzip2 \
		--without-png \
		--with-harfbuzz \
	&& \
	$(MAKE) && \
	$(MAKE) install

$(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libass.a: $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libfribidi.a $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libfreetype.a $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libharfbuzz.a | $(DEPENDENCY_SOURCE_DIRECTORY_LIBASS) $(DEPENDENCY_OUTPUT_DIRECTORY)
	mkdir -p $(DEPENDENCY_BUILD_DIRECTORY_LIBASS) && \
	cd $(DEPENDENCY_BUILD_DIRECTORY_LIBASS) && \
	PKG_CONFIG_PATH=$(DEPENDENCY_OUTPUT_DIRECTORY)/lib/pkgconfig \
	$(DEPENDENCY_SOURCE_DIRECTORY_LIBASS)/configure \
		CFLAGS="-DFRIBIDI_LIB_STATIC -O2" \
		--prefix="$(DEPENDENCY_OUTPUT_DIRECTORY)" \
		--host=$(patsubst %-,%,$(TOOL_TRIPLET_PREFIX)) \
		--disable-shared \
		--enable-static \
		--disable-asm \
		\
		--disable-require-system-font-provider \
		--disable-directwrite \
		--disable-fontconfig \
	&& \
	$(MAKE) && \
	$(MAKE) install

$(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libmpv.a: $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libiconv.a $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libz.a $(FFMPEG_LIBS) $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libass.a | $(DEPENDENCY_SOURCE_DIRECTORY_MPV) $(DEPENDENCY_OUTPUT_DIRECTORY)
	mkdir -p $(DEPENDENCY_SOURCE_DIRECTORY_MPV) && \
	cd $(DEPENDENCY_SOURCE_DIRECTORY_MPV) && \
	python3 $(DEPENDENCY_SOURCE_DIRECTORY_MPV)/bootstrap.py \
	&& \
	CC=$(CC) \
	CXX=$(CXX) \
	AR=$(AR) \
	NM=$(NM) \
	RANLIB=$(RANLIB) \
	CFLAGS="-I$(DEPENDENCY_OUTPUT_DIRECTORY)/include" \
	LDFLAGS="-L$(DEPENDENCY_OUTPUT_DIRECTORY)/lib" \
	LIBRARY_PATH=$(DEPENDENCY_OUTPUT_DIRECTORY) \
	PKG_CONFIG_PATH=$(DEPENDENCY_OUTPUT_DIRECTORY)/lib/pkgconfig \
	PKG_CONFIG_LIBDIR=$(DEPENDENCY_OUTPUT_DIRECTORY)/lib/pkgconfig \
	PKG_CONFIG_SYSROOT_DIR="$(DEPENDENCY_OUTPUT_DIRECTORY)" \
	WINDRES=$(WINDRES) \
	python3 $(DEPENDENCY_SOURCE_DIRECTORY_MPV)/waf configure \
		--enable-libmpv-static \
		--enable-static-build \
		--disable-lua \
		--disable-shaderc \
		--disable-spirv-cross \
		--disable-d3d11 \
		--disable-cplayer \
		--prefix=$(DEPENDENCY_OUTPUT_DIRECTORY) \
	&& \
	CC=$(CC) \
	CXX=$(CXX) \
	AR=$(AR) \
	NM=$(NM) \
	RANLIB=$(RANLIB) \
	CFLAGS="-I$(DEPENDENCY_OUTPUT_DIRECTORY)/include" \
	LDFLAGS="-L$(DEPENDENCY_OUTPUT_DIRECTORY)/lib" \
	WINDRES=$(WINDRES) \
	python3 $(DEPENDENCY_SOURCE_DIRECTORY_MPV)/waf build --verbose \
	&& \
	CC=$(CC) \
	CXX=$(CXX) \
	AR=$(AR) \
	NM=$(NM) \
	RANLIB=$(RANLIB) \
	CFLAGS="-I$(DEPENDENCY_OUTPUT_DIRECTORY)/include" \
	LDFLAGS="-L$(DEPENDENCY_OUTPUT_DIRECTORY)/lib" \
	python3 $(DEPENDENCY_SOURCE_DIRECTORY_MPV)/waf install

main.o: $(DEPENDENCY_OUTPUT_DIRECTORY)/lib/libmpv.a

clean::
	rm -rf $(DEPENDENCY_SOURCE_DIRECTORY) $(DEPENDENCY_BUILD_DIRECTORY) $(DEPENDENCY_OUTPUT_DIRECTORY)
