CXX = g++
CXX_FLAGS = -Wall -Wextra -O3 -std=c++17 -I/opt/vc/include
LD_FLAGS = -L/opt/vc/lib -lstdc++fs -lbcm_host

SRC = $(wildcard src/*.cpp)
OBJS = $(SRC:.cpp=.o)

fm: $(OBJS)
    $(CXX) $(CXX_FLAGS) $(OBJS) src/mailbox.c -o $@ $(LD_FLAGS)

.cpp.o:
    $(CXX) $(CXX_FLAGS) -c $< -o $@

.PHONY: clean
clean:
    rm -rf fm src/*.o
