FLAGS := -std=c++17 -Wall -Wextra -Wreorder -Isrc/core -Isrc/statuses -Isrc/abilities -Isrc/ui -Isrc/game
FILES := $(shell find src -name '*.cpp')
all:
	g++ $(FLAGS) $(FILES) -o game -lraylib
