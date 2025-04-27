# Copyright (c) third_party_cmake developers

cmake_minimum_required(VERSION 3.12)
project(third_party_cmake)

if((NOT DEFINED CMAKE_BUILD_TYPE) OR (NOT CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE STREQUAL ""))
set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

if((NOT DEFINED MPV_ENABLEOPENGL) OR (NOT MPV_ENABLEOPENGL) OR (MPV_ENABLEOPENGL STREQUAL ""))
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL i686)
set(MPV_ENABLEOPENGL --enable-gl-win32)
endif()
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL amd64)
set(MPV_ENABLEOPENGL --enable-gl-win32)
endif()
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL arm)
set(MPV_ENABLEOPENGL --disable-gl-win32)
endif()
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL arm64)
set(MPV_ENABLEOPENGL --disable-gl-win32)
endif()
endif()

set(DEPENDENCY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/build-libraries)
file(MAKE_DIRECTORY ${DEPENDENCY_OUTPUT_DIRECTORY}/include)
set(DEPENDENCY_OUTPUT_DIRECTORY_NOHB ${CMAKE_CURRENT_BINARY_DIR}/build-libraries-nohb)
file(MAKE_DIRECTORY ${DEPENDENCY_OUTPUT_DIRECTORY_NOHB}/include)

if((NOT DEFINED MAKE_JOB_ARG) OR (NOT MAKE_JOB_ARG) OR (MAKE_JOB_ARG STREQUAL ""))
include(ProcessorCount)
ProcessorCount(MAKE_JOB_NUM)
set(MAKE_JOB_ARG -j1)
if(NOT MAKE_JOB_NUM EQUAL 0)
set(MAKE_JOB_ARG -j${MAKE_JOB_NUM})
endif()
endif()

if(NOT DEFINED TOOL_TRIPLET_PREFIX)
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL i686)
set(TOOL_TRIPLET_PREFIX i686-w64-mingw32-)
endif()
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL amd64)
set(TOOL_TRIPLET_PREFIX x86_64-w64-mingw32-)
endif()
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL arm)
set(TOOL_TRIPLET_PREFIX armv7-w64-mingw32-)
endif()
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL arm64)
set(TOOL_TRIPLET_PREFIX aarch64-w64-mingw32-)
endif()
if(NOT DEFINED TOOL_TRIPLET_PREFIX)
set(TOOL_TRIPLET_PREFIX i686-w64-mingw32-)
endif()
endif()
if(NOT DEFINED FFMPEG_ARCH)
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL i686)
set(FFMPEG_ARCH x86)
endif()
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL amd64)
set(FFMPEG_ARCH x86_64)
endif()
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL arm)
set(FFMPEG_ARCH arm)
endif()
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL arm64)
set(FFMPEG_ARCH aarch64)
endif()
if(NOT DEFINED FFMPEG_ARCH)
set(FFMPEG_ARCH x86)
endif()
endif()
string(REGEX REPLACE "-$" "" TOOL_TRIPLET_PREFIX_NOTRAILINGDASH "${TOOL_TRIPLET_PREFIX}")

include(ExternalProject)

