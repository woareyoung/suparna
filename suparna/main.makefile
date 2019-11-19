CC = gcc
CCFLAGS = -c -W
CXX = g++
CXXFLAGS = $(CCFLAGS) -std=c++11
COMPILE_TYPE = debug
OUTPUT_DIR = ../$(COMPILE_TYPE)

INCLUDE_PATH = E:/ffmpeg/ffmpeg/include
LIB_PATH = E:/ffmpeg/ffmpeg/lib

FFMPEG_LIB = -llibavcodec -llibavdivice -llibavfilter \
             -llibavformat -llibavuti -llibpostproc \
			 -llibswresample -llibswscale 

TARGET = $(OUTPUT_DIR)/suparna

# $(shell mkdir -p $(OUTPUT_DIR))
# $(shell mkdir -p $(OUTPUT_DIR)/io)
# $(shell mkdir -p $(OUTPUT_DIR)/base)

objects = $(OUTPUT_DIR)/io/io.o $(OUTPUT_DIR)/base/base.o

base_objects = $(OUTPUT_DIR)/base/sbreak.o $(OUTPUT_DIR)/base/sjoin.o \
				$(OUTPUT_DIR)/base/smerge.o $(OUTPUT_DIR)/base/sspilt.o

main: $(objects)
	$(CXX) -o $(TARGET) $(objects)

$(OUTPUT_DIR)/io/io.o: io/io.c io/io.h
	$(CC) $(CCFLAGS) -I$(INCLUDE_PATH) -o $(OUTPUT_DIR)/io/io.o io/io.c

$(OUTPUT_DIR)/base/base.o: $(base_objects)
	$(CXX) $(CXXFLAGS) $(FFMPEG_LIB) -o $(OUTPUT_DIR)/base/base.o $(base_objects)

$(OUTPUT_DIR)/base/sbreak.o: base/sbreak.cpp base/sbreak.h 
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_PATH) -o $(OUTPUT_DIR)/base/sbreak.o base/sbreak.cpp
$(OUTPUT_DIR)/base/sjoin.o: base/sjoin.cpp base/sjoin.h
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_PATH) -o $(OUTPUT_DIR)/base/sjoin.o base/sjoin.cpp
$(OUTPUT_DIR)/base/smerge.o: base/smerge.cpp base/smerge.h
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_PATH) -o $(OUTPUT_DIR)/base/smerge.o base/smerge.cpp
$(OUTPUT_DIR)/base/sspilt.o: base/sspilt.cpp base/sspilt.h
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_PATH) -o $(OUTPUT_DIR)/base/sspilt.o base/sspilt.cpp

clean:
	rm edit $(objects)