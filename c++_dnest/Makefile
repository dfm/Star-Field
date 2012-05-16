OPTIONS = -O2 -Wall -Wextra -pedantic -ansi -pthread -DNDEBUG
LIBS = -lgsl -lgslcblas -ldnest -pthread -lboost_thread -lboost_program_options

default:
	g++ $(OPTIONS) -c *.cpp Hyperparameters/*.cpp
	g++ -o main *.o $(LIBS)
	rm -f main.o
	ar rcs libstarfield.a *.o
	rm -f *.o