ExternalProject_Add(spirv-cross-build
    URL https://github.com/uyjulian/SPIRV-Cross/archive/refs/tags/staticlinking-1.3.224.1.tar.gz
    URL_HASH SHA256=849402350d207bf1b45b6713367697ad5a6b8648c3d2e87c0831760451b0ee99
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    CONFIGURE_COMMAND ${CMAKE_COMMAND} -E env
        "PKG_CONFIG_PATH=${DEPENDENCY_OUTPUT_DIRECTORY}/lib/pkgconfig"
        ${CMAKE_COMMAND}
            -B <BINARY_DIR>
            -S <SOURCE_DIR>
            -DCMAKE_SYSTEM_NAME=Windows
            -DCMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR}
            -DCMAKE_FIND_ROOT_PATH=/dev/null
            -DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER
            -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY
            -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY
            -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ONLY
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -DCMAKE_RC_COMPILER=${CMAKE_RC_COMPILER}
            -DCMAKE_INSTALL_PREFIX=${DEPENDENCY_OUTPUT_DIRECTORY}
            -DCMAKE_BUILD_TYPE=Release
            -DSPIRV_CROSS_SHARED=ON
            -DSPIRV_CROSS_CLI=OFF
            -DSPIRV_CROSS_ENABLE_TESTS=OFF
            -DCMAKE_POLICY_VERSION_MINIMUM=3.12
    BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> && ${CMAKE_COMMAND} --build <BINARY_DIR> --target install && cp -f ${DEPENDENCY_OUTPUT_DIRECTORY}/lib/pkgconfig/spirv-cross-c-shared.pc ${DEPENDENCY_OUTPUT_DIRECTORY}/lib/pkgconfig/spirv-cross.pc
    BUILD_BYPRODUCTS ${DEPENDENCY_OUTPUT_DIRECTORY}/lib/libspirv-cross-c-shared.a
    INSTALL_COMMAND ""
)
list(APPEND THIRDPARTY_DEPENDS spirv-cross-build)

ExternalProject_Add(spirv-tools-build
    URL https://github.com/KhronosGroup/SPIRV-Tools/archive/refs/tags/v2025.1.tar.gz
    URL_HASH SHA256=6a9313fa68e061d463f616357cd24cdf1c3a27d906ea791d7ba67dd1b6666a40
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)
ExternalProject_Get_Property(spirv-tools-build SOURCE_DIR)
set(SHADERC_SPIRV_TOOLS_DIR ${SOURCE_DIR})
ExternalProject_Add(spirv-headers-build
    URL https://github.com/KhronosGroup/SPIRV-Headers/archive/refs/tags/vulkan-sdk-1.4.309.0.tar.gz
    URL_HASH SHA256=a96f8b4f2dfb18f7432e5c523e220ab0075372a9509e0c25fbff21c76af0de7c
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)
ExternalProject_Get_Property(spirv-headers-build SOURCE_DIR)
set(SHADERC_SPIRV_HEADERS_DIR ${SOURCE_DIR})
ExternalProject_Add(glslang-build
    URL https://github.com/KhronosGroup/glslang/archive/refs/tags/15.3.0.tar.gz
    URL_HASH SHA256=c6c21fe1873c37e639a6a9ac72d857ab63a5be6893a589f34e09a6c757174201
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)
ExternalProject_Get_Property(glslang-build SOURCE_DIR)
set(SHADERC_GLSLANG_DIR ${SOURCE_DIR})
ExternalProject_Add(shaderc-build
    DEPENDS spirv-tools-build spirv-headers-build glslang-build
    URL https://github.com/google/shaderc/archive/refs/tags/v2025.2.tar.gz
    URL_HASH SHA256=3fddc13bbb87411c6f7b8f447e87c1637933450087e70fc21da650041f4e0132
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    CONFIGURE_COMMAND ${CMAKE_COMMAND} -E env
        "PKG_CONFIG_PATH=${DEPENDENCY_OUTPUT_DIRECTORY}/lib/pkgconfig"
        ${CMAKE_COMMAND}
            -B <BINARY_DIR>
            -S <SOURCE_DIR>
            -DCMAKE_SYSTEM_NAME=Windows
            -DCMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR}
            -DCMAKE_FIND_ROOT_PATH=/dev/null
            -DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER
            -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY
            -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY
            -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ONLY
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -DCMAKE_RC_COMPILER=${CMAKE_RC_COMPILER}
            -DCMAKE_INSTALL_PREFIX=${DEPENDENCY_OUTPUT_DIRECTORY}
            -DCMAKE_BUILD_TYPE=Release
            -DSHADERC_SKIP_TESTS=ON
            -DSHADERC_SKIP_EXAMPLES=ON
            -DSHADERC_SPIRV_TOOLS_DIR=${SHADERC_SPIRV_TOOLS_DIR}
            -DSHADERC_SPIRV_HEADERS_DIR=${SHADERC_SPIRV_HEADERS_DIR}
            -DSHADERC_GLSLANG_DIR=${SHADERC_GLSLANG_DIR}
            -DSPIRV-Headers_SOURCE_DIR=${SHADERC_SPIRV_HEADERS_DIR}
            -DCMAKE_POLICY_VERSION_MINIMUM=3.12
    BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> && ${CMAKE_COMMAND} --build <BINARY_DIR> --target install
    BUILD_BYPRODUCTS ${DEPENDENCY_OUTPUT_DIRECTORY}/lib/libshaderc.a ${DEPENDENCY_OUTPUT_DIRECTORY}/lib/libshaderc_combined.a
    INSTALL_COMMAND ""
)
list(APPEND THIRDPARTY_DEPENDS shaderc-build)

