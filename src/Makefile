OBJECTS = main_filter.o filter.o building.o cell.o tools.o
CXXFLAGS = -O2 -larmadillo

filter : $(OBJECTS)
	g++ -o filter $(OBJECTS) $(CXXFLAGS)

main_filter.o : main_filter.cpp
	g++ -c main_filter.cpp $(CXXFLAGS)

filter.o : filter.cpp
	g++ -c filter.cpp $(CXXFLAGS)

building.o : building.cpp
	g++ -c building.cpp $(CXXFLAGS)

cell.o : cell.cpp
	g++ -c cell.cpp $(CXXFLAGS)

tools.o : tools.cpp
	g++ -c tools.cpp $(CXXFLAGS)

.PHONY : clean

clean : 
	rm filter $(OBJECTS)
