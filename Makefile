CXXFLAGS = -g -O3 -Wall -Werror -std=c++17 -I./include
LDLIBS =

PRGM  = SBCFATTool
SRCS  = $(shell find ./src     -type f -name *.cpp)
HEADS = $(shell find ./include -type f -name *.h)
OBJS := $(SRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all clean

all: $(PRGM)

$(PRGM): $(OBJS) $(HEADS)
	$(CXX) $(OBJS) $(LDLIBS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

run: $(PRGM)
	./$(PRGM)

clean:
	rm -rf $(OBJS) $(DEPS) $(PRGM)

-include $(DEPS)