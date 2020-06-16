TARGET = main
CXX = g++
CXXFLAGS = -Wall -O0 -g -std=c++0x
VPATH = ./src:./headers
INCLUDES = -I./headers

$(TARGET): main.o queue_exceptions.o
	$(CXX) $^ -o $@

main.o: main.cpp queue.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

queue_exceptions.o: queue_exceptions.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean
clean:
	rm -rf *.o *.exe $(TARGET)
