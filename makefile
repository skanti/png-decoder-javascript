INCLUDES:=-I/home/amon/software/emsdk/emscripten/1.37.36/system/include -I./src -I./src/png
PNG_SRC:= \
	src/png/png.c \
	src/png/pngerror.c \
	src/png/pngget.c \
	src/png/pngmem.c \
	src/png/pngpread.c \
	src/png/pngread.c \
	src/png/pngrio.c \
	src/png/pngrtran.c \
	src/png/pngrutil.c \
	src/png/pngset.c \
	src/png/pngtrans.c \
	src/png/pngwio.c \
	src/png/pngwrite.c \
	src/png/pngwtran.c \
	src/png/pngwutil.c
PNG_OBJ:= $(PNG_SRC:.c=.o)

Z_SRC:= \
    src/zlib/zlib.h \
    src/zlib/crc32.h \
    src/zlib/deflate.h \
    src/zlib/gzguts.h \
    src/zlib/inffast.h \
    src/zlib/inffixed.h \
    src/zlib/inflate.h \
    src/zlib/inftrees.h \
    src/zlib/trees.h \
    src/zlib/zutil.h \
    src/zlib/adler32.c \
    src/zlib/compress.c \
    src/zlib/crc32.c \
    src/zlib/deflate.c \
    src/zlib/gzclose.c \
    src/zlib/gzlib.c \
    src/zlib/gzread.c \
    src/zlib/gzwrite.c \
    src/zlib/inflate.c \
    src/zlib/infback.c \
    src/zlib/inftrees.c \
    src/zlib/inffast.c \
    src/zlib/trees.c \
    src/zlib/uncompr.c \
    src/zlib/zutil.c 

Z_OBJ:= $(Z_SRC:.c=.o)

all : LoaderPNG.js 


LoaderPNG.js : LoaderPNG.cpp  src/libpng.a
	em++ -L./src/zlib -L./src --bind $(INCLUDES) --memory-init-file 0 -s MODULARIZE=1 -s ALLOW_MEMORY_GROWTH=1 -std=c++14 -DNDEBUG -DUSE_ZLIB=1 -o $@ $< -lpng -lz

%.o : %.c
	emcc --bind $(INCLUDES)  -DNDEBUG -D_LARGEFILE64_SOURCE=1 -o $@ -c $^

src/libpng.a : $(PNG_OBJ)
	emar rc $@ $^

# TO COMPILE ZLIB:
# - GoTo src/zlib
# - emconfigure ./cofigure

#src/libz.a : $(Z_OBJ)
#	emar rvs $@ $^

clean:
	rm -f LoaderPNG.js ./src/*.a
	#rm -f LoaderPNG.js ./src/*.a ./src/png/*.o ./src/zlib/*.o
