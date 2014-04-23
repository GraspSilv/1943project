CXX := g++
CXXFLAGS := -g -Wall
RM := rm -f
LDLIBS := -lSDL -lSDL_image -lSDL_ttf
EXEC := Blitz

OBJS := main.o Counter.o GraphElement.o Player.o Powerup.o Bullet.o

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC) $(LDLIBS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

Counter.o: Counter.h Counter.cpp
	$(CXX) $(CXXFLAGS) -c Counter.cpp

GraphElement.o: GraphElement.h GraphElement.cpp
	$(CXX) $(CXXFLAGS) -c GraphElement.cpp

Player.o: Player.h Player.cpp
	$(CXX) $(CXXFLAGS) -c Player.cpp
	
Powerup.o: Powerup.h Powerup.cpp
	$(CXX) $(CXXFLAGS) -c Powerup.cpp

Bullet.o: Bullet.h Bullet.cpp
	$(CXX) $(CXXFLAGS) -c Bullet.cpp

clean:
	$(RM) *.o $(EXEC)
