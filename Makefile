CC = gcc
CFLAGS = -Wall
LIBS = -lform -lncurses
SOURCEDIR = src
BUILDDIR = .build
SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst $(SOURCEDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))
DEPS := $(OBJECTS:.o=.d)
TARGET = nomp

.SILENT:

all: build $(BUILDDIR)/$(TARGET)

build:
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@ $(LIBS)

-include $(DEPS)

$(OBJECTS): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.c
	$(CC) $(CFLAGS) -MMD -c $< -o $@

clean:
	rm -f $(BUILDDIR)/*
