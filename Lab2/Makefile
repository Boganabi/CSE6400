CXX = g++
CXXFLAGS = -std=c++17
TARGET = lab2
SOURCES = Lab2.cpp Lab2FileManip.cpp SearchAgents.cpp Game.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)