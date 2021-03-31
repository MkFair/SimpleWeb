ALL_SOURCES = $(wildcard core/*.cpp) 
ALL_OBJS = $(patsubst core/%.cpp,temp/%.o,$(ALL_SOURCES))
temp/%.o: core/%.cpp
	@echo 'Building file: $<'
	g++ --std=c++17  -c $< -o temp/$(<:core/%.cpp=%.o)
linked: $(ALL_OBJS)
	@echo 'Building file: $(ALL_OBJS)'
all: linked
	g++ --std=c++17  -c webapi.cpp -o temp/webapi.o
	g++ -pthread  temp/*.o -ljsoncpp
	
clean:
	rm -rf temp/*
