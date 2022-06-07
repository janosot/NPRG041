CC = g++
CXXFLAGS = -Wall -Wextra -g -O3 -std=c++17

LIBS = -lm -lc++

_PARSER = jparser.hpp
PARSER = $(patsubst %.hpp,%.cpp,$(_DEPS))

_OBJ = jparser.o main.o
OBJ = $(patsubst %.cpp,%.o,$(_OBJ))

_TST = jparser.o test.o
TST = $(patsubst %.cpp,%.o,$(_TST))

make: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o out

test: $(TST)
	$(CC) $(CFLAGS) $(TST) $(LIBS) -o test

.PHONY: clean

clean:
	rm -f $(OBJ) out
	rm -f $(TST) test