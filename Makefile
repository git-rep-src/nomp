CC = g++
CFLAGS = $(shell pkg-config libxml++-2.6 --cflags) -std=c++11 -Wno-deprecated-declarations -Wall
LIBS = $(shell pkg-config libxml++-2.6 --libs) -lform -lncurses
SOURCEDIR = src
BUILDDIR = .build
SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))
DEPS := $(OBJECTS:.o=.d)
TARGET = nomp

.SILENT:

all: build $(BUILDDIR)/$(TARGET)

build:
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@ $(LIBS)

-include $(DEPS)

$(OBJECTS): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.cpp
	$(CC) $(CFLAGS) -MMD -c $< -o $@

clean:
	rm -f $(BUILDDIR)/*
