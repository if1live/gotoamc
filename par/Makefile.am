bin_PROGRAMS	= amc
amc_SOURCES	= main.cpp frame.cpp context.cpp frameHeap.cpp videoIO.cpp textFrame.cpp  textFrame2PPM.cpp frame2TextFrame.cpp
amc_CXXFLAGS	= -lpthread -lavutil -g @CXXFLAGS@ -I/usr/include/ffmpeg -I/usr/include/ffmpeg/libavcodec -I/usr/include/ffmpeg/libavformat -I/usr/include/libavcodec/ -I/usr/include/libavformat 

clean :
	rm -rf *.o amc
	@echo "cleaning"
