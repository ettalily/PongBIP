mkdir -p build
cp -r sound ./build/sound
x86_64-w64-mingw32-gcc -o ./build/pongbip.exe ./src/global.hpp ./src/*.cpp -I ./raylib/windows/include -L ./raylib/windows/lib/libraylib.a -lgdi32 -lwinmm