24/07/2019
The software labelInverter is a program written in C by Jeremy Dalphin.

It only takes two arguments: the input *.mesh file name followed by the output
*.mesh file name.

The input *.mesh file must already exist (since it is read).

Warning: the output *.mesh file is overwritten if it already exists (it is
automatically created if it does not exist).
The maximal number of characters allowed in the file names is 500

Remark: the maximal length for the file name can be changed at compilation
thanks to the NAME_SIZE_MAX preprocessor constant (do not forget to add one for
the terminating nul character).

The program copy the content of the input *.mesh into the *.mesh output file,
modifying every LABEL_INTERNAL into LABEL_EXTERNAL (and every LABEL_EXTERNAL
into LABEL_INTERNAL) in labels of the mesh elements (tetrahedra or hexahedra).

Remark: we use the convention of mmg3d software (a version is provided with the
        mpdToolbox MPD/sources/mmg3d or see https://github.com/MmgTools/mmg):
        tetrahedra labelled 3 if they belongs to the domain, otherwise 2;
        The values can be changed thanks to the LABEL_INTERNAL and
        LABEL_EXTERNAL preprocessor constants (careful, they are defined as
        char).

Remark: do not forget to recompile the program if LABEL_INTERNAL,
        LABEL_EXTERNAL, or NAME_SIZE_MAX is changed from its default value.

Warning: the labelInverter program assumes that the mesh elements are either
         tetrahedra or hexahedra. In addition, it considers that the labels of
         the mesh elements are either (+/-)LABEL_INTERNAL or
         (+/-)LABEL_EXTERNAL.
         Finally, if one aims to modify the default values 2 and 3, then the
         labelInverter program assumes that the labels are integer number
         comprised between -9 and 9.

Remark: if the label is -2, it will be changed in -3. Similarly, -3 will be
        changed in -2

Warning: for tetrahedral mesh, if a domain represents a subpart of the mesh,
         and if the normal vectors were computed and saved on this mesh, then
         in the output mesh, the normal vector field will be reverted since
         normal vectors are not modified by the labelInverter program.

////////////////////////////////////////////////////////////////////////////////

The program can be compiled (with gcc compiler) by typing in a command prompt

gcc labelInverter.c -o labelInverter

and executed (on the provided example) by typing in a command prompt

./labelInverter example.ini.mesh example.out.mesh

////////////////////////////////////////////////////////////////////////////////

An example is provided with the program. The file example.ini.mesh contains the
tetrahedral 3D mesh of computational box [-3 3]x[-3 3]x[-3 3]. Inside this cube,
there is a certain spherical sub-domain corresponding to the domain locally
maximizing the probability of finding exactly two electrons inside and eight
electrons outside for the neon atom Ne. It is a model in quantum chemistry that
allows to represents in particuler here, the core region of the neon atom.

It can be vizualized with medit software (https://github.com/ISCDtoolbox/Medit).
A version of medit is also provided with the mpdToolbox (MPD/sources/medit).

A screenshot of the mesh is displayed in the example.jpg file.

The mesh is adapted to the metric corresponding to the one associated with the
neon atom Ne. We can observe that there are a lot of tetrahedra around the
core and fewer as we get further.

The program labelInverter inverts the interior (tetrahedra labelled 3) and
exterior (tetrahedra labelled 2) of the little sphere in example.ini.mesh file
and saves the new mesh obtained in the example.out.mesh file.

