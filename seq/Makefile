CXX	= g++
INC_PATH = -I/homes/pp/u2007141097/xeon-share/include -I/homes/pp/u2007141097/xeon-share/include/libavcodec -I/homes/pp/u2007141097/xeon-share/include/libavformat -I/usr/include -I/usr/include/ffmpeg/ -I/usr/include/libavcodec -I/usr/include/libavformat -I/usr/include/ffmpeg/libavcodec/ -I/usr/include/ffmpeg/libavformat
LIB_PATH = -L/homes/pp/u2007141097/xeon-share/lib
LDFLAGS = -laa -lavformat -lavcodec 
CXXFLAGS = -g 

TARGET	= amc

all: $(TARGET)

$(TARGET): main.o textFrame.o textFrame2PPM.o frame.o context.o frame2TextFrame.o frameHeap.o videoIO.o 
	$(CXX) $(INC_PATH) $(LIB_PATH) -o $@ $^ $(LDFLAGS)

main.o: main.cpp
	$(CXX) $(INC_PATH) $(LIB_PATH) -c $^ $(CXXFLAGS)

textFrame.o: textFrame.cpp
	$(CXX) $(INC_PATH) $(LIB_PATH) -c $^ $(CXXFLAGS)

textFrame2PPM.o: textFrame2PPM.cpp textFrame.o frame.o
	$(CXX) $(INC_PATH) $(LIB_PATH) -c $^ $(CXXFLAGS)

frame.o: frame.cpp
	$(CXX) $(INC_PATH) $(LIB_PATH) -c $^ $(CXXFLAGS)

context.o: context.cpp videoIO.o textFrame.o
	$(CXX) $(INC_PATH) $(LIB_PATH) -c $^ $(CXXFLAGS)

frame2TextFrame.o: frame2TextFrame.cpp frame.o textFrame.o
	$(CXX) $(INC_PATH) $(LIB_PATH) -c $^ $(CXXFLAGS)

frameHeap.o: frameHeap.cpp frame.o
	$(CXX) $(INC_PATH) $(LIB_PATH) -c $^ $(CXXFLAGS)

videoIO.o: videoIO.cpp frame.o
	$(CXX) $(INC_PATH) $(LIB_PATH) -c $^ $(CXXFLAGS) -lspe2

clean:
	rm *.o $(TARGET)