ExternalProject_Add(iconv-build
    URL https://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.18.tar.gz
    URL_HASH SHA256=3b08f5f4f9b4eb82f151a7040bfd6fe6c6fb922efe4b1659c66ea933276965e8
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    CONFIGURE_COMMAND ${CMAKE_COMMAND} -E env
        "PKG_CONFIG_PATH=${DEPENDENCY_OUTPUT_DIRECTORY}/lib/pkgconfig"
        <SOURCE_DIR>/configure
            CFLAGS=-O2
            "--prefix=${DEPENDENCY_OUTPUT_DIRECTORY}"
            --host=${TOOL_TRIPLET_PREFIX_NOTRAILINGDASH}
            --enable-static
            --disable-shared
    BUILD_COMMAND make -C <BINARY_DIR> ${MAKE_JOB_ARG} && make -C <BINARY_DIR> ${MAKE_JOB_ARG} install
    BUILD_BYPRODUCTS ${DEPENDENCY_OUTPUT_DIRECTORY}/lib/libiconv.a
    INSTALL_COMMAND ""
)
list(APPEND THIRDPARTY_DEPENDS iconv-build)

ExternalProject_Add(zlib-build
    URL https://github.com/madler/zlib/archive/refs/tags/v1.2.12.tar.gz
    URL_HASH SHA256=d8688496ea40fb61787500e863cc63c9afcbc524468cedeb478068924eb54932
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    CONFIGURE_COMMAND ${CMAKE_COMMAND} -E env
        "PKG_CONFIG_PATH=${DEPENDENCY_OUTPUT_DIRECTORY}/lib/pkgconfig"
        CFLAGS=-O2
        "CROSS_PREFIX=${TOOL_TRIPLET_PREFIX}"
        <SOURCE_DIR>/configure
            "--prefix=${DEPENDENCY_OUTPUT_DIRECTORY}"
            --static
    BUILD_COMMAND make -C <BINARY_DIR> ${MAKE_JOB_ARG} && make -C <BINARY_DIR> ${MAKE_JOB_ARG} install
    BUILD_BYPRODUCTS ${DEPENDENCY_OUTPUT_DIRECTORY}/lib/libz.a
    INSTALL_COMMAND ""
)
list(APPEND THIRDPARTY_DEPENDS zlib-build)

ExternalProject_Add(ffmpeg-build
    URL https://ffmpeg.org/releases/ffmpeg-5.0.tar.xz
    URL_HASH SHA256=51e919f7d205062c0fd4fae6243a84850391115104ccf1efc451733bc0ac7298
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    CONFIGURE_COMMAND
        <SOURCE_DIR>/configure
            "--prefix=${DEPENDENCY_OUTPUT_DIRECTORY}"
            --enable-optimizations
            --disable-asm
            --disable-avdevice
            --disable-cuda
            --disable-cuvid
            --disable-debug
            --disable-stripping
            --disable-doc
            --disable-nvenc
            --disable-postproc
            --disable-programs
            --disable-schannel
            --disable-shared
            --disable-audiotoolbox
            --disable-d3d11va
            --disable-dxva2
            --enable-swresample
            --enable-runtime-cpudetect
            --enable-static
            --enable-w32threads
            --disable-protocols
            --disable-network
            --disable-devices
            --disable-muxers
            --disable-encoders
            --disable-pthreads
            --enable-decoders
            --enable-demuxers
            --arch=${FFMPEG_ARCH}
            --enable-cross-compile
            --cross-prefix=${TOOL_TRIPLET_PREFIX}
            --target-os=mingw64
    BUILD_COMMAND make -C <BINARY_DIR> ${MAKE_JOB_ARG} && make -C <BINARY_DIR> ${MAKE_JOB_ARG} install
    BUILD_BYPRODUCTS ${DEPENDENCY_OUTPUT_DIRECTORY}/lib/libavformat.a ${DEPENDENCY_OUTPUT_DIRECTORY}/lib/libavcodec.a ${DEPENDENCY_OUTPUT_DIRECTORY}/lib/libavfilter.a ${DEPENDENCY_OUTPUT_DIRECTORY}/lib/libswresample.a ${DEPENDENCY_OUTPUT_DIRECTORY}/lib/libswscale.a ${DEPENDENCY_OUTPUT_DIRECTORY}/lib/libavutil.a
    INSTALL_COMMAND ""
)
list(APPEND THIRDPARTY_DEPENDS ffmpeg-build)

