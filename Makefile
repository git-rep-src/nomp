CC = g++
CFLAGS = $(shell pkg-config libxml++-3.0 --cflags) -std=c++11 -Wall
LIBS = $(shell pkg-config libxml++-3.0 --libs) -pthread -lcrypto -lssl -lform -lncurses
SOURCEDIR = src
BUILDDIR = .build
SOURCES = $(wildcard src/*.cc)
OBJECTS = $(patsubst $(SOURCEDIR)/%.cc,$(BUILDDIR)/%.o,$(SOURCES))
DEPS := $(OBJECTS:.o=.d)
TARGET = nomp

.SILENT:

all: build $(BUILDDIR)/$(TARGET)

build:
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@ $(LIBS)

-include $(DEPS)

$(OBJECTS): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.cc
	$(CC) $(CFLAGS) -MMD -c $< -o $@

clean:
	rm -f $(BUILDDIR)/*
