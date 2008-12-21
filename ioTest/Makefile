CXX	= g++
INC_PATH = -I/usr/include/ffmpeg/libavcodec -I/usr/include/ffmpeg/libavformat -I/usr/include/ffmpeg#-I/homes/pp/u2007141097/xeon-share/include -I/homes/pp/u2007141097/xeon-share/include/libavcodec -I/homes/pp/u2007141097/xeon-share/include/libavformat -I/usr/include -I/usr/include/ffmpeg/ -I/usr/include/libavcodec -I/usr/include/libavformat -I/usr/include/ffmpeg/libavcodec/ -I/usr/include/ffmpeg/libavformat
LIB_PATH = #-L/homes/pp/u2007141097/xeon-share/lib
LDFLAGS = -laa -lavformat -lavcodec 
CXXFLAGS = -g 

TARGET	= amc

all: $(TARGET)

$(TARGET): main.o frame.o context.o frameHeap.o videoIO.o 
	$(CXX) $(INC_PATH) $(LIB_PATH) -o $@ $^ $(LDFLAGS)

main.o: main.cpp
	$(CXX) $(INC_PATH) $(LIB_PATH) -c $^ $(CXXFLAGS)

frame.o: frame.cpp
	$(CXX) $(INC_PATH) $(LIB_PATH) -c $^ $(CXXFLAGS)

context.o: context.cpp videoIO.o 
	$(CXX) $(INC_PATH) $(LIB_PATH) -c $^ $(CXXFLAGS)

frameHeap.o: frameHeap.cpp frame.o
	$(CXX) $(INC_PATH) $(LIB_PATH) -c $^ $(CXXFLAGS)

videoIO.o: videoIO.cpp frame.o
	$(CXX) $(INC_PATH) $(LIB_PATH) -c $^ $(CXXFLAGS) 

clean:
	rm *.o $(TARGET)
