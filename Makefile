CXX=g++
CXXFLAGS=-Wall -Wextra -g
LDFLAGS=-L/usr/local/lib
# LDFLAGS=-L/path/to/lib -Wl,-rpath -Wl,/path/to/lib
LDLIBS=-lm -lgsl -lgslcblas

main: main.o parser.o controller.o IRLS.o LogLink.o
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@
test_irls: test.o IRLS.o LogLink.o
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

main.o: main.cc
	$(CXX) $(CXXFLAGS) -c $^
test.o: test.cpp
	$(CXX) $(CXXFLAGS) -c $^
parser.o: parser.cc parser.h
	$(CXX) $(CXXFLAGS) -c parser.cc
controller.o: controller.cc controller.h
	$(CXX) $(CXXFLAGS) -c controller.cc
IRLS.o: IRLS.h IRLS.cc
	$(CXX) $(CXXFLAGS) -c IRLS.cc
LogLink.o: LogLink.cc LogLink.h
	$(CXX) $(CXXFLAGS) -c LogLink.cc

clean:
	rm -f *.o *~ main test_irls
