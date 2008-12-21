CC	= ppu-gcc
CXX	= ppu-g++
SCXX = spu-g++
INC_PATH = -I/homes/pp/u2007141097/ps3-share/include -I/homes/pp/u2007141097/ps3-share/include/libavcodec -I/homes/pp/u2007141097/ps3-share/include/libavformat
LIB_PATH = -L/usr/lib
LDFLAGS = -laa -lpthread -lavformat -lavcodec -m32
CXXFLAGS = -g -m32
EMBEDSPU = ppu-embedspu -m32

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

	#TODO
#textFrame2PPM_spe.o: textFrame2PPM_spe.cpp
#	$(CXX) $(INC_PATH) $(LIB_PATH) -c $^ 

videoIO_spe1 : videoIO_spe1.cpp
	$(SCXX) $(INC_PATH) $(LIB_PATH) -o $@ $^

videoIO_spe1.o: videoIO_spe1
	$(EMBEDSPU) videoIO_spe1_handle videoIO_spe1 videoIO_spe1.o

frame.o: frame.cpp
	$(CXX) $(INC_PATH) $(LIB_PATH) -c $^ $(CXXFLAGS)

context.o: context.cpp videoIO.o textFrame.o
	$(CXX) $(INC_PATH) $(LIB_PATH) -c $^ $(CXXFLAGS)

frame2TextFrame.o: frame2TextFrame.cpp frame.o textFrame.o
	$(CXX) $(INC_PATH) $(LIB_PATH) -c $^ $(CXXFLAGS)

frameHeap.o: frameHeap.cpp frame.o
	$(CXX) $(INC_PATH) $(LIB_PATH) -c $^ $(CXXFLAGS)

videoIO.o: videoIO.cpp videoIO_spe1.o frame.o
	$(CXX) $(INC_PATH) $(LIB_PATH) -c $^ $(CXXFLAGS)

clean:
	rm *.o $(TARGET)