ExternalProject_Add(fribidi-build
    URL https://github.com/fribidi/fribidi/releases/download/v1.0.11/fribidi-1.0.11.tar.xz
    URL_HASH SHA256=30f93e9c63ee627d1a2cedcf59ac34d45bf30240982f99e44c6e015466b4e73d
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    CONFIGURE_COMMAND ${CMAKE_COMMAND} -E env
        "PKG_CONFIG_PATH=${DEPENDENCY_OUTPUT_DIRECTORY}/lib/pkgconfig"
        <SOURCE_DIR>/configure
            CFLAGS=-O2
            "--prefix=${DEPENDENCY_OUTPUT_DIRECTORY}"
            --host=${TOOL_TRIPLET_PREFIX_NOTRAILINGDASH}
            --enable-static
            --disable-shared
            --disable-dependency-tracking
            --disable-debug
    BUILD_COMMAND make -C <BINARY_DIR> ${MAKE_JOB_ARG} && make -C <BINARY_DIR> ${MAKE_JOB_ARG} install
    BUILD_BYPRODUCTS ${DEPENDENCY_OUTPUT_DIRECTORY}/lib/libfribidi.a
    INSTALL_COMMAND ""
)
list(APPEND THIRDPARTY_DEPENDS fribidi-build)

ExternalProject_Add(brotli-build
    URL https://github.com/google/brotli/archive/refs/tags/v1.0.9.tar.gz
    URL_HASH SHA256=f9e8d81d0405ba66d181529af42a3354f838c939095ff99930da6aa9cdf6fe46
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    CONFIGURE_COMMAND 
        bash -c "cd <SOURCE_DIR> && NOCONFIGURE=1 ./bootstrap" &&
        ${CMAKE_COMMAND} -E env
        "PKG_CONFIG_PATH=${DEPENDENCY_OUTPUT_DIRECTORY}/lib/pkgconfig"
        <SOURCE_DIR>/configure
            CFLAGS=-O2
            CXXFLAGS=-O2
            "--prefix=${DEPENDENCY_OUTPUT_DIRECTORY}"
            --host=${TOOL_TRIPLET_PREFIX_NOTRAILINGDASH}
            --enable-static
            --disable-shared
            --disable-dependency-tracking
    BUILD_COMMAND make -C <BINARY_DIR> ${MAKE_JOB_ARG} && make -C <BINARY_DIR> ${MAKE_JOB_ARG} install
    BUILD_BYPRODUCTS ${DEPENDENCY_OUTPUT_DIRECTORY}/lib/libbrotlidec.a ${DEPENDENCY_OUTPUT_DIRECTORY}/lib/libbrotlicommon.a
    INSTALL_COMMAND ""
)
list(APPEND THIRDPARTY_DEPENDS brotli-build)

