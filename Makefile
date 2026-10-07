FLAGS := -Wall -Wextra -Wreorder
FILES :=  main.cpp game.cpp cell.cpp map.cpp render.cpp robot.cpp factory.cpp around_ab.cpp range_ab.cpp heal_ab.cpp teleport_ab.cpp slow.cpp overdrive.cpp shield.cpp burning.cpp
all:
	g++ $(FLAGS) $(FILES) -o game -lraylib
