CXX = g++

CXXFLAGS = -Wall -g -std=c++11

OBJECTS = main.o des.o

main : $(OBJECTS)
	@echo linking $^ ...
	@echo Builded.
	@$(CXX) $(CXXFLAGS) -o main $^

%.o : %.cpp
	@echo compile $<...
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

# clean
.PHONY : clean

clean :
	-rm $(OBJECTS) main