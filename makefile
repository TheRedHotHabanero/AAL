FLAGS = -Wall -Wextra -g -std=c++0x -pthread
GTKMM_LIBS = `pkg-config gtkmm-3.0 --cflags --libs`
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system
SDL_LIBS = -lSDL2
CC = g++
BUILD_DIR=build
SRC_SIR=src
.PHONY: clean

all: dir AAL

dir:
	@if [ ! -d "build" ]; then mkdir build; fi

AAL: $(BUILD_DIR)/window.o $(BUILD_DIR)/lyapunov.o $(BUILD_DIR)/region.o $(BUILD_DIR)/menu.o
	$(CC) -o $@ $^ $(FLAGS) $(SDL_LIBS) $(GTKMM_LIBS)

$(BUILD_DIR)/window.o: $(SRC_SIR)/window.cpp
	$(CC) -c -o $@ $^ $(FLAGS)

$(BUILD_DIR)/lyapunov.o: $(SRC_SIR)/lyapunov.cpp
	$(CC) -c -o $@ $^ $(FLAGS) $(GTKMM_LIBS)

$(BUILD_DIR)/region.o: $(SRC_SIR)/region.cpp
	$(CC) -c -o $@ $^ $(FLAGS)

$(BUILD_DIR)/menu.o: $(SRC_SIR)/menu.cpp
	$(CC) -c -o $@ $^ $(FLAGS) $(GTKMM_LIBS)

clean:
	rm -Rf $(BUILD_DIR)/*.o $(SRC_SIR)/*.gch
