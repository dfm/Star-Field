OPTIONS = -O2 -Wall -Wextra -pedantic -ansi -DNDEBUG
LIBS = -lgsl -lgslcblas -ldnest3

default:
	g++ $(OPTIONS) -c *.cpp Hyperparameters/*.cpp
	g++ -o main *.o $(LIBS)
	rm -f main.o
	ar rcs libstarfield.a *.o
	rm -f *.o

