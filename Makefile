# Linux Makefile for Computer Graphics course 2016
#
# GLM installation is included in include/linux/glm.
# Other libraries have to be found from the system.
#
TARGET_DIR = bin/linux
TARGET = $(TARGET_DIR)/cg-2016
TARGET_D = $(TARGET_DIR)/cg-2016-debug
ZIPFILE = cg-sources.zip
SRCDIR = cg-sources
OBJDIR = build/linux/release
OBJDIR_D = build/linux/debug
CPP = g++
CPP_OPTS = -g -O3 -Wall `pkg-config --cflags sdl2` `pkg-config --cflags SDL2_image` -Iinclude/linux
CPP_OPTS_D = -g -O0 -Wall `pkg-config --cflags sdl2` `pkg-config --cflags SDL2_image` -Iinclude/linux
LINKER = g++
LINKER_OPTS =
LINKER_OPTS_D =
LINKER_LIBRARIES = `pkg-config --libs sdl2` `pkg-config --libs SDL2_image` -lGLEW -lGL -lm
SOURCES = $(shell find $(SRCDIR) -type f -name *.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.cpp=.o))
OBJECTS_D = $(patsubst $(SRCDIR)/%,$(OBJDIR_D)/%,$(SOURCES:.cpp=.o))
DEPS = make.dep

help::
	@echo "Computer Graphics 2016 Makefile help"
	@echo "------------------------------------"
	@echo "Run \"make debug\" in this directory to create a debug-build"
	@echo "Run \"make release\" in this directory to create a release-build"
	@echo "Run \"make run\" in this directory to create a release-build and run it"
	@echo "Run \"make gdb\" in this directory to create a debug-build and run it inside gdb"
	@echo "Run \"make valgrind\" in this directory to create a debug-build and run it inside valgrind"
	@echo "Run \"make zip\" in this directory to create a compressed file '$(ZIPFILE)' of '$(SRCDIR)' suitable for submission"

debug: $(TARGET_D)
	@echo "Done. To run debug build: cd $(SRCDIR); ../$(TARGET_D)"

release: $(TARGET)
	@echo "Done. To run release build: cd $(SRCDIR); ../$(TARGET)"

run:: $(TARGET)
	@echo "Running release-build.."
	cd $(SRCDIR); ../$(TARGET)

gdb:: $(TARGET_D)
	@echo "Running with gdb.."
	cd $(SRCDIR); gdb ../$(TARGET_D)

valgrind:: $(TARGET_D)
	@echo "Running with valgrind.."
	cd $(SRCDIR); valgrind --leak-check=full --track-origins=yes ../$(TARGET_D)

zip::
	@echo "Creating $(ZIPFILE).."
	@rm -f $(ZIPFILE)
	@zip -r -9 $(ZIPFILE) $(SRCDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p `dirname $@`
	$(CPP) $(CPP_OPTS) -c -o $@ $<

$(OBJDIR_D)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p `dirname $@`
	$(CPP) $(CPP_OPTS_D) -c -o $@ $<

$(DEPS): $(SOURCES)
	$(CPP) -c -MM $(SOURCES) > $@

clean::
	rm -f $(OBJECTS) $(OBJECTS_D) $(TARGET) $(DEPS)

$(TARGET): $(OBJECTS)
	@mkdir -p `dirname $@`
	$(LINKER) $(LINKER_OPTS) -o $@ $^ $(LINKER_LIBRARIES)

$(TARGET_D): $(OBJECTS_D)
	@mkdir -p `dirname $@`
	$(LINKER) $(LINKER_OPTS_D) -o $@ $^ $(LINKER_LIBRARIES)

-include $(DEPS)
