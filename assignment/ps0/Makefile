CC = g++
#CFLAGS = --std=c++17 -Wall -Werror -pedantic -g
#LIB = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lboost_unit_test_framework
CFLAGS = --std=c++17 -Wall -Werror -pedantic -g -I /opt/homebrew/include
LIB = -L /opt/homebrew/lib -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
# Your .hpp files
DEPS = 
# Your compiled .o files
OBJECTS = 
# The name of your program
PROGRAM = sfml-app

.PHONY: all clean lint

all: $(PROGRAM)

# Wildcard recipe to make .o files from corresponding .cpp file
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $<

$(PROGRAM): main.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

clean:
	rm *.o $(PROGRAM)

lint:
	cpplint *.cpp *.hpp
