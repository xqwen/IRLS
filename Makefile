main: main.o controller.o parser.o IRLS.o LogLink.o
	g++  -O3 main.o parser.o controller.o IRLS.o LogLink.o -lm -L /usr/local/lib -lgsl -lgslcblas
main.o: main.cc
	g++ -c main.cc
parser.o: parser.cc parser.h
	g++ -c parser.cc
controller.o: controller.cc controller.h
	g++ -c controller.cc
IRLS.o: IRLS.h IRLS.cc
	g++ -c IRLS.cc
LogLink.o: LogLink.cc LogLink.h
	g++ -c LogLink.cc
clean:
	rm *.o a.out
