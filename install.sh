sudo add-apt-repository main
sudo add-apt-repository universe
sudo apt-get update

sudo apt-get install g++ gcc make cmake
sudo apt-get install libgomp1 freeglut3-dev libxmu-dev libxi-dev libptscotch-dev liblapacke-dev

cd ./sources/commons
mkdir build
cd build
cmake ..
make
make install

cd ./../../advection
mkdir build
cd build
cmake ..
make
make install

cd ./../../elasticity
mkdir build
cd build
cmake ..
make
make install

cd ./../../mshdistance
mkdir build
cd build
cmake ..
make
make install

cd ./../../medit
mkdir build
cd build
cmake ..
make
make install

cd ./../../mmg3d
mkdir build
cd build
cmake ..
make
make install

cd ./../../mpd
mkdir build
cd build
cmake ..
make
make install

mkdir outputFiles
cd ./outputFiles
mkdir exampleTetrahedra
mv ./../../inputFiles/exHfTet.info ./exampleTetrahedra/
mkdir exampleHexahedra
mv ./../../inputFiles/exHfHex.info ./exampleHexahedra/

