default:
	g++ -O2 -Wall -W -pedantic -ansi -I/home/brewer/Projects/DNest2 -c *.cpp
	g++ -L/home/brewer/Projects/DNest2 -o main *.o -lgsl -lgslcblas -ldnest -lboost_thread -lboost_program_options
	rm -f main.o
	ar rcs libstarfield.a *.o
	rm -f *.o

