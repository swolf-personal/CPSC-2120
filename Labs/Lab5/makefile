# default target
all: build
	@echo "All Done!"

#Link all built objects
build:
	g++ -std=c++11 -o nLogNDriver nLogNDriver.cpp
	g++ -std=c++11 -o nSquaredDriver nSquaredDriver.cpp

#Start the program with the flags after compile
run: build
	./nLogNDriver < cuts.txt
	./nSquaredDriver < cuts.txt

#Remove the executable and binaries
clean:
	rm -f nLogNDriver
	rm -f nSquaredDriver

#Archive the directory TODO - don't tar a tar
tar: 
	tar -czvf wolf7_Lab5.tar.gz *