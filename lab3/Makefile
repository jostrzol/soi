CXX = g++-11
CXXFLAGS = -Wall -Wextra -std=c++20 -g

SRC = main.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = consumer-producer

LDLIBS = -pthread

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) -o $@ $(OBJ) $(LDLIBS)

clean:
	rm -rf $(OBJ) $(EXEC)
