OBJS = \
	glToy.o \
	utils.o \
	Particle.o \
	ParticleSystem.o \
	FluidParticleSystem.o \
	Program.o \
	RenderPass.o \
	ScreenRenderPass.o \
	TextureRenderPass.o \
	CellNoiseRenderer.o \
	CombineRenderer.o \
	ofxMSAFluidSolver.o

TARGET = glToy

UNAME := $(shell uname)

INCLUDES = -Iinclude/
LIBS = -lGLEW -lGLU -lglut

CXXFLAGS = -O2 -g -Wall -fmessage-length=0

ifeq ($(UNAME),Darwin)
    CXXFLAGS += -I/opt/local/include
    LIBS = -framework GLUT \
           -framework OpenGL \
           -framework Cocoa \
           -L/opt/local/lib -lGLEW
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS) $(OBJS:%.o=%.d) $(TARGET)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MM -MP -MD $<

-include $(OBJS:%.o=%.d)
