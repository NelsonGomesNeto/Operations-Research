CPLEXFLAGS=-O3 -m64 -O -fPIC -fexceptions -DNDEBUG -DIL_STD -I/opt/cplex/cplex/include -I/opt/cplex/concert/include  -L/opt/cplex/cplex/lib/x86-64_sles10_4.1/static_pic -lilocplex -lcplex -L/opt/cplex/concert/lib/x86-64_sles10_4.1/static_pic -lconcert -lm -pthread -std=c++0x 

CFLAGS=-std=c++11 -static-libstdc++ -static-libgcc -Wall

all:transportProblem.o
	g++  transportProblem.o -o transportProblem.run $(CPLEXFLAGS) $(CFLAGS)


%.o: %.cpp %.hpp
	g++ -c $< -o $@ $(CFLAGS)

transportProblem.o: transportProblem.cpp
	g++ -c -o transportProblem.o transportProblem.cpp $(CPLEXFLAGS) $(CFLAGS)

clean:
	rm -f *.o
