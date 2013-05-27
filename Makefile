CXX=g++
CXXFLAGS=-Wall -Wextra -g

main: main.o controller.o parser.o IRLS.o LogLink.o
	$(CXX) $(CXXFLAGS) main.o parser.o controller.o IRLS.o LogLink.o -lm -lgsl -lgslcblas -L /usr/local/lib
test: test.o IRLS.o LogLink.o
	$(CXX) $(CXXFLAGS) test.o IRLS.o LogLink.o -lm -lgsl -lgslcblas -L /usr/local/lib -o test_irls
main.o: main.cc
	$(CXX) $(CXXFLAGS) -c main.cc
test.o: test.cpp
	$(CXX) $(CXXFLAGS) -c test.cpp
parser.o: parser.cc parser.h
	$(CXX) $(CXXFLAGS) -c parser.cc
controller.o: controller.cc controller.h
	$(CXX) $(CXXFLAGS) -c controller.cc
IRLS.o: IRLS.h IRLS.cc
	$(CXX) $(CXXFLAGS) -c IRLS.cc
LogLink.o: LogLink.cc LogLink.h
	$(CXX) $(CXXFLAGS) -c LogLink.cc
clean:
	rm -f *.o *~ a.out test_irls
