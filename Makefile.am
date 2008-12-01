bin_PROGRAMS	= amc
amc_SOURCES	= main.cpp frame.cpp frameHeap.cpp frameQueue.cpp frameStack.cpp videoIO.cpp textFrame.cpp textFrameQueue.cpp
amc_CXXFLAGS	= -lz -lavutil -lm @CXXFLAGS@ -lavutil -I/usr/include/ffmpeg -I/usr/include/ffmpeg/libavcodec -I/usr/include/ffmpeg/libavformat -I/usr/include/libavcodec/ -I/usr/include/libavformat 

clean :
	rm -rf *.o amc
	@echo "cleaning"
