
mkdir build
cd build
conan install .. --build missing
cmake .. -G "Visual Studio 14 Win64"
cmake --build .
cd bin
.\PuckMan.exe
cd ../..
