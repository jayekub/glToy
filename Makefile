OBJS = glToy.o utils.o Particle.o ParticleSystem.o FluidParticleSystem.o \
	   Program.o RenderPass.o ScreenRenderPass.o TextureRenderPass.o \
	   Renderer.o CellNoiseRenderer.o FractalRenderer.o ofxMSAFluidSolver.o
TARGET = glToy

UNAME := $(shell uname)

INCLUDES = -Iinclude/
LIBS = -lGLEW -lGLU -lglut

CXXFLAGS =	-O2 -g -Wall -fmessage-length=0 $(INCLUDES)

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
