MINGW_DIR = /usr/local/lib/gcc/i686-pc-mingw32/4.4.1

CXX = i686-pc-mingw32-g++
FILES = *.cpp
LIBS_DIRS = -L$(MINGW_DIR)
STATIC_LIBS = -lftgl -lglu32 -lfreetype
DYNAMIC_LIBS = -lSDL -lopengl32
INCLUDE_DIRS = -I$(MINGW_DIR)/include/ -I$(MINGW_DIR)/include/freetype2/
OUT = program.exe

CXXCOMPILE = $(CXX) $(FILES) -Wl,-Bstatic $(STATIC_LIBS) -Wl,-Bdynamic $(DYNAMIC_LIBS) $(LIBS_DIRS) $(INCLUDE_DIRS) -D __WIN32__ -o $(OUT)

all:
	$(CXXCOMPILE)
clean:
	rm $(OUT)
