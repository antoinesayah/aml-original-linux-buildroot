
FILE_DIR=ffmpeg

DEPENDS=zlib-1.2.3

CONFIG_FLAGS= --prefix=${PREFIX} --disable-static --enable-shared --disable-ffplay --disable-ffserver --disable-doc \
    --disable-mpegaudio-hp --disable-encoders --disable-decoder=h264 --disable-muxers --disable-filters --enable-cross-compile \
    --cross-prefix=arc-linux-uclibc- --extra-ldflags=-L${PREFIX}/lib --arch=arc --disable-altivec --disable-amd3dnow \
    --disable-amd3dnowext --disable-mmx --disable-mmx2 --disable-sse --disable-ssse3 --disable-armv5te --disable-armv6 \
    --disable-armv6t2 --disable-armvfp --disable-iwmmxt --disable-mmi --disable-neon --disable-vis --disable-yasm --enable-pic \


ifdef DEBUG
CONFIG_FLAGS+=    --enable-debug --disable-stripping
endif    

TAR_DIR=${PKG_BUILD_DIR}/${FILE_DIR}

P_FILE_F=${PKG_DIR}/${P_FILE}
BUILD_DIR=${PKG_BUILD_DIR}/${FILE_DIR}

.PHONY:configure all config install clean distclean


all:before_cmd config
	make -C ${BUILD_DIR}
	make install

configure:config

config:get_file
	cd ${BUILD_DIR} && ${PKG_TOP_DIR}/${FILE_DIR}/src/configure  ${CONFIG_FLAGS}

get_file:
	@if [ ! -d "${TAR_DIR}" ]; then \
		mkdir ${TAR_DIR}; \
	fi;
	##cp -rf ${PKG_TOP_DIR}/${FILE_DIR}/src/* ${PKG_BUILD_DIR}/${FILE_DIR};

install:
	make -C ${BUILD_DIR} install

distclean:clean

clean:
	-rm ${PREFIX}/lib/libav*;\
	make -C $(BUILD_DIR) clean
#	rm $(BUILD_DIR)/*.o;

before_cmd:depends

depends:
	@if [   "${DEPENDS}" != "" ]; then \
                cd ${PKG_TOP_DIR};make lib_depends TOP_DEPENDS="${DEPENDS}";    \
        fi


#include $(MAKEFILES_DIR)/package.rules
