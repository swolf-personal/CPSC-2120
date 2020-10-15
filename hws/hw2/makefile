# default target
all: build
	@echo "All Done!"

#Link all built objects
build:
	mkdir progs
	mkdir bads
	g++ -std=c++11 prog1.cpp -o progs/prog1
	g++ -std=c++11 prog2.cpp -o progs/prog2
	g++ -std=c++11 prog3.cpp -o progs/prog3
	g++ -std=c++11 bad1.cpp -o bads/bad1
	g++ -std=c++11 bad2.cpp -o bads/bad2
	g++ -std=c++11 bad3.cpp -o bads/bad3

#Start the program with the flags after compile
run: build
	time ./bads/bad1 100000 | ./progs/prog1
	time ./bads/bad2 100000 | ./progs/prog2
	time ./bads/bad3 100000 | ./progs/prog3

#Archive the directory TODO - don't tar a tar
tar: 
	tar -czvf wolf7_hw2.tar.gz *