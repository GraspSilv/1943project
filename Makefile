CXX := g++
CXXFLAGS := -g -Wall
RM := rm -f
LDLIBS := -lSDL -lSDL_image -lSDL_ttf
EXEC := Blitz

OBJS := main.o Bullet.o Counter.o Enemy.o GraphElement.o Player.o Powerup.o Timer.o

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC) $(LDLIBS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

Bullet.o: Bullet.h Bullet.cpp
	$(CXX) $(CXXFLAGS) -c Bullet.cpp

Counter.o: Counter.h Counter.cpp
	$(CXX) $(CXXFLAGS) -c Counter.cpp

Enemy.o: Enemy.h Enemy.cpp
	$(CXX) $(CXXFLAGS) -c Enemy.cpp

GraphElement.o: GraphElement.h GraphElement.cpp
	$(CXX) $(CXXFLAGS) -c GraphElement.cpp

Player.o: Player.h Player.cpp
	$(CXX) $(CXXFLAGS) -c Player.cpp
	
Powerup.o: Powerup.h Powerup.cpp
	$(CXX) $(CXXFLAGS) -c Powerup.cpp

Timer.o: Timer.h Timer.cpp
	$(CXX) $(CXXFLAGS) -c Timer.cpp

clean:
	$(RM) *.o $(EXEC)
