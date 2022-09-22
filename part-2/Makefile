PROGRAM = Raycast

CC := g++
CXXFLAGS=-std=c++11
SRCDIR := src
BUILDDIR := build
TARGET := bin
PLATFORM ?= win

SOURCES := $(shell find $(SRCDIR) -type f -name *.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%, $(SOURCES:.cpp=.o ))
LIBS := -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
WIN_INCLUDES := -IC:\mingw_dev_lib\include\SDL2 -LC:\mingw_dev_lib\lib -w -Wl,-subsystem,windows
OSX_INCLUDES := -I/usr/local/Cellar/ # Cellar is where brew installs libs
INC := -I include
RM_R := rm -r
RM := rm
OUTDIR := $(TARGET)/$(PROGRAM)

all: $(OUTDIR)

$(OUTDIR): $(OBJECTS)
	@echo " Linking...";
	@mkdir -p $(TARGET);
ifeq ($(PLATFORM),osx)
	$(CC) $(CXXFLAGS) $(OSX_INCLUDES) $^ -o $(OUTDIR) $(LIBS)
endif
ifeq ($(PLATFORM),win)
	$(CC) $(CXXFLAGS) $(WIN_INCLUDES) $^ -o $(OUTDIR) $(LIBS)
endif

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR);
ifeq ($(PLATFORM),osx)
	$(CC) $(CXXFLAGS) $(OSX_INCLUDES) $(INC) -c -g -o $@ $<
endif
ifeq ($(PLATFORM),win)
	$(CC) $(CXXFLAGS) $(WIN_INCLUDES) $(INC) -c -g -o $@ $<
endif

clean:
	@echo " Cleaning...";
	$(RM_R) $(BUILDDIR)
	$(RM) $(OUTDIR)