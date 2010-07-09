OBJS =glToy.o utils.o Particle.o
TARGET = glToy

UNAME := $(shell uname)

CXXFLAGS =	-O2 -g -Wall -fmessage-length=0
LIBS = -lGLEW -lGLU -lglut

ifeq ($(UNAME),Darwin)
    CXXFLAGS += -I/opt/local/include
    LIBS = -framework GLUT \
           -framework OpenGL \
           -framework Cocoa \
           -L/opt/local/lib -lGLEW
endif

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
