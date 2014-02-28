OBJECTS = main_filter.o tools.o floor.o building.o filter.o
CXXFLAGS = -g -O2 -larmadillo

filter : $(OBJECTS)
	g++ -o filter $(OBJECTS) $(CXXFLAGS)

main_filter.o : main_filter.cpp
	g++ -c main_filter.cpp $(CXXFLAGS)

filter.o : filter.cpp
	g++ -c filter.cpp $(CXXFLAGS)

building.o : building.cpp
	g++ -c building.cpp $(CXXFLAGS)

floor.o : floor.cpp
	g++ -c floor.cpp $(CXXFLAGS)

tools.o : tools.cpp
	g++ -c tools.cpp $(CXXFLAGS)

.PHONY : clean

clean : 
	rm filter *.o