ExternalProject_Add(freetype-build-nohb
    URL https://download.savannah.gnu.org/releases/freetype/freetype-2.11.0.tar.xz
    URL_HASH SHA256=8bee39bd3968c4804b70614a0a3ad597299ad0e824bc8aad5ce8aaf48067bde7
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    CONFIGURE_COMMAND 
        ${CMAKE_COMMAND} -E env
        "PKG_CONFIG_PATH=${DEPENDENCY_OUTPUT_DIRECTORY_NOHB}/lib/pkgconfig"
        <SOURCE_DIR>/configure
            CFLAGS=-O2
            "--prefix=${DEPENDENCY_OUTPUT_DIRECTORY_NOHB}"
            --host=${TOOL_TRIPLET_PREFIX_NOTRAILINGDASH}
            --enable-static
            --disable-shared

            --without-brotli
            --without-zlib
            --without-bzip2
            --without-png
            --without-harfbuzz
    BUILD_COMMAND make -C <BINARY_DIR> ${MAKE_JOB_ARG} && make -C <BINARY_DIR> ${MAKE_JOB_ARG} install
    BUILD_BYPRODUCTS ${DEPENDENCY_OUTPUT_DIRECTORY_NOHB}/lib/libfreetype.a
    INSTALL_COMMAND ""
)
# Dependency on freetype-build-nohb not added to list

ExternalProject_Add(harfbuzz-build
    DEPENDS freetype-build-nohb
    URL https://github.com/harfbuzz/harfbuzz/releases/download/3.1.2/harfbuzz-3.1.2.tar.xz
    URL_HASH SHA256=4056b1541dd8bbd8ec29207fe30e568805c0705515632d7fec53a94399bc7945
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    CONFIGURE_COMMAND 
        ${CMAKE_COMMAND} -E env
        "PKG_CONFIG_PATH=${DEPENDENCY_OUTPUT_DIRECTORY_NOHB}/lib/pkgconfig"
        <SOURCE_DIR>/configure
            "CFLAGS=-O2 -DHB_NO_MT -DHB_NO_PRAGMA_GCC_DIAGNOSTIC_ERROR"
            "CXXFLAGS=-O2 -DHB_NO_MT -DHB_NO_PRAGMA_GCC_DIAGNOSTIC_ERROR"
            "--prefix=${DEPENDENCY_OUTPUT_DIRECTORY}"
            --host=${TOOL_TRIPLET_PREFIX_NOTRAILINGDASH}
            --enable-static
            --disable-shared
            --disable-dependency-tracking

            --without-cairo
            --without-fontconfig
            --without-icu
            --with-freetype
            --without-glib
    BUILD_COMMAND make -C <BINARY_DIR> ${MAKE_JOB_ARG} && make -C <BINARY_DIR> ${MAKE_JOB_ARG} install
    BUILD_BYPRODUCTS ${DEPENDENCY_OUTPUT_DIRECTORY}/lib/libharfbuzz.a
    INSTALL_COMMAND ""
)
list(APPEND THIRDPARTY_DEPENDS harfbuzz-build)

ExternalProject_Add(freetype-build
    DEPENDS harfbuzz-build brotli-build
    URL https://download.savannah.gnu.org/releases/freetype/freetype-2.11.0.tar.xz
    URL_HASH SHA256=8bee39bd3968c4804b70614a0a3ad597299ad0e824bc8aad5ce8aaf48067bde7
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    CONFIGURE_COMMAND 
        ${CMAKE_COMMAND} -E env
        "PKG_CONFIG_PATH=${DEPENDENCY_OUTPUT_DIRECTORY}/lib/pkgconfig:${DEPENDENCY_OUTPUT_DIRECTORY_NOHB}/lib/pkgconfig"
        <SOURCE_DIR>/configure
            CFLAGS=-O2
            "--prefix=${DEPENDENCY_OUTPUT_DIRECTORY}"
            --host=${TOOL_TRIPLET_PREFIX_NOTRAILINGDASH}
            --enable-static
            --disable-shared

            --with-brotli
            --without-zlib
            --without-bzip2
            --without-png
            --with-harfbuzz
    BUILD_COMMAND make -C <BINARY_DIR> ${MAKE_JOB_ARG} && make -C <BINARY_DIR> ${MAKE_JOB_ARG} install
    BUILD_BYPRODUCTS ${DEPENDENCY_OUTPUT_DIRECTORY}/lib/libfreetype.a
    INSTALL_COMMAND ""
)
list(APPEND THIRDPARTY_DEPENDS freetype-build)

