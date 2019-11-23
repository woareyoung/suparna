LIB_PATH = E:/ffmpeg/ffmpeg/lib
CC = gcc
CCFLAGS = -Wall -g
CXX = g++
CXXFLAGS = $(CCFLAGS) -std=c++17
COMPILE_TYPE = debug
OUTPUT_DIR = ../$(COMPILE_TYPE)
INCLUDE_PATH = E:/ffmpeg/ffmpeg/include

TARGET = $(OUTPUT_DIR)/Suparna

HEADERS = io/io.h \
		  base/sbreak.h \
		  base/sjoin.h \
		  base/smerge.h \
		  base/sspilt.h 

base_objects = $(OUTPUT_DIR)/base/sbreak.o $(OUTPUT_DIR)/base/sjoin.o \
				$(OUTPUT_DIR)/base/smerge.o $(OUTPUT_DIR)/base/sspilt.o

objects = $(base_objects) $(OUTPUT_DIR)/io/io.o

FFMPEG_LIBS = -lavcodec -lswscale

FFMPEG_LIB = -lavcodec -lavdevice \
			 -lavfilter -lavformat \
			 -lavutil -lpostproc \
			 -lswresample -lswscale 

# $(shell mkdir -p $(OUTPUT_DIR))
# $(shell mkdir -p $(OUTPUT_DIR)/io)
# $(shell mkdir -p $(OUTPUT_DIR)/base)

$(TARGET): main.cpp $(objects) $(HEADERS)
	$(CXX) $(CXXFLAGS) -L$(LIB_PATH) $(FFMPEG_LIB) -o $(TARGET) main.cpp $(objects)

$(OUTPUT_DIR)/io/io.o: io/io.cpp io/io.h
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_PATH) -c io/io.cpp -o $(OUTPUT_DIR)/io/io.o 

$(OUTPUT_DIR)/base/sbreak.o: base/sbreak.cpp base/sbreak.h 
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_PATH) -c base/sbreak.cpp -o $(OUTPUT_DIR)/base/sbreak.o 
$(OUTPUT_DIR)/base/sjoin.o: base/sjoin.cpp base/sjoin.h
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_PATH) -c base/sjoin.cpp -o $(OUTPUT_DIR)/base/sjoin.o 
$(OUTPUT_DIR)/base/smerge.o: base/smerge.cpp base/smerge.h
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_PATH) -c base/smerge.cpp -o $(OUTPUT_DIR)/base/smerge.o 
$(OUTPUT_DIR)/base/sspilt.o: base/sspilt.cpp base/sspilt.h
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_PATH) -c base/sspilt.cpp -o $(OUTPUT_DIR)/base/sspilt.o 

# $(OUTPUT_DIR)/main.o: main.cpp $(HEADERS)
# 	$(CXX) $(CXXFLAGS) -c main.cpp -o $(OUTPUT_DIR)/main.o

clean:
	rm -f $(objects)
