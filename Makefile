CC := gcc
SRCDIR := src
BUILDDIR := build
TARGETDIR := bin
TARGET := $(TARGETDIR)/run
MODDIR := $(TARGETDIR)/std_mods
SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
DIRS := $(dir $(OBJECTS)) $(MODDIR)
STDMODSDIR := std_mods
STDMODSSOURCES := $(shell find $(STDMODSDIR) -type f -name mod.$(SRCEXT))
STDMODSOBJECTS := $(patsubst $(STDMODSDIR)/%/mod.$(SRCEXT),$(BUILDDIR)/%.o,$(STDMODSSOURCES))
STDMODSLIBS := $(patsubst $(STDMODSDIR)/%/mod.$(SRCEXT),$(MODDIR)/%.so,$(STDMODSSOURCES))
CFLAGS := -std=c99 -Wall -Wextra -pedantic -Wno-misleading-indentation -Wno-shift-negative-value -O2
LIB :=
INC :=

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	LIB := -L/opt/homebrew/lib -lportaudio
	INC := -I/opt/homebrew/include -I include
	DYLIB_EXT := .dylib
	CCFLAGS += -dynamiclib
else
	LIB := -lportaudio
	INC := -I include
	DYLIB_EXT := .so
	CCFLAGS += -shared
endif

STDMODSLIBS := $(patsubst $(STDMODSDIR)/%/mod.$(SRCEXT),$(MODDIR)/%$(DYLIB_EXT),$(STDMODSSOURCES))


all: directories $(STDMODSLIBS) $(TARGET)

directories: $(DIRS)

$(DIRS):
	mkdir -p $@

$(TARGET): $(OBJECTS)
	@echo "Linking... 🌟"
	$(CC) $^ -o $(TARGET) $(LIB) -lm

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@echo "Building... 🛠️"
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(BUILDDIR)/%.o: $(STDMODSDIR)/%/mod.$(SRCEXT)
	@echo "Building std_mods... 🛠️"
	$(CC) $(CFLAGS) $(INC) -fPIC -c -o $@ $<

$(MODDIR)/%$(DYLIB_EXT): $(BUILDDIR)/%.o
	@echo "Building dynamic library $@... 📚"
	$(CC) $(CFLAGS) $(CCFLAGS) -o $@ $< $(LIB)

clean:
	@echo "Cleaning up... 🧹✨"
	$(RM) -r $(BUILDDIR) $(TARGETDIR)

.PHONY: clean all directories
