OBJS = \
    glToy.o \
    utils.o \
    Listener.o \
    Program.o \
    ScreenRenderPass.o \
    TextureRenderPass.o \
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

ifdef USE_GL3W
    OBJS += gl3w.o
endif

TARGET = glToy

SRC_DIR = src
BUILD_DIR = build

####
BUILD_OBJS = $(patsubst %,$(BUILD_DIR)/%,$(OBJS))

UNAME := $(shell uname)

INCLUDES = -I$(SRC_DIR)/include/
LIBS = -lGLU -lglut

CFLAGS = -O2 -g -Wall -fmessage-length=0

ifdef USE_GL3W
    CFLAGS += -DUSE_GL3W
endif

CXXFLAGS = $(CFLAGS) -std=c++0x

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

$(TARGET): $(BUILD_OBJS)
	$(CXX) -o $(TARGET) $^ $(LIBS)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<
	$(CC) $(CFLAGS) $(INCLUDES) -MM -MP -MT $@ -MF $(@:%.o=%.d) $<

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MM -MP -MT $@ -MF $(@:%.o=%.d) $<

clean:
	rm -f $(BUILD_OBJS) $(BUILD_OBJS:%.o=%.d) $(TARGET)

-include $(BUILD_OBJS:%.o=%.d)
