OBJS = \
    glToy.o \
    utils.o \
    Listener.o \
    Program.o \
    ScreenRenderPass.o \
    TextureRenderPass.o \
    TextureRenderer.o \
    ofxMSAFluidSolver.o \
    CellNoiseFluid.o \
    CellNoiseScene.o \
    Graph.o \
    Visitor.o \
    SceneRenderVisitor.o \
    Noise.o \
    Anemone.o \
    Bubbles.o \
    BubblesScene.o

TARGET = glToy

SRC_DIR = src
BUILD_DIR = build

####

OS := $(shell uname -s)

INCLUDES = -I$(SRC_DIR)/include/
LIBS = -lGLU -lglut

CFLAGS = -O3 -Wall -fmessage-length=0
LDFLAGS =

ifeq ($(OS),CYGWIN_NT-5.1)
    FREEGLUT_DIR = "C:\MinGW\freeglut"
    BOOST_DIR = "C:\Program Files\boost_1_44_0"

    CFLAGS += -D_WIN32
    INCLUDES += -I$(FREEGLUT_DIR)/include -I$(BOOST_DIR)

    OBJS += gl3w.o
    LIBS = -lglut32 -lglu32 -lopengl32
endif

CXXFLAGS = $(CFLAGS) -std=c++0x

ifeq ($(OS),Darwin)
    CXXFLAGS += -I/opt/local/include
    LIBS = -framework GLUT -framework OpenGL -framework Cocoa \
           -L/opt/local/lib -lGLEW
endif

BUILD_OBJS = $(patsubst %,$(BUILD_DIR)/%,$(OBJS))

####

all: $(TARGET)

$(TARGET): $(BUILD_OBJS)
	$(CXX) $(LDFLAGS) $(LIBS) -o $(TARGET) $^

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<
	$(CC) $(CFLAGS) $(INCLUDES) -MM -MP -MT $@ -MF $(@:%.o=%.d) $<

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MM -MP -MT $@ -MF $(@:%.o=%.d) $<

clean:
	rm -f $(BUILD_OBJS) $(BUILD_OBJS:%.o=%.d) $(TARGET) gmon.out

-include $(BUILD_OBJS:%.o=%.d)
