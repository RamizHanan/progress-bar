# CXX Make variable for compiler
CXX=g++
# Make variable for compiler options
#	-std=c++11  C/C++ variant to use, e.g. C++ 2011
#	-g          include information for symbolic debugger e.g. gdb 
CXXFLAGS=-std=c++11
LDFLAGS=-pthread
# Rules format:
# target : dependency1 dependency2 ... dependencyN
#     Command to make target, uses default rules if not specified

wordcount : wordcount.o
	$(CXX) $(CXXFLAGS) -o wordcount $^ $(LDFLAGS)

wordcount.o : wordcount.C

clean:
	rm *.o
