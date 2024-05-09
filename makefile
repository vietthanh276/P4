run_simulation: shopQueue.o simulation.o
	g++ -o run_simulation shopQueue.o simulation.o -Wall

shopQueue.o: shopQueue.cpp shopQueue.h
	g++ -c shopQueue.cpp -Wall

simulation.o: simulation.cpp shopQueue.h
	g++ -c simulation.cpp -Wall

clean:
	rm -f run_simulation shopQueue.o simulation.o

all: run_simulation