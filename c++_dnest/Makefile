DNEST_DIR = /home/brewer/Projects/DNest2
OPTIONS = -O2 -Wall -W -pedantic -ansi
LIBS = -lgsl -lgslcblas -ldnest -lboost_thread -lboost_program_options

default:
	g++ $(OPTIONS) -I$(DNEST_DIR) -c *.cpp
	g++ -L$(DNEST_DIR) -o main *.o $(LIBS)
	rm -f main.o
	ar rcs libstarfield.a *.o
	rm -f *.o

