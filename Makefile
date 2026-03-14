CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

SRC = src/main.cpp src/Entity.cpp src/Player.cpp src/Monster.cpp src/Item.cpp src/Combat.cpp src/Bestiary.cpp src/Game.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = alterdune

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -I include -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
