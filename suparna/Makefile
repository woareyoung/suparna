DEFINES +=   # 编译源文件时宏定义
CC = gcc
CCFLAGS = -Wall -g $(DEFINES)
CXX = g++
CXXFLAGS = $(CCFLAGS) -std=c++17  # g++ 编译选项
STD_LIB = -lstdc++fs  # 需要链接的标准库
COMPILE_TYPE = debug
OUTPUT_DIR = ../$(COMPILE_TYPE)

INCLUDE_PATH = E:/ffmpeg/ffmpeg/include  # ffmpeg头文件路径
LIB_PATH = E:/ffmpeg/ffmpeg/lib  # ffmpeg静态库路径

TARGET = $(OUTPUT_DIR)/Suparna # 编译目标

# 工程头文件
HEADERS = io/io.h \
		  base/sbreak.h \
		  base/sjoin.h \
		  base/smerge.h \
		  base/sspilt.h 

# base目录
base_objects = $(OUTPUT_DIR)/base/sbreak.o $(OUTPUT_DIR)/base/sjoin.o \
				$(OUTPUT_DIR)/base/smerge.o $(OUTPUT_DIR)/base/sspilt.o

objects = $(base_objects) $(OUTPUT_DIR)/io/io.o $(OUTPUT_DIR)/main.o $(OUTPUT_DIR)/systool/file.o

# 需要链接的ffmpeg静态库
FFMPEG_LIB = -lavcodec -lavdevice \
			 -lavfilter -lavformat \
			 -lavutil -lpostproc \
			 -lswresample -lswscale

# 创建编译输出的目录
# $(shell mkdir -p $(OUTPUT_DIR))
# $(shell mkdir -p $(OUTPUT_DIR)/io)
# $(shell mkdir -p $(OUTPUT_DIR)/base)
# $(shell mkdir -p $(OUTPUT_DIR)/systool)

$(TARGET): $(objects)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(objects) -L$(LIB_PATH) $(FFMPEG_LIB) $(STD_LIB)

$(OUTPUT_DIR)/main.o: main.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c main.cpp -o $(OUTPUT_DIR)/main.o

$(OUTPUT_DIR)/io/io.o: io/io.cpp io/io.h
	$(CXX) $(CXXFLAGS) -c io/io.cpp -I$(INCLUDE_PATH) -o $(OUTPUT_DIR)/io/io.o 

$(OUTPUT_DIR)/systool/file.o: systool/file.cpp systool/file.h
	$(CXX) $(CXXFLAGS) -c systool/file.cpp -o $(OUTPUT_DIR)/systool/file.o 

$(OUTPUT_DIR)/base/sbreak.o: base/sbreak.cpp base/sbreak.h 
	$(CXX) $(CXXFLAGS) -c base/sbreak.cpp -I$(INCLUDE_PATH) -o $(OUTPUT_DIR)/base/sbreak.o 
$(OUTPUT_DIR)/base/sjoin.o: base/sjoin.cpp base/sjoin.h
	$(CXX) $(CXXFLAGS) -c base/sjoin.cpp -I$(INCLUDE_PATH) -o $(OUTPUT_DIR)/base/sjoin.o 
$(OUTPUT_DIR)/base/smerge.o: base/smerge.cpp base/smerge.h
	$(CXX) $(CXXFLAGS) -c base/smerge.cpp -I$(INCLUDE_PATH) -o $(OUTPUT_DIR)/base/smerge.o 
$(OUTPUT_DIR)/base/sspilt.o: base/sspilt.cpp base/sspilt.h
	$(CXX) $(CXXFLAGS) -c base/sspilt.cpp -I$(INCLUDE_PATH) -o $(OUTPUT_DIR)/base/sspilt.o 

clean:
	rm -f $(objects)
