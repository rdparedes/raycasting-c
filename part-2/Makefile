PROGRAM = Raycast

CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := bin

SOURCES := $(shell find $(SRCDIR) -type f -name *.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%, $(SOURCES:.cpp=.o ))
LIBS := -lSDL2 -lSDL2_image
INC := -I include

all: $(TARGET)/$(PROGRAM)

$(TARGET)/$(PROGRAM): $(OBJECTS)
	@echo " Linking...";
	@mkdir -p $(TARGET);
	$(CC) $^ -o $(TARGET)/$(PROGRAM) $(LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR);
	$(CC) $(INC) -c -g -o $@ $<

RM_R := rm -r

clean:
	@echo " Cleaning...";
	$(RM_R) $(BUILDDIR) $(TARGET)