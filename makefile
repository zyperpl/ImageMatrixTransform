EXECUTABLE = cgimt

CC         = g++
FLAGS      = -std=c++11 -Wall -Wextra -Wpedantic -Wno-implicit-fallthrough -I ./src/ -I ./include/ -O3

LIBS       = -lstdc++ -lm

SRC_PATH   = ./src
OBJ_PATH   = ./obj

SOURCES  := $(wildcard $(SRC_PATH)/*.cpp)
INCLUDES := $(wildcard $(SRC_PATH)/*.h)
OBJECTS  := $(SOURCES:$(SRC_PATH)/%.cpp=$(OBJ_PATH)/%.o)

$(EXECUTABLE): directories $(OBJECTS)
	@echo "Linking..."
	$(CC) -o $@ $(OBJECTS) $(LIBS) $(FLAGS)
	@echo "Done!"

$(OBJECTS): $(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp 
	@echo "Compiling $@..."
	$(CC) -c -o $@ $< $(FLAGS)

.PHONY: clean run directories

directories:
	mkdir -p obj

clean:
	rm -rf obj/*
	rmdir obj
	rm $(EXECUTABLE)	
	@echo "Cleaned!"

run:
	make -j4 && ./${EXECUTABLE} ${ARGS}
