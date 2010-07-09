CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		glToy.o utils.o Particle.o

LIBS =	 	-lGLEW -lGLU -lglut

TARGET =	glToy

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
