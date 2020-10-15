#Compiler
CC=g++
#Compile flags
CFLAGS=-g -std=c++11 -O3
#Linker flags
LFLAGS=-lm
#Program args
CARGS=
#Driver name
TARGET=driver
#Name of the assignment
PROJECT=hw1


#Dynamically find all source files, even in nested dirs
C_SRCS := \
	$(wildcard *.cpp) \
	$(wildcard src/*.cpp) \
	$(wildcard src/**/*.cpp)
#Just like above, but header hunting this time
HDRS := \
	$(wildcard *.h) \
	$(wildcard src/*.h) \
	$(wildcard src/**/*.h) \
#Very scarry patsubt command that effectively
#Just generates object files for all the found sources
OBJS := $(patsubst %.cpp, bin/%.o, $(wildcard *.cpp))
OBJS += $(filter %.o, $(patsubst src/%.cpp, bin/%.o, $(C_SRCS)))


# default target
all: build
	@echo "All Done!"

#Link all built objects
build: $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LFLAGS)

# Catch root directory src files
bin/%.o: %.cpp $(HDRS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Catch all nested directory files
bin/%.o: src/%.cpp $(HDRS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

#Start the program with the flags after compile
run: build
	./$(TARGET) $(CARGS)

#Display all found files
which:
	@echo "FOUND SOURCES: ${C_SRCS}"
	@echo "FOUND HEADERS: ${HDRS}"

#Remove the executable and binaries
clean:
	rm -f $(TARGET)
	rm -rf bin

#Archive the directory TODO - don't tar a tar
tar: 
	tar -czvf wolf7_$(PROJECT).tar.gz *