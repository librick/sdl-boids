default:
	g++ src/*.cpp -g -std=c++17 -lSDL2 -lSDL2_ttf -lSDL2_image -Wall `sdl2-config --cflags --libs` -o out 