ExternalProject_Add(libass-build
    DEPENDS fribidi-build freetype-build harfbuzz-build
    URL https://github.com/libass/libass/releases/download/0.15.2/libass-0.15.2.tar.xz
    URL_HASH SHA256=1be2df9c4485a57d78bb18c0a8ed157bc87a5a8dd48c661961c625cb112832fd
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    CONFIGURE_COMMAND 
        ${CMAKE_COMMAND} -E env
        "PKG_CONFIG_PATH=${DEPENDENCY_OUTPUT_DIRECTORY}/lib/pkgconfig"
        <SOURCE_DIR>/configure
            "CFLAGS=-DFRIBIDI_LIB_STATIC -O2"
            "--prefix=${DEPENDENCY_OUTPUT_DIRECTORY}"
            --host=${TOOL_TRIPLET_PREFIX_NOTRAILINGDASH}
            --disable-shared
            --enable-static
            --disable-asm

            --disable-require-system-font-provider
            --disable-directwrite
            --disable-fontconfig
    BUILD_COMMAND make -C <BINARY_DIR> ${MAKE_JOB_ARG} && make -C <BINARY_DIR> ${MAKE_JOB_ARG} install
    BUILD_BYPRODUCTS ${DEPENDENCY_OUTPUT_DIRECTORY}/lib/libass.a
    INSTALL_COMMAND ""
)
list(APPEND THIRDPARTY_DEPENDS libass-build)

ExternalProject_Add(mpv-build
    DEPENDS spirv-cross-build shaderc-build iconv-build zlib-build ffmpeg-build libass-build
    URL https://github.com/mpv-player/mpv/archive/refs/tags/v0.34.1.tar.gz
    URL_HASH SHA256=32ded8c13b6398310fa27767378193dc1db6d78b006b70dbcbd3123a1445e746
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    BUILD_IN_SOURCE TRUE
    PATCH_COMMAND 
        sed -i -e "s/bf971e98edc2414968a262c6aa6b88541a26c3cd248689c89f4c57370955ee7f/dcec3e179f9c33a66544f1b3d7d91f20f6373530510fa6a858cddb6bfdcde14b/" <SOURCE_DIR>/bootstrap.py &&
        sed -i -e "s/2.0.20/2.0.26/" <SOURCE_DIR>/bootstrap.py &&
        perl -i -p -e "s/\\x23include <ksguid.h>/\\x23include <mmreg.h>\\n\\x23include <ksguid.h>/" <SOURCE_DIR>/audio/out/ao_wasapi_utils.c
    CONFIGURE_COMMAND 
        python3.11 <SOURCE_DIR>/bootstrap.py &&
        ${CMAKE_COMMAND} -E env
        CC=${CMAKE_C_COMPILER}
        CXX=${CMAKE_CXX_COMPILER}
        AR=${CMAKE_AR}
        NM=${TOOL_TRIPLET_PREFIX}nm
        RANLIB=${CMAKE_RANLIB}
        "CFLAGS=-Wno-incompatible-pointer-types -I${DEPENDENCY_OUTPUT_DIRECTORY}/include -I${DEPENDENCY_OUTPUT_DIRECTORY}/include/spirv_cross"
        "LDFLAGS=-L${DEPENDENCY_OUTPUT_DIRECTORY}/lib"
        WINDRES=${CMAKE_RC_COMPILER}
        "LIBRARY_PATH=${DEPENDENCY_OUTPUT_DIRECTORY}"
        "PKG_CONFIG_PATH=${DEPENDENCY_OUTPUT_DIRECTORY}/lib/pkgconfig"
        "PKG_CONFIG_LIBDIR=${DEPENDENCY_OUTPUT_DIRECTORY}/lib/pkgconfig"
        "PKG_CONFIG_SYSROOT_DIR=${DEPENDENCY_OUTPUT_DIRECTORY}"
        python3.11 <SOURCE_DIR>/waf configure
            --enable-libmpv-static
            --enable-static-build
            --disable-lua
            --disable-cplayer
            --enable-lgpl
            --enable-d3d11
            ${MPV_ENABLEOPENGL}
            "--prefix=${DEPENDENCY_OUTPUT_DIRECTORY}"
    BUILD_COMMAND 
        ${CMAKE_COMMAND} -E env
        CC=${CMAKE_C_COMPILER}
        CXX=${CMAKE_CXX_COMPILER}
        AR=${CMAKE_AR}
        NM=${TOOL_TRIPLET_PREFIX}nm
        RANLIB=${CMAKE_RANLIB}
        "CFLAGS=-Wno-incompatible-pointer-types -I${DEPENDENCY_OUTPUT_DIRECTORY}/include -I${DEPENDENCY_OUTPUT_DIRECTORY}/include/spirv_cross"
        "LDFLAGS=-L${DEPENDENCY_OUTPUT_DIRECTORY}/lib"
        WINDRES=${CMAKE_RC_COMPILER}
        python3.11 <SOURCE_DIR>/waf build --verbose
            &&
        ${CMAKE_COMMAND} -E env
        CC=${CMAKE_C_COMPILER}
        CXX=${CMAKE_CXX_COMPILER}
        AR=${CMAKE_AR}
        NM=${TOOL_TRIPLET_PREFIX}nm
        RANLIB=${CMAKE_RANLIB}
        "CFLAGS=-Wno-incompatible-pointer-types -I${DEPENDENCY_OUTPUT_DIRECTORY}/include -I${DEPENDENCY_OUTPUT_DIRECTORY}/include/spirv_cross"
        "LDFLAGS=-L${DEPENDENCY_OUTPUT_DIRECTORY}/lib"
        WINDRES=${CMAKE_RC_COMPILER}
        python3.11 <SOURCE_DIR>/waf install
    BUILD_BYPRODUCTS ${DEPENDENCY_OUTPUT_DIRECTORY}/lib/libmpv.a
    INSTALL_COMMAND ""
)
list(APPEND THIRDPARTY_DEPENDS mpv-build)

foreach(CUR_DEPEND IN LISTS THIRDPARTY_DEPENDS)
ExternalProject_Get_Property(${CUR_DEPEND} BUILD_BYPRODUCTS)
list(APPEND THIRDPARTY_LIBRARIES ${BUILD_BYPRODUCTS})
endforeach()

add_custom_command(
    OUTPUT ${DEPENDENCY_OUTPUT_DIRECTORY}/lib/libthird_party_cmake.a
    COMMAND 
        printf 'create %s\\n' ${DEPENDENCY_OUTPUT_DIRECTORY}/lib/libthird_party_cmake.a > ${DEPENDENCY_OUTPUT_DIRECTORY}/build.mri &&
        printf 'addlib %s\\n' ${THIRDPARTY_LIBRARIES} >> ${DEPENDENCY_OUTPUT_DIRECTORY}/build.mri &&
        printf 'save\\nend\\n' >> ${DEPENDENCY_OUTPUT_DIRECTORY}/build.mri &&
        cat ${DEPENDENCY_OUTPUT_DIRECTORY}/build.mri | ${CMAKE_AR} -M
    DEPENDS ${THIRDPARTY_DEPENDS}
)

add_custom_command(
    OUTPUT ${DEPENDENCY_OUTPUT_DIRECTORY}/include/third_party_cmake.h
    COMMAND touch ${DEPENDENCY_OUTPUT_DIRECTORY}/include/third_party_cmake.h
    DEPENDS ${DEPENDENCY_OUTPUT_DIRECTORY}/lib/libthird_party_cmake.a
)

add_custom_target(main_output_target
    ALL
    DEPENDS ${DEPENDENCY_OUTPUT_DIRECTORY}/lib/libthird_party_cmake.a ${DEPENDENCY_OUTPUT_DIRECTORY}/include/third_party_cmake.h
)
