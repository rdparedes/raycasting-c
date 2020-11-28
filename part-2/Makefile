PROGRAM = Raycast

CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := bin

SOURCES := $(shell find $(SRCDIR) -type f -name *.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%, $(SOURCES:.cpp=.o ))
LIBS := -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
WIN_INCLUDES := -IC:\mingw_dev_lib\include\SDL2 -LC:\mingw_dev_lib\lib -w -Wl,-subsystem,windows
INC := -I include
RM_R := rm -r

all: $(TARGET)/$(PROGRAM)

$(TARGET)/$(PROGRAM): $(OBJECTS)
	@echo " Linking...";
	@mkdir -p $(TARGET);
	$(CC) $(WIN_INCLUDES) $^ -o $(TARGET)/$(PROGRAM) $(LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR);
	$(CC) $(WIN_INCLUDES) $(INC) -c -g -o $@ $<

clean:
	@echo " Cleaning...";
	$(RM_R) $(BUILDDIR) $(TARGET)