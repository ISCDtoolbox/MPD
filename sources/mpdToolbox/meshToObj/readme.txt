18/07/2019
The software meshToObj is a program written in C by Jeremy Dalphin.

It converts a given *.mesh file into the *.obj format (successive lines of
'v x1 x2 x3' followed by some 'f i1 i2 i3' describing vertex coordinates
followed by the enumeration of triangulated faces).

A description of the *.mesh format can be found at the address

https://www.ljll.math.upmc.fr/frey/logiciels/Docmedit.dir/Docmedit.html#SECTION00031000000000000000

We recall brifely the *.mesh format (only for a triangulated 3D mesh,
other type of elements are ignored by the program)
We recall that '#' refers to a comment (until the end of the line)

MeshVersionFormatted 2 # 2=double precision 1=single precision
Dimension 3
Vertices
nVer # total number of vertices
1.5 1e-4 -5.68595 1 # successive description (nVer lines) of the vertices
...                    # x_i y_i z_i label_i (3 double and 1 integer)
Triangles
nTri # total number of triangles
1 4 5 10 # successive description (nTri lines) of the triangles
...      # p1_i p2_i p3_i label_i (4 integers)
End

The program meshToObj only takes two arguments: the input *.mesh file name
followed by the output *.obj file name.

The first *.mesh file must already exist (since it is read).
Warning: the second *.obj file is overwritten it already exits (it is
automatically created if it does not exist).
The maximal number of characters allowed in the file names is 500.

Remark: the maximal length for the file name can be changed at compilation
thanks to the NAME_SIZE_MAX preprocessor constant (do not forget to add one for
the terminating nul character).

////////////////////////////////////////////////////////////////////////////////

The program can be compiled (with gcc compiler) by typing in a command prompt

gcc meshToObj.c -o meshToObj

and executed (on the provided example) by typing in a command prompt

./meshToObj example.mesh example.obj

////////////////////////////////////////////////////////////////////////////////

An example is provided with the program. The file example.mesh (given by
Auréliane Gailliègue) contains the surfacic 3D mesh of a wolf pelvic bone, which
can be vizualized with medit software (https://github.com/ISCDtoolbox/Medit).
A version of medit is also provided with the mpdToolbox (MPD/sources/medit).

The file example.jpg is a screen shot taken from example.mesh loaded with medit.

WARNING COPYRIGHT: the example.mesh file cannot be used for reproduction.
                   It comes from Canis Lupus referred to as ZM-2016-1665
                   and scanned from the platform 'surfaçus' of the Museum
                   National d'Histoire Naturelle (http://surfacus.mnhn.fr/).
                   Check carefully the terms before using it. The work has
                   been done by Surfaçus numérisation surfacique 3D de la
                   Direction générale déléguée aux Collections, MNHN, Paris.

