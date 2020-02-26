ifeq ($(OS),Windows_NT)     # is Windows_NT on XP, 2000, 7, Vista, 10...
    detected_OS := Windows
else
    detected_OS := $(shell uname)  # same as "uname -s"
endif

ifeq ($(detected_OS), Windows)
OUTPUT := main.exe
LIBLOC := -Llib
FLAGS := -std=c++17 -Iinclude -Wall -g -pthread
else
OUTPUT := main.out
LIBLOC := -L/usr/lib/x86_64-linux-gnu
FLAGS := -std=c++17 -Wall -g -pthread
endif

CC := g++
OUTFLAG := -o
ENTRY = MainWin.cpp
SRC := include
SRCS := $(wildcard $(SRC)/*.cpp)
SRCSCORE := $(wildcard $(SRC)/Core/*.cpp)
SRCSARTH := $(wildcard $(SRC)/Arth/*.cpp)
LIBS := -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-system -lsfml-window -lSFGUI


myOS:
	@echo $(detected_OS)

b: $(ENTRY)
	$(CC) $(ENTRY) $(SRCS) $(SRCSCORE) $(SRCSARTH) $(OUTFLAG) $(OUTPUT) $(LIBLOC) $(LIBS) $(FLAGS)

r: MainWin.cpp
	./$(OUTPUT)

br: MainWin.cpp
	make b && make r