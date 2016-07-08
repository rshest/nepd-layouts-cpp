
CC=g++
CXXFLAGS=-Isrc/. --std=c++11 -O3 -lm
SOURCE=src/main.cpp src/arranger.cpp src/layout.cpp
OUT=build/nepd_arranger

text_cfs: 
    $(CC) -o $(OUT) $(SOURCE) $(CXXFLAGS) 

clean:
    rm $(OUT)

