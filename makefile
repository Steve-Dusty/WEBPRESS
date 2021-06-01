compiler = g++
main = test.cpp
output = test.o
args = -Isrc/ -pthread



command = $(compiler) -o $(output) $(main) $(args)

all:
	$(command)

debug:
	$(command)
	./$(output)
