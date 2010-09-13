OBJS = \
	glToy.o \
	utils.o \
	Listener.o \
	Program.o \
	ScreenRenderPass.o \
	TextureRenderPass.o \
	DepthRenderPass.o \
	CellNoiseRenderer.o \
	TextureRenderer.o \
	Graph.o \
	Visitor.o \
	SceneRenderVisitor.o \
	Noise.o \
	Anemone.o \
	Bubbles.o \
	BubblesScene.o \
	ofxMSAFluidSolver.o

TARGET = glToy

UNAME := $(shell uname)

INCLUDES = -Iinclude/
LIBS = -lGLEW -lGLU -lglut

CXXFLAGS = -O2 -g -Wall -fmessage-length=0 -std=c++0x

ifeq ($(UNAME),Darwin)
    CXXFLAGS += -I/opt/local/include
    LIBS = -framework GLUT \
           -framework OpenGL \
           -framework Cocoa \
           -L/opt/local/lib -lGLEW
endif

ifeq ($(UNAME),CYGWIN_NT-5.1)
   LIBS = -lglut32 \
          -lglu32 \
          -lopengl32
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
