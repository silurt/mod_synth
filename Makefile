CC := gcc
SRCDIR := src
BUILDDIR := build
TARGET := bin/run
SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
DIRS := $(dir $(OBJECTS))
CFLAGS := -std=c99 -Wall -Wextra -pedantic -Wno-misleading-indentation -Wno-shift-negative-value -O2
LIB := -L/opt/homebrew/lib -lportaudio
INC := -I/opt/homebrew/include -I include

all: directories $(TARGET)

directories: $(DIRS)

$(DIRS):
	mkdir -p $@

$(TARGET): $(OBJECTS)
	@echo " Linking... 🌟"
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB) -O3

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@echo " Building... 🛠️"
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $< -O3

clean:
	@echo " Cleaning up... 🧹✨"
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean

watch:
	while true; do \
		$(MAKE) -q || ($(MAKE) && echo "\n Running...\n --------------------------- " && ./$(TARGET)); \
		sleep 0.5; \
	done
