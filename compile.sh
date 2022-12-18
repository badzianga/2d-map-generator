# compile files as objects
cd src
g++ -std=c++20 -c ../lib/Caves.cpp
g++ -std=c++20 -c ../lib/App.cpp
g++ -std=c++20 -c ../lib/main.cpp

# compile as executable file
g++ -std=c++20 Caves.o App.o main.o -o map-generator -lsfml-graphics -lsfml-window -lsfml-system
# rm lib/*.o

# run app
./map-generator
