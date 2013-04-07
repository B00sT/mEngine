CXX = g++
FILES = main.cpp\
	../mEngine/mEngine.cpp\
	../mEngine/mFont.cpp
STATIC_LIBS = -lftgl -lGLU -lfreetype
DYNAMIC_LIBS = -lSDL -lGL
INCLUDE_DIRS = -I../ -I/usr/local/include/freetype2/
OUT = program
CXXCOMPILE = $(CXX) $(FILES) -static-libgcc -static-libstdc++ -Wl,-Bstatic $(STATIC_LIBS) -Wl,-Bdynamic $(DYNAMIC_LIBS) $(INCLUDE_DIRS) -o $(OUT)

all:
	$(CXXCOMPILE)
clean:
	rm $(OUT)
