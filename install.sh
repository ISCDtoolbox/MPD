#sudo add-apt-repository main
#sudo add-apt-repository universe
#sudo apt-get update

#sudo apt-get install gcc make cmake
#sudo apt-get install libpthread-stubs0-dev libgomp1 freeglut3-dev libxmu-dev libxi-dev libptscotch-dev liblapacke-dev

cd ./sources/commons
mkdir build
cd build
cmake ..
make
make install

cd ../../advection
mkdir build
cd build
cmake ..
make
make install

cd ../../elasticity
mkdir build
cd build
cmake ..
make
make install

cd ../../mshdistance
mkdir build
cd build
cmake ..
make
make install

cd ../../medit
mkdir build
cd build
cmake ..
make
make install

cd ../../mmg3d
mkdir build
cd build
cmake ..
make
make install

cd ../../mpd
mkdir build
cd build
cmake ..
make
make install

cd ../../mpdToolbox/adaptTools
mkdir build
cd build
cmake ..
make
make install

mkdir ../../../../bin/mpdToolbox
cp ../bin/mshint ../../../../bin/mpdToolbox
cp ../bin/mshmet ../../../../bin/mpdToolbox

cd ../../buildBoundedDomain
gcc buildBoundedDomain.c -o buildBoundedDomain
#./buildBoundedDomain example.mesh example.obj
cp buildBoundedDomain ../../../bin/mpdToolbox

cd ../buildLevelSetOfHalfSpace
gcc buildLevelSetOfHalfSpace.c -o buildLevelSetOfHalfSpace
#./buildLevelSetOfHalfSpace example.mesh example.sol
cp buildLevelSetOfHalfSpace ../../../bin/mpdToolbox

cd ../cubeToMesh
gcc cubeToMesh.c -o cubeToMesh
#./cubeToMesh example.cube example.mesh
cp cubeToMesh ../../../bin/mpdToolbox

cd ../doubleSphere
gcc doubleSphere.c -lm -o doubleSphere
#./doubleSphere example.mesh example.sol
#../../../bin/mmg3d -in example.mesh -ls -sol example.sol -out exampleAdapted.mesh -nr -hmin 0.05 -hmax 1.0 -hausd 0.01 -hgrad 2.0
cp doubleSphere ../../../bin/mpdToolbox

cd ../getIntersection
gcc getIntersection.c -o getIntersection
#./getIntersection example.ini.mesh example.out.sol
#../../../bin/mshdist valence2.mesh -dom -ncpu 4
#../../../bin/mpdToolbox/mshint valence2.mesh valence1.mesh
#./getIntersection valence1.mesh valence12.sol
#cp valence1.mesh valence12.mesh
#../../../bin/mshdist valence12.mesh -ncpu 4
#../../../bin/mmg3d -in valence12.mesh -ls -sol valence12.sol -out valence12.mesh -nr -hmin 0.05 -hmax 1.0 -hausd 0.01 -hgrad 2.0
cp getIntersection ../../../bin/mpdToolbox

cd ../hexaToTetra
gcc hexaToTetra.c -o hexaToTetra
#./hexaToTetra example.ini.mesh example.out.mesh
cp hexaToTetra ../../../bin/mpdToolbox

cd ../labelInverter
gcc labelInverter.c -o labelInverter
#./labelInverter example.ini.mesh example.out.mesh
cp labelInverter ../../../bin/mpdToolbox

cd ../meshToCube
gcc meshToCube.c -o meshToCube
#./meshToCube example.mesh example.wfn example.cube
#../../../bin/mshdist tetraToHexa.out.mesh -dom -ncpu 4
#../../../bin/mpdToolbox/cubeToMesh tetraToHexa.ini.cube tetraToHexa.ini.mesh
#../../../bin/mpdToolbox/hexaToTetra tetraToHexa.ini.mesh tetraToHexa.temp.mesh
#../../../bin/mpdToolbox/mshint tetraToHexa.out.mesh tetraToHexa.temp.mesh
#cp tetraToHexa.temp.sol tetraToHexa.ini.sol
#./meshToCube tetraToHexa.ini.mesh tetraToHexa.ini.wfn tetraToHexa.out.cube
cp meshToCube ../../../bin/mpdToolbox

cd ../meshToObj
gcc meshToObj.c -o meshToObj
#./meshToObj example.mesh example.obj
cp meshToObj ../../../bin/mpdToolbox

cd ../objMerge
gcc objMerge.c -o objMerge
#./objMerge example1.ini.obj example2.ini.obj example.out.obj
cp objMerge ../../../bin/mpdToolbox

cd ../refineCubeMesh
gcc refineCubeMesh.c -o refineCubeMesh
#./refineCubeMesh example.ini.mesh example.out.mesh
cp refineCubeMesh ../../../bin/mpdToolbox

cd ../resizeObjFile
gcc resizeObjFile.c -o resizeObjFile
#./resizeObjFile example.ini.obj example.out.obj
cp resizeObjFile ../../../bin/mpdToolbox

cd ../shapeExtractor
gcc shapeExtractor.c -o shapeExtractor
#./shapeExtractor example.ini.mesh example.out.mesh
cp shapeExtractor ../../../bin/mpdToolbox

cd ../vectorFieldNormAtVertices
gcc vectorFieldNormAtVertices.c -lm -o vectorFieldNormAtVertices
#./vectorFieldNormAtVertices example.ini.sol example.out.sol
cp vectorFieldNormAtVertices ../../../bin/mpdToolbox

cd ../../..
mkdir outputFiles
cd ./outputFiles
mkdir quickstartTests
cd ./quickstartTests
mkdir test0
cp ../../inputFiles/quickstart0.info ./test0
mkdir test1
cp ../../inputFiles/quickstart1.info ./test1

