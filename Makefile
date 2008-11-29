CC	= gcc
CXX	= g++
CXXFLAGS= -g -W -Wall
LDFLAGS = -lavformat -lavcodec -lz -lm -lswscale
INC	= -I/usr/include/ffmpeg/

TARGET	= amc

all: $(TARGET)
	

$(TARGET): frameHeap.o frameQueue.o main.o frame.o videoIO.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(INC)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^ $(LDFLAGS) $(INC)

frame.o: frame.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^ $(LDFLAGS) $(INC)

frameHeap.o: frameHeap.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^ $(LDFLAGS) $(INC)

frameQueue.o: frameQueue.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^ $(LDFLAGS) $(INC)

videoIO.o: videoIO.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^ $(LDFLAGS) $(INC)

clean:
	rm *.o $(TARGET) *.ppm
