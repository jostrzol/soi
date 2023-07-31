CXX = g++-11
CXXFLAGS = -Wall -Wextra -std=c++20 -g

SRC = main.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = main

LDLIBS = -pthread

all: $(EXEC)

main.o: monitor.h sync_queue.h

$(EXEC): $(OBJ)
	$(CXX) -o $@ $(OBJ) $(LDLIBS)

clean:
	rm -rf $(OBJ) $(EXEC)
