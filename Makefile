FLAGS := -Wall -Wextra -Wreorder
FILES :=  main.cpp game.cpp cell.cpp map.cpp render.cpp robot.cpp factory.cpp
all:
	g++ $(FLAGS) $(FILES) -o game -lraylib
