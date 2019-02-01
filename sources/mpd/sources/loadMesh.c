/**
* \file loadMesh.c
* \brief It contains all the functions used to load properly the mesh from a
*        *.mesh/ *.cube file (or use the default/specified parameters if the
*        file does not exists) in the MPD algorithm.
* \author Jeremy DALPHIN
* \version 2.0
* \date September 1st, 2018
*
* The main function of this file is called \ref loadMesh and many other
* functions should be static but have been define as non-static for performing
* unit-tests on them.
*/

#include "loadMesh.h"

/* ************************************************************************** */
// The function initializeMeshStructure sets to zero all the variables the Mesh
// structure (and pointers to NULL). It has the Mesh* variable (defined in
// main.h) as input argument and it does not return any value (void output)
/* ************************************************************************** */
void initializeMeshStructure(Mesh* pMesh)
{
    if (pMesh!=NULL)
    {
        pMesh->nver=0;
        pMesh->pver=NULL;

        pMesh->ncor=0;

        pMesh->nnorm=0;
        pMesh->pnorm=NULL;

        pMesh->ntan=0;
        pMesh->ptan=NULL;

        pMesh->nedg=0;
        pMesh->pedg=NULL;

        pMesh->ntri=0;
        pMesh->ptri=NULL;

        pMesh->ntet=0;
        pMesh->ptet=NULL;

        pMesh->nqua=0;
        pMesh->pqua=NULL;

        pMesh->nhex=0;
        pMesh->phex=NULL;

        pMesh->nadj=0;
        pMesh->padj=NULL;
    }

    return;
}

/* ************************************************************************** */
// The function freeMeshMemory free the memory dynamically allocated with
// calloc/malloc/realloc for the Mesh structure (but other variables than
// pointers are not reset to zero). It has the Mesh* variable (defined in
// main.h) as input argument and it does not return value any value
/* ************************************************************************** */
void freeMeshMemory(Mesh* pMesh)
{
    if (pMesh!=NULL)
    {
        free(pMesh->pver); // free function does not return any value
        pMesh->pver=NULL;

        free(pMesh->pnorm);
        pMesh->pnorm=NULL;

        free(pMesh->ptan);
        pMesh->ptan=NULL;

        free(pMesh->pedg);
        pMesh->pedg=NULL;

        free(pMesh->ptri);
        pMesh->ptri=NULL;

        free(pMesh->ptet);
        pMesh->ptet=NULL;

        free(pMesh->pqua);
        pMesh->pqua=NULL;

        free(pMesh->phex);
        pMesh->phex=NULL;

        free(pMesh->padj);
        pMesh->padj=NULL;
    }

    return;
}

/* ************************************************************************** */
// The function initializeCubeDiscretization loads some default parameters in
// the structure pointed by pParameters in order to initialize the meshing of
// the cube associated with a default initial computational box. Depending on
// pParameters->opt_mode (<=0/>0), it computes the number of vertices, elements
// (hexahedra/tetrahdra), and boundary ones (quadrilaterals/triangles) and store
// the corresponding values in the structure pointed by pMesh. It has the
// parameters* and Mesh* variables (both defined in main.h) as input arguments
// and it returns one (respectively minus one), if the suggested discretization
// is manually approved (resp. disapproved) by the user, otherwise zero (error)
/* ************************************************************************** */
int initializeCubeDiscretization(Parameters* pParameters, Mesh* pMesh)
{
    int nX=0, nY=0, nZ=0, sizeMemory=0, readChar=0, returnValue=0;
    int nVer=0, nEdg=0, nNorm=0, nTan=0, nTri=0, nTet=0, nQua=0, nHex=0;

    // Check if the input pParameters or pMesh variable is pointing to NULL
    if (pParameters==NULL || pMesh==NULL)
    {
        PRINT_ERROR("In loadMesh: at least one of the input pParameters ");
        fprintf(stderr,"%p or pMesh %p ",(void*)pParameters,(void*)pMesh);
        fprintf(stderr,"variable does not have a valid address.\n");
        return 0;
    }

    // Check if the pParamers->n_xyz variables are strictly greater than two
    nX=pParameters->n_x;
    nY=pParameters->n_y;
    nZ=pParameters->n_z;
    if (nX<3 || nY<3 || nZ<3)
    {
        PRINT_ERROR("In loadMesh: expecting that the n_x ");
        fprintf(stderr,"(=%d), n_y (=%d), and n_z (=%d) variables ",nX,nY,nZ);
        fprintf(stderr,"of the structure pointed by pParameters must remain ");
        fprintf(stderr,"(strictly) greater than two (in order to have at ");
        fprintf(stderr,"least one normal/tangent vector on each face of the ");
        fprintf(stderr,"cube associated with the default initial ");
        fprintf(stderr,"computational box).\n");
        return 0;
    }

    nVer=nX*nY*nZ;

    nNorm=2*((nX-2)*(nY-2)+(nX-2)*(nZ-2)+(nY-2)*(nZ-2));
    nTan=4*((nX-2)+(nY-2)+(nZ-2));
    nEdg=4*((nX-1)+(nY-1)+(nZ-1));

    // (2 triangles per square) x 2 faces
    nTri=4*((nX-1)*(nY-1)+(nX-1)*(nZ-1)+(nY-1)*(nZ-1));

    // 2 faces per square
    nQua=2*((nX-1)*(nY-1)+(nX-1)*(nZ-1)+(nY-1)*(nZ-1));

    nHex=(nX-1)*(nY-1)*(nZ-1);

    // 6 tetrahedra per cube (warning: do not forget to respect the adjacency
    // between cube cells in the mesh discretization)
    nTet=6*(nX-1)*(nY-1)*(nZ-1);

    // Evaluate the memory needed for such a discretization
    sizeMemory=sizeof(Mesh)+nVer*sizeof(Point);
    if (pParameters->opt_mode>0)
    {
        sizeMemory+=(nNorm+nTan)*sizeof(Vector)+nEdg*sizeof(Edge);
        sizeMemory+=nTri*sizeof(Triangle)+nTet*sizeof(Tetrahedron);
    }
    else
    {
        sizeMemory+=nQua*sizeof(Quadrilateral)+nHex*sizeof(Hexahedron);
    }

    // Recall the discretization that is going to be performed
    if (pParameters->verbose)
    {
        fprintf(stdout,"\nThe default initial computational box will be the ");
        fprintf(stdout,"cube\n[%lf,%lf]",pParameters->x_min,pParameters->x_max);
        fprintf(stdout,"x[%lf,%lf]x[",pParameters->y_min,pParameters->y_max);
        fprintf(stdout,"%lf,%lf]\nwith ",pParameters->z_min,pParameters->z_max);
        fprintf(stdout,"(%lf)x(%lf)",pParameters->delta_x,pParameters->delta_y);
        fprintf(stdout,"x(%lf) and (%d)x(%d)x",pParameters->delta_z,nX,nY);
        fprintf(stdout,"(%d) points.\n",nZ);
    }

    fprintf(stdout,"\nThe (default) initial mesh we want to generate will ");
    fprintf(stdout,"have:\n%d vertices, ",nVer);
    if (pParameters->opt_mode>0)
    {
        fprintf(stdout,"%d tetrahedra, and ",nTet);
        fprintf(stdout,"%d boundary triangles\n(8 corners, ",nTri);
        fprintf(stdout,"%d edges, %d tangent and %d ",nEdg,nTan,nNorm);
        fprintf(stdout,"normal vectors).\n");
    }
    else
    {
        fprintf(stdout,"%d hexahedra, and ",nHex);
        fprintf(stdout,"%d boundary quadrilaterals.\n",nQua);
    }
    fprintf(stdout,"\nGenerate default initial mesh for the ");
    fprintf(stdout,"computational box will take ");
    fprintf(stdout,"%d,%d Mo.\n",sizeMemory/1000000,sizeMemory%1000000);

    // If pParameters->save_print equals zero, graphic and prompt mode is off
    if (pParameters->save_print)
    {
        fprintf(stdout,"Do you want to continue (y/n)? ");

        // Manually confirm the discretization of the cube: getchar returns the
        // character read as an unsigned char cast to an int or EOF for an error
        readChar=getchar();
        switch (readChar)
        {
            case 'y':
                fprintf(stdout,"Ok, we mesh the cube!\n");

                // Set the structure to zero for variables and NULL for pointers
                initializeMeshStructure(pMesh);

                pMesh->nver=nVer;
                if (pParameters->opt_mode>0)
                {
                    pMesh->ncor=8;
                    pMesh->nnorm=nNorm;
                    pMesh->ntan=nTan;
                    pMesh->nedg=nEdg;
                    pMesh->ntri=nTri;
                    pMesh->ntet=nTet;
                }
                else
                {
                    pMesh->nqua=nQua;
                    pMesh->nhex=nHex;
                }

                returnValue=1;
                break;

            case 'n':
                fprintf(stdout,"Ok, leaving program.\n");
                returnValue=-1;
                break;

            case EOF:
                PRINT_ERROR("In initializeCubeDiscretization: wrong return ");
                fprintf(stderr,"(=%d) of the standard getchar c-function ",EOF);
                fprintf(stderr,"in the attempt of reading your answer ");
                fprintf(stderr,"('y'=yes or 'n'=no).\n");
                returnValue=0;
                break;

            default:
                PRINT_ERROR("In initializeCubeDiscretization: expecting ");
                fprintf(stderr,"'y'(=yes) or 'n'(=no) instead of ");
                fprintf(stderr,"'%c' in your answer.\n",readChar);
                returnValue=0;
                break;
        }

        // Clean the input buffer of the standard input stream (stdin) in order
        // to avoid buffer overflow later (warning here: be sure that it is not
        // empty otherwise it will wait undefinitively for a non-empty input)
        while (readChar!='\n' && readChar!=EOF)
        {
            readChar=getchar();
        }
    }
    else
    {
        fprintf(stdout,"Ok, we mesh the cube!\n");

        // Set the structure to zero for variables and NULL for pointers
        initializeMeshStructure(pMesh);

        pMesh->nver=nVer;
        if (pParameters->opt_mode>0)
        {
            pMesh->ncor=8;
            pMesh->nnorm=nNorm;
            pMesh->ntan=nTan;
            pMesh->nedg=nEdg;
            pMesh->ntri=nTri;
            pMesh->ntet=nTet;
        }
        else
        {
            pMesh->nqua=nQua;
            pMesh->nhex=nHex;
        }

        returnValue=1;
    }

    return returnValue;
}

/* ************************************************************************** */
// The function allocateInitialMeshMemory dynamically allocates memory for the
// structure pointed by pMesh thanks to the values already loaded in the
// corresponding Mesh structure by the initializeCubeDiscretization function.
// It has the Mesh* variable (defined in main.h) as input argument and it returns zero if an error occurs, otherwise one is returned in case of success
/* ************************************************************************** */
int allocateInitialMeshMemory(Parameters* pParameters, Mesh* pMesh)
{
    // Check if the pMesh pointer (input variable) has a NULL value or not
    if (pMesh==NULL)
    {
        PRINT_ERROR("In allocateInitialMeshMemory: the input pMesh variable ");
        fprintf(stderr,"is pointing to the %p address.\n",(void*)pMesh);
        return 0;
    }

    // Check the number of elements, depending on pParameters->opt_mode
    if (pParameters->opt_mode>0)
    {
        if (pMesh->nver<1 || pMesh->ncor<8 || pMesh->nnorm<1 || pMesh->ntan<1 ||
                                pMesh->nedg<1 || pMesh->ntri<1 || pMesh->ntet<1)
        {
            PRINT_ERROR("In allocateInitialMeshMemory: the number of ");
            fprintf(stderr,"vertices (=%d), ",pMesh->nver);
            fprintf(stderr,"corners (=%d), ",pMesh->ncor);
            fprintf(stderr,"normal vectors (=%d), ",pMesh->nnorm);
            fprintf(stderr,"tangent vectors (=%d), ",pMesh->ntan);
            fprintf(stderr,"edges (=%d), ",pMesh->nedg);
            fprintf(stderr,"triangles (=%d), ",pMesh->ntri);
            fprintf(stderr,"and tetrahedra (=%d), saved in the ",pMesh->ntet);
            fprintf(stderr,"structure pointed by pMesh, should be positive ");
            fprintf(stderr,"integers (and expecting at least 8 corners).\n");
            return 0;
        }

        if (pParameters->verbose)
        {
            fprintf(stdout,"Allocating memory for the Mesh structure.\n");
        }

        // Allocate memory for the different pointers and check if it worked
        pMesh->pver=(Point*)calloc(pMesh->nver,sizeof(Point));
        pMesh->pnorm=(Vector*)calloc(pMesh->nnorm,sizeof(Vector));
        pMesh->ptan=(Vector*)calloc(pMesh->ntan,sizeof(Vector));
        pMesh->pedg=(Edge*)calloc(pMesh->nedg,sizeof(Edge));
        pMesh->ptri=(Triangle*)calloc(pMesh->ntri,sizeof(Triangle));
        pMesh->ptet=(Tetrahedron*)calloc(pMesh->ntet,sizeof(Tetrahedron));

        if (pMesh->pver==NULL || pMesh->pnorm==NULL || pMesh->ptan==NULL ||
                    pMesh->pedg==NULL || pMesh->ptri==NULL || pMesh->ptet==NULL)
        {
            PRINT_ERROR("In allocateInitialMeshMemory: could not allocate ");
            fprintf(stderr,"memory for the initial mesh.\n");
            return 0;
        }
    }
    else
    {
        if (pMesh->nver<1 || pMesh->nqua<1 || pMesh->nhex<1)
        {
            PRINT_ERROR("In allocateInitialMeshMemory: the number of ");
            fprintf(stderr,"vertices (=%d), ",pMesh->nver);
            fprintf(stderr,"quadrilaterals (=%d), ",pMesh->nqua);
            fprintf(stderr,"and hexahedra (=%d), saved in the ",pMesh->nhex);
            fprintf(stderr,"structure pointed by pMesh, should be positive ");
            fprintf(stderr,"integers.\n");
            return 0;
        }

        if (pParameters->verbose)
        {
            fprintf(stdout,"Allocating memory for the Mesh structure.\n");
        }

        // Allocate memory for the different pointers and check if it worked
        pMesh->pver=(Point*)calloc(pMesh->nver,sizeof(Point));
        pMesh->pqua=(Quadrilateral*)calloc(pMesh->nqua,sizeof(Quadrilateral));
        pMesh->phex=(Hexahedron*)calloc(pMesh->nhex,sizeof(Hexahedron));
        if (pMesh->pver==NULL || pMesh->pqua==NULL || pMesh->phex==NULL)
        {
            PRINT_ERROR("In allocateInitialMeshMemory: could not allocate ");
            fprintf(stderr,"memory for the initial mesh.\n");
            return 0;
        }
    }

    return 1;
}

/* ************************************************************************** */
// The function labelPoint set the labels for the cube vertices: 0 on internal
// points; 1, 2, 3, 4, 5, and 6 on the corresponding faces; 12, 14, 15, 16, 23,
// 25, 26, 34, 35, 36, 45, and 46 on the corresponding ridges; 125, 126, 145,
// 146, 235, 236, 345, and 346 on the corresponding corners. It has the three
// indices associated with the point coordinates (i,j,k) = (x_min+i*delta_x,
// y_min+j*delta_y,z_min+k*delta_z) i=0...nX-1, j=0...nY-1, k=0...nZ-1 as input
// arguments and it returns the point label (warning here: be sure that i,j,k
// lie between 0 and nXYZ-1 before using this function)
/* ************************************************************************** */
int labelPoint(int i, int j, int k, int nX, int nY, int nZ)
{
    int returnValue=0;

    if (!i)
    {
        if (!j)
        {
            if (!k)
            {
                returnValue=145;
            }
            else if (k==nZ-1)
            {
                returnValue=146;
            }
            else
            {
                returnValue=14;
            }
        }
        else if (j==nY-1)
        {
            if (!k)
            {
                returnValue=345;
            }
            else if (k==nZ-1)
            {
                returnValue=346;
            }
            else
            {
                returnValue=34;
            }
        }
        else
        {
            if (!k)
            {
                returnValue=45;
            }
            else if (k==nZ-1)
            {
                returnValue=46;
            }
            else
            {
                returnValue=4;
            }
        }
    }
    else if (i==nX-1)
    {
        if (!j)
        {
            if (!k)
            {
                returnValue=125;
            }
            else if (k==nZ-1)
            {
                returnValue=126;
            }
            else
            {
                returnValue=12;
            }
        }
        else if (j==nY-1)
        {
            if (!k)
            {
                returnValue=235;
            }
            else if (k==nZ-1)
            {
                returnValue=236;
            }
            else
            {
                returnValue=23;
            }
        }
        else
        {
            if (!k)
            {
                returnValue=25;
            }
            else if (k==nZ-1)
            {
                returnValue=26;
            }
            else
            {
                returnValue=2;
            }
        }
    }
    else
    {
        if (!j)
        {
            if (!k)
            {
                returnValue=15;
            }
            else if (k==nZ-1)
            {
                returnValue=16;
            }
            else
            {
                returnValue=1;
            }
        }
        else if (j==nY-1)
        {
            if (!k)
            {
                returnValue=35;
            }
            else if (k==nZ-1)
            {
                returnValue=36;
            }
            else
            {
                returnValue=3;
            }
        }
        else
        {
            if (!k)
            {
                returnValue=5;
            }
            else if (k==nZ-1)
            {
                returnValue=6;
            }
            else
            {
                returnValue=0;
            }
        }
    }

    return returnValue;
}

/* ************************************************************************** */
// The function meshCube fills the structure pointed by pMesh, whose memory
// has already been allocated thanks to the allocateInitialMeshMemory function
// according the mesh discretization given by the parameters stored in the
// structure pointed by pParameters. It has the Parameters* and Mesh* variables
// (both defined in main.h) as input arguments and it returns one on success,
// otherwise zero is returned if an error is encountered
/* ************************************************************************** */
int meshCube(Parameters* pParameters, Mesh* pMesh)
{
    int i=0, j=0, k=0, counter=0, nX=0, nY=0, nZ=0;
    double delta=0.;

    // Check if the input variables are pointing to NULL
    if (pParameters==NULL || pMesh==NULL)
    {
        PRINT_ERROR("In meshCube: one of the input variable pParameters=");
        fprintf(stderr,"%p pr pMesh=%p does ",(void*)pParameters,(void*)pMesh);
        fprintf(stderr,"not have a valid address.\n");
        return 0;
    }

    // Check discretization in the first-coordinate direction
    delta=(pParameters->x_max-pParameters->x_min)/(double)(pParameters->n_x-1);
    if (pParameters->n_x<3 || pParameters->x_min>=pParameters->x_max ||
                                    DEF_ABS(pParameters->delta_x-delta)>=1.e-16)
    {
        PRINT_ERROR("In meshCube: incorrect discretization of the cube in ");
        fprintf(stderr,"the first-coordinate direction: expecting\n");
        fprintf(stderr,"(pParameters->n_x=%d) > 2\n",pParameters->n_x);
        fprintf(stderr,"(pParameters->x_min=%lf) < ",pParameters->x_min);
        fprintf(stderr,"(pParameters->x_max=%lf)\n",pParameters->x_max);
        fprintf(stderr,"(pParameters->delta_x=%.18lf) ",pParameters->delta_x);
        fprintf(stderr,"== %.18lf (up to 15 decimals).\n",delta);
        return 0;
    }

    // Check discretization in the second-coordinate direction
    delta=(pParameters->y_max-pParameters->y_min)/(double)(pParameters->n_y-1);
    if (pParameters->n_y<3 || pParameters->y_min>=pParameters->y_max ||
                                    DEF_ABS(pParameters->delta_y-delta)>=1.e-16)
    {
        PRINT_ERROR("In meshCube: incorrect discretization of the cube in ");
        fprintf(stderr,"the second-coordinate direction: expecting\n");
        fprintf(stderr,"(pParameters->n_y=%d) > 2\n",pParameters->n_y);
        fprintf(stderr,"(pParameters->y_min=%lf) < ",pParameters->y_min);
        fprintf(stderr,"(pParameters->y_max=%lf)\n",pParameters->y_max);
        fprintf(stderr,"(pParameters->delta_y=%.18lf) ",pParameters->delta_y);
        fprintf(stderr,"== %.18lf (up to 15 decimals).\n",delta);
        return 0;
    }

    // Check discretization in the third-coordinate direction
    delta=(pParameters->z_max-pParameters->z_min)/(double)(pParameters->n_z-1);
    if (pParameters->n_z<3 || pParameters->z_min>=pParameters->z_max ||
                                    DEF_ABS(pParameters->delta_z-delta)>=1.e-16)
    {
        PRINT_ERROR("In meshCube: incorrect discretization of the cube in ");
        fprintf(stderr,"the third-coordinate direction: expecting\n");
        fprintf(stderr,"(pParameters->n_z=%d) > 2\n",pParameters->n_z);
        fprintf(stderr,"(pParameters->z_min=%lf) < ",pParameters->z_min);
        fprintf(stderr,"(pParameters->z_max=%lf)\n",pParameters->z_max);
        fprintf(stderr,"(pParameters->delta_z=%.18lf) ",pParameters->delta_z);
        fprintf(stderr,"== %.18lf (up to 15 decimals).\n",delta);
        return 0;
    }

    // Check if memory has been allocated for the different types of elements
    if (pParameters->opt_mode>0)
    {
        if (pMesh->pver==NULL || pMesh->pnorm==NULL || pMesh->ptan==NULL ||
                    pMesh->pedg==NULL || pMesh->ptri==NULL || pMesh->ptet==NULL)
        {
            PRINT_ERROR("In meshCube: one of the following pointer\n");
            fprintf(stderr,"pMesh->pver=%p\n",(void*)pMesh->pver);
            fprintf(stderr,"pMesh->pnorm=%p\n",(void*)pMesh->pnorm);
            fprintf(stderr,"pMesh->ptan=%p\n",(void*)pMesh->ptan);
            fprintf(stderr,"pMesh->pedg=%p\n",(void*)pMesh->pedg);
            fprintf(stderr,"pMesh->ptri=%p\n",(void*)pMesh->ptri);
            fprintf(stderr,"pMesh->ptet=%p\n",(void*)pMesh->ptet);
            fprintf(stderr,"does not have a valid allocated adress.\n");
            return 0;
        }
    }
    else
    {
        if (pMesh->pver==NULL || pMesh->pqua==NULL || pMesh->phex==NULL)
        {
            PRINT_ERROR("In meshCube: one of the following pointer\n");
            fprintf(stderr,"pMesh->pver=%p\n",(void*)pMesh->pver);
            fprintf(stderr,"pMesh->pqua=%p\n",(void*)pMesh->pqua);
            fprintf(stderr,"pMesh->phex=%p\n",(void*)pMesh->phex);
            fprintf(stderr,"does not have a valid allocated adress.\n");
            return 0;
        }
    }

    // Saving the cube vertices: for i=0...n_x-1, j=0...n_y-1, k=0...n_z-1,
    // P[(i*n_y+j)*n_z+k]=(x_min+i*delta_x, y_min+j*delta_y, z_min+k*delta_z)
    // Points are labelled according to boundary face, edge (10*face1+face2
    // with face1<face2), or corner (100*face1+10*face2+face3 with
    // face1<face2<face3), otherwise zero (for internal points)
    if (pParameters->verbose)
    {
        fprintf(stdout,"Saving vertices. ");
    }

    nX=pParameters->n_x;
    nY=pParameters->n_y;
    nZ=pParameters->n_z;

    counter=nX*nY*nZ;
    if (pMesh->nver!=counter)
    {
        PRINT_ERROR("In meshCube: expecting ");
        fprintf(stderr,"%d vertices instead of %d saved ",counter,pMesh->nver);
        fprintf(stderr,"in the structure pointed by pMesh.\n");
        return 0;
    }

    counter=0;
    for (i=0; i<nX; i++)
    {
        for (j=0; j<nY; j++)
        {
            for (k=0; k<nZ; k++)
            {
                // Saving the current point
                pMesh->pver[counter].x=pParameters->x_min+
                                                         i*pParameters->delta_x;
                pMesh->pver[counter].y=pParameters->y_min+
                                                         j*pParameters->delta_y;
                pMesh->pver[counter].z=pParameters->z_min+
                                                         k*pParameters->delta_z;
                pMesh->pver[counter].label=labelPoint(i,j,k,nX,nY,nZ);
                counter++;
            }
        }
    }

    if (pParameters->opt_mode<=0)
    {
        // Saving the hexahedra corresponding to each cube cell. Point are
        // saved as 1+(a*nY+b)*nZ+c a=0...nX-1, b=0...nY-1, c=0...nZ-1
        // Point reference (a,b,c) in a cube cell: 1=(i-1,j-1,k-1);
        // 2=(i,j-1,k-1); 3=(i,j,k-1); 4=(i-1,j,k-1); 5=(i-1,j-1,k); 6=(i,j-1,k)
        // 7=(i,j,k); 8=(i-1,j,k); i=1...nX-1, j=1...nY-1, k=1...nZ-1
        if (pParameters->verbose)
        {
            fprintf(stdout,"Saving hexahedra. ");
        }

        counter=(nX-1)*(nY-1)*(nZ-1);
        if (pMesh->nhex!=counter)
        {
            PRINT_ERROR("In meshCube: expecting ");
            fprintf(stderr,"%d hexahedra instead of %d ",counter,pMesh->nhex);
            fprintf(stderr,"saved in the structure pointed by pMesh.\n");
            return 0;
        }

        counter=0;
        for (i=1; i<nX; i++)
        {
            for (j=1; j<nY; j++)
            {
                for (k=1; k<nZ; k++)
                {
                    pMesh->phex[counter].p1=1+((i-1)*nY+(j-1))*nZ+(k-1);
                    pMesh->phex[counter].p2=1+(i*nY+(j-1))*nZ+(k-1);
                    pMesh->phex[counter].p3=1+(i*nY+j)*nZ+(k-1);
                    pMesh->phex[counter].p4=1+((i-1)*nY+j)*nZ+(k-1);
                    pMesh->phex[counter].p5=1+((i-1)*nY+(j-1))*nZ+k;
                    pMesh->phex[counter].p6=1+(i*nY+(j-1))*nZ+k;
                    pMesh->phex[counter].p7=1+(i*nY+j)*nZ+k;
                    pMesh->phex[counter].p8=1+((i-1)*nY+j)*nZ+k;
                    pMesh->phex[counter].label=0;
                    counter++;
                }
            }
        }

        // Saving the boundary quadrilaterals on each faces (trigonometric
        // orientation in front of it looking from the exterior of the cube)
        if (pParameters->verbose)
        {
            fprintf(stdout,"Saving boundary quadrilaterals.\n");
        }

        counter=2*((nX-1)*(nY-1)+(nX-1)*(nZ-1)+(nY-1)*(nZ-1));
        if (pMesh->nqua!=counter)
        {
            PRINT_ERROR("In meshCube: expecting ");
            fprintf(stderr,"%d quadrilaterals instead of ",counter);
            fprintf(stderr,"%d saved in the structure pointed by ",pMesh->nqua);
            fprintf(stderr,"pMesh.\n");
            return 0;
        }


        counter=0;
        // Face 1 (j=0) with reference (a,b,c) in each quadrilateral related to
        // the square cell: 1=(i-1,k-1); 2=(i,k-1); 3=(i,k); 4=(i-1,k)
        for (i=1; i<nX; i++)
        {
            for (k=1; k<nZ; k++)
            {
                // Square 1234
                pMesh->pqua[counter].p1=1+(i-1)*nY*nZ+(k-1);
                pMesh->pqua[counter].p2=1+i*nY*nZ+(k-1);
                pMesh->pqua[counter].p3=1+i*nY*nZ+k;
                pMesh->pqua[counter].p4=1+(i-1)*nY*nZ+k;
                pMesh->pqua[counter].label=1;
                counter++;
            }
        }

        // Face 2 (i=nX-1) with reference (a,b,c) in each quadrilateral related
        // to the square cell: 1=(j-1,k-1); 2=(j,k-1); 3=(j,k); 4=(j-1,k)
        for (j=1; j<nY; j++)
        {
            for (k=1; k<nZ; k++)
            {
                // Square 1234
                pMesh->pqua[counter].p1=1+((nX-1)*nY+(j-1))*nZ+(k-1);
                pMesh->pqua[counter].p2=1+((nX-1)*nY+j)*nZ+(k-1);
                pMesh->pqua[counter].p3=1+((nX-1)*nY+j)*nZ+k;
                pMesh->pqua[counter].p4=1+((nX-1)*nY+(j-1))*nZ+k;
                pMesh->pqua[counter].label=2;
                counter++;
            }
        }

        // Face 3 (j=nY-1) with reference (a,b,c) in each quadrilateral related
        // to the square cell: 1=(i-1,k-1); 2=(i,k-1); 3=(i,k); 4=(i-1,k)
        for (i=1; i<nX; i++)
        {
            for (k=1; k<nZ; k++)
            {
                // Square 1432
                pMesh->pqua[counter].p1=1+((i-1)*nY+(nY-1))*nZ+(k-1);
                pMesh->pqua[counter].p2=1+((i-1)*nY+(nY-1))*nZ+k;
                pMesh->pqua[counter].p3=1+(i*nY+(nY-1))*nZ+k;
                pMesh->pqua[counter].p4=1+(i*nY+(nY-1))*nZ+(k-1);
                pMesh->pqua[counter].label=3;
                counter++;
            }
        }

        // Face 4 (i=0) with reference (a,b,c) in each quadrilateral related to
        // the square cell: 1=(j-1,k-1); 2=(j,k-1); 3=(j,k); 4=(j-1,k)
        for (j=1; j<nY; j++)
        {
            for (k=1; k<nZ; k++)
            {
                // Square 1432
                pMesh->pqua[counter].p1=1+(j-1)*nZ+(k-1);
                pMesh->pqua[counter].p2=1+(j-1)*nZ+k;
                pMesh->pqua[counter].p3=1+j*nZ+k;
                pMesh->pqua[counter].p4=1+j*nZ+(k-1);
                pMesh->pqua[counter].label=4;
                counter++;
            }
        }


        // Face 5 (k=0) with reference (a,b,c) in each quadrilateral related to
        // the square cell: 1=(i-1,j-1); 2=(i,j-1); 3=(i,j); 4=(i-1,j)
        for (i=1; i<nX; i++)
        {
            for (j=1; j<nY; j++)
            {
                // Square 1432
                pMesh->pqua[counter].p1=1+((i-1)*nY+(j-1))*nZ;
                pMesh->pqua[counter].p2=1+((i-1)*nY+j)*nZ;
                pMesh->pqua[counter].p3=1+(i*nY+j)*nZ;
                pMesh->pqua[counter].p4=1+(i*nY+(j-1))*nZ;
                pMesh->pqua[counter].label=5;
                counter++;
            }
        }

        // Face 6 (k=nZ-1) with reference (a,b,c) in each quadrilateral related
        // to the square cell: 1=(i-1,j-1); 2=(i,j-1); 3=(i,j); 4=(i-1,j)
        for (i=1; i<nX; i++)
        {
            for (j=1; j<nY; j++)
            {
                // Square 1234
                pMesh->pqua[counter].p1=1+((i-1)*nY+(j-1))*nZ+(nZ-1);
                pMesh->pqua[counter].p2=1+(i*nY+(j-1))*nZ+(nZ-1);
                pMesh->pqua[counter].p3=1+(i*nY+j)*nZ+(nZ-1);
                pMesh->pqua[counter].p4=1+((i-1)*nY+j)*nZ+(nZ-1);
                pMesh->pqua[counter].label=6;
                counter++;
            }
        }
    }
    else
    {
        // Saving six tetrahedra (positively oriented) inside each cube cell.
        // Point is saved as 1+(a*nY+b)*nZ+c a=0...nX-1, b=0...nY-1, c=0...nZ-1
        // Point reference (a,b,c) in cube cell: 1=(i-1,j-1,k-1); 2=(i,j-1,k-1);
        // 3=(i,j,k-1); 4=(i-1,j,k-1); 5=(i-1,j-1,k); 6=(i,j-1,k); 7=(i,j,k)
        // 8=(i-1,j,k); i=1...nX-1, j=1...nY-1, k=1...nZ-1 (warning here:
        // tetrahedra must respect adjacency between different cube cells
        // (a symmetric cutting inside each cube cell is necessary)

        if (pParameters->verbose)
        {
            fprintf(stdout,"Saving tetrahedra. ");
        }

        counter=6*(nX-1)*(nY-1)*(nZ-1);
        if (pMesh->ntet!=counter)
        {
            PRINT_ERROR("In meshCube: expecting ");
            fprintf(stderr,"%d tetrahedra instead of %d ",counter,pMesh->ntet);
            fprintf(stderr,"saved in the structure pointed by pMesh.\n");
            return 0;
        }


        counter=0;
        for (i=1; i<nX; i++)
        {
            for (j=1; j<nY; j++)
            {
                for (k=1; k<nZ; k++)
                {
                    // Tetrahedron 1524
                    pMesh->ptet[counter].p1=1+((i-1)*nY+(j-1))*nZ+(k-1);
                    pMesh->ptet[counter].p2=1+((i-1)*nY+(j-1))*nZ+k;
                    pMesh->ptet[counter].p3=1+(i*nY+(j-1))*nZ+(k-1);
                    pMesh->ptet[counter].p4=1+((i-1)*nY+j)*nZ+(k-1);
                    pMesh->ptet[counter].label=0;
                    counter++;

                    // Tetrahedron 6254
                    pMesh->ptet[counter].p1=1+(i*nY+(j-1))*nZ+k;
                    pMesh->ptet[counter].p2=1+(i*nY+(j-1))*nZ+(k-1);
                    pMesh->ptet[counter].p3=1+((i-1)*nY+(j-1))*nZ+k;
                    pMesh->ptet[counter].p4=1+((i-1)*nY+j)*nZ+(k-1);
                    pMesh->ptet[counter].label=0;
                    counter++;

                    // Tetrahedron 2346
                    pMesh->ptet[counter].p1=1+(i*nY+(j-1))*nZ+(k-1);
                    pMesh->ptet[counter].p2=1+(i*nY+j)*nZ+(k-1);
                    pMesh->ptet[counter].p3=1+((i-1)*nY+j)*nZ+(k-1);
                    pMesh->ptet[counter].p4=1+(i*nY+(j-1))*nZ+k;
                    pMesh->ptet[counter].label=0;
                    counter++;

                    // Tetrahedron 7836
                    pMesh->ptet[counter].p1=1+(i*nY+j)*nZ+k;
                    pMesh->ptet[counter].p2=1+((i-1)*nY+j)*nZ+k;
                    pMesh->ptet[counter].p3=1+(i*nY+j)*nZ+(k-1);
                    pMesh->ptet[counter].p4=1+(i*nY+(j-1))*nZ+k;
                    pMesh->ptet[counter].label=0;
                    counter++;

                    // Tetrahedron 6483
                    pMesh->ptet[counter].p1=1+(i*nY+(j-1))*nZ+k;
                    pMesh->ptet[counter].p2=1+((i-1)*nY+j)*nZ+(k-1);
                    pMesh->ptet[counter].p3=1+((i-1)*nY+j)*nZ+k;
                    pMesh->ptet[counter].p4=1+(i*nY+j)*nZ+(k-1);
                    pMesh->ptet[counter].label=0;
                    counter++;

                    // Tetrahedron 8654
                    pMesh->ptet[counter].p1=1+((i-1)*nY+j)*nZ+k;
                    pMesh->ptet[counter].p2=1+(i*nY+(j-1))*nZ+k;
                    pMesh->ptet[counter].p3=1+((i-1)*nY+(j-1))*nZ+k;
                    pMesh->ptet[counter].p4=1+((i-1)*nY+j)*nZ+(k-1);
                    pMesh->ptet[counter].label=0;
                    counter++;
                }
            }
        }

        // Saving the boundary triangles on each faces (trigonometric
        // orientation in front of it looking from the exterior of the cube)
        if (pParameters->verbose)
        {
            fprintf(stdout,"Saving boundary triangles.\n");
        }

        counter=4*((nX-1)*(nY-1)+(nX-1)*(nZ-1)+(nY-1)*(nZ-1));
        if (pMesh->ntri!=counter)
        {
            PRINT_ERROR("In meshCube: expecting ");
            fprintf(stderr,"%d triangles instead of %d ",counter,pMesh->ntri);
            fprintf(stderr,"saved in the structure pointed by pMesh.\n");
            return 0;
        }

        counter=0;
        // Face 1 (j=0) with reference (a,b,c) in each quadrilateral related to
        // the square cell: 1=(i-1,k-1); 2=(i,k-1); 3=(i,k); 4=(i-1,k)
        for (i=1; i<nX; i++)
        {
            for (k=1; k<nZ; k++)
            {
                // Triangle 124
                pMesh->ptri[counter].p1=1+(i-1)*nY*nZ+(k-1);
                pMesh->ptri[counter].p2=1+i*nY*nZ+(k-1);
                pMesh->ptri[counter].p3=1+(i-1)*nY*nZ+k;
                pMesh->ptri[counter].label=1;
                counter++;

                // Triangle 234
                pMesh->ptri[counter].p1=1+i*nY*nZ+(k-1);
                pMesh->ptri[counter].p2=1+i*nY*nZ+k;
                pMesh->ptri[counter].p3=1+(i-1)*nY*nZ+k;
                pMesh->ptri[counter].label=1;
                counter++;
            }
        }

        // Face 2 (i=nX-1) with reference (a,b,c) in each quadrilateral related
        // to the square cell: 1=(j-1,k-1); 2=(j,k-1); 3=(j,k); 4=(j-1,k)
        for (j=1; j<nY; j++)
        {
            for (k=1; k<nZ; k++)
            {
                // Triangle 124
                pMesh->ptri[counter].p1=1+((nX-1)*nY+(j-1))*nZ+(k-1);
                pMesh->ptri[counter].p2=1+((nX-1)*nY+j)*nZ+(k-1);
                pMesh->ptri[counter].p3=1+((nX-1)*nY+(j-1))*nZ+k;
                pMesh->ptri[counter].label=2;
                counter++;

                // Triangle 234
                pMesh->ptri[counter].p1=1+((nX-1)*nY+j)*nZ+(k-1);
                pMesh->ptri[counter].p2=1+((nX-1)*nY+j)*nZ+k;
                pMesh->ptri[counter].p3=1+((nX-1)*nY+(j-1))*nZ+k;
                pMesh->ptri[counter].label=2;
                counter++;
            }
        }

        // Face 3 (j=nY-1) with reference (a,b,c) in each quadrilateral related
        // to the square cell: 1=(i-1,k-1); 2=(i,k-1); 3=(i,k); 4=(i-1,k)
        for (i=1; i<nX; i++)
        {
            for (k=1; k<nZ; k++)
            {
                // Triangle 142
                pMesh->ptri[counter].p1=1+((i-1)*nY+(nY-1))*nZ+(k-1);
                pMesh->ptri[counter].p2=1+((i-1)*nY+(nY-1))*nZ+k;
                pMesh->ptri[counter].p3=1+(i*nY+(nY-1))*nZ+(k-1);
                pMesh->ptri[counter].label=3;
                counter++;

                // Triangle 243
                pMesh->ptri[counter].p1=1+(i*nY+(nY-1))*nZ+(k-1);
                pMesh->ptri[counter].p2=1+((i-1)*nY+(nY-1))*nZ+k;
                pMesh->ptri[counter].p3=1+(i*nY+(nY-1))*nZ+k;
                pMesh->ptri[counter].label=3;
                counter++;
            }
        }

        // Face 4 (i=0) with reference (a,b,c) in each quadrilateral related to
        // the square cell: 1=(j-1,k-1); 2=(j,k-1); 3=(j,k); 4=(j-1,k)
        for (j=1; j<nY; j++)
        {
            for (k=1; k<nZ; k++)
            {
                // Triangle 142
                pMesh->ptri[counter].p1=1+(j-1)*nZ+(k-1);
                pMesh->ptri[counter].p2=1+(j-1)*nZ+k;
                pMesh->ptri[counter].p3=1+j*nZ+(k-1);
                pMesh->ptri[counter].label=4;
                counter++;

                // Triangle 243
                pMesh->ptri[counter].p1=1+j*nZ+(k-1);
                pMesh->ptri[counter].p2=1+(j-1)*nZ+k;
                pMesh->ptri[counter].p3=1+j*nZ+k;
                pMesh->ptri[counter].label=4;
                counter++;
            }
        }

        // Face 5 (k=0) with reference (a,b,c) in each quadrilateral related to
        // the square cell: 1=(i-1,j-1); 2=(i,j-1); 3=(i,j); 4=(i-1,j)
        for (i=1; i<nX; i++)
        {
            for (j=1; j<nY; j++)
            {
                // Triangle 142
                pMesh->ptri[counter].p1=1+((i-1)*nY+(j-1))*nZ;
                pMesh->ptri[counter].p2=1+((i-1)*nY+j)*nZ;
                pMesh->ptri[counter].p3=1+(i*nY+(j-1))*nZ;
                pMesh->ptri[counter].label=5;
                counter++;

                // Triangle 243
                pMesh->ptri[counter].p1=1+(i*nY+(j-1))*nZ;
                pMesh->ptri[counter].p2=1+((i-1)*nY+j)*nZ;
                pMesh->ptri[counter].p3=1+(i*nY+j)*nZ;
                pMesh->ptri[counter].label=5;
                counter++;
            }
        }

        // Face 6 (k=nZ-1) with reference (a,b,c) in each quadrilateral related
        // to the square cell: 1=(i-1,j-1); 2=(i,j-1); 3=(i,j); 4=(i-1,j)
        for (i=1; i<nX; i++)
        {
            for (j=1; j<nY; j++)
            {
                // Triangle 124
                pMesh->ptri[counter].p1=1+((i-1)*nY+(j-1))*nZ+(nZ-1);
                pMesh->ptri[counter].p2=1+(i*nY+(j-1))*nZ+(nZ-1);
                pMesh->ptri[counter].p3=1+((i-1)*nY+j)*nZ+(nZ-1);
                pMesh->ptri[counter].label=6;
                counter++;

                // Triangle 234
                pMesh->ptri[counter].p1=1+(i*nY+(j-1))*nZ+(nZ-1);
                pMesh->ptri[counter].p2=1+(i*nY+j)*nZ+(nZ-1);
                pMesh->ptri[counter].p3=1+((i-1)*nY+j)*nZ+(nZ-1);
                pMesh->ptri[counter].label=6;
                counter++;
            }
        }

        // Saving the boundary edges called ridges and labelled
        // 10*face1+ face2 (with label face1<face2)
        if (pParameters->verbose)
        {
            fprintf(stdout,"Saving geometry (ridges, edges, corners, normal ");
            fprintf(stdout,"and tangent vectors).\n");
        }

        counter=4*((nX-1)+(nY-1)+(nZ-1));
        if (pMesh->nedg!=counter)
        {
            PRINT_ERROR("In meshCube: expecting ");
            fprintf(stderr,"%d edges instead of %d ",counter,pMesh->nedg);
            fprintf(stderr,"saved in the structure pointed by pMesh.\n");
            return 0;
        }

        counter=0;
        for (i=1; i<nX; i++)
        {
            // Edge 15 (j=0, k=0)
            pMesh->pedg[counter].p1=1+(i-1)*nY*nZ;
            pMesh->pedg[counter].p2=1+i*nY*nZ;
            pMesh->pedg[counter].label=15;
            counter++;

            // Edge 35 (j=nY-1, k=0)
            pMesh->pedg[counter].p1=1+((i-1)*nY+(nY-1))*nZ;
            pMesh->pedg[counter].p2=1+(i*nY+(nY-1))*nZ;
            pMesh->pedg[counter].label=35;
            counter++;

            // Edge 36 (j=nY-1, k=nZ-1)
            pMesh->pedg[counter].p1=1+((i-1)*nY+(nY-1))*nZ+(nZ-1);
            pMesh->pedg[counter].p2=1+(i*nY+(nY-1))*nZ+(nZ-1);
            pMesh->pedg[counter].label=36;
            counter++;

            // Edge 16 (j=0, k=nZ-1)
            pMesh->pedg[counter].p1=1+(i-1)*nY*nZ+(nZ-1);
            pMesh->pedg[counter].p2=1+i*nY*nZ+(nZ-1);
            pMesh->pedg[counter].label=16;
            counter++;
        }

        for (j=1; j<nY; j++)
        {
            // Edge 45 (i=0, k=0)
            pMesh->pedg[counter].p1=1+(j-1)*nZ;
            pMesh->pedg[counter].p2=1+j*nZ;
            pMesh->pedg[counter].label=45;
            counter++;

            // Edge 25 (i=nX-1, k=0)
            pMesh->pedg[counter].p1=1+((nX-1)*nY+(j-1))*nZ;
            pMesh->pedg[counter].p2=1+((nX-1)*nY+j)*nZ;
            pMesh->pedg[counter].label=25;
            counter++;

            // Edge 26 (i=nX-1, k=nZ-1)
            pMesh->pedg[counter].p1=1+((nX-1)*nY+(j-1))*nZ+(nZ-1);
            pMesh->pedg[counter].p2=1+((nX-1)*nY+j)*nZ+(nZ-1);
            pMesh->pedg[counter].label=26;
            counter++;

            // Edge 46 (i=0, k=nZ-1)
            pMesh->pedg[counter].p1=1+(j-1)*nZ+(nZ-1);
            pMesh->pedg[counter].p2=1+j*nZ+(nZ-1);
            pMesh->pedg[counter].label=46;
            counter++;
        }

        for (k=1; k<nZ; k++)
        {
            // Edge 14 (i=0, j=0)
            pMesh->pedg[counter].p1=1+(k-1);
            pMesh->pedg[counter].p2=1+k;
            pMesh->pedg[counter].label=14;
            counter++;

            // Edge 12 (i=nX-1, j=0)
            pMesh->pedg[counter].p1=1+(nX-1)*nY*nZ+(k-1);
            pMesh->pedg[counter].p2=1+(nX-1)*nY*nZ+k;
            pMesh->pedg[counter].label=12;
            counter++;

            // Edge 23 (i=nX-1, j=nY-1)
            pMesh->pedg[counter].p1=1+((nX-1)*nY+(nY-1))*nZ+(k-1);
            pMesh->pedg[counter].p2=1+((nX-1)*nY+(nY-1))*nZ+k;
            pMesh->pedg[counter].label=23;
            counter++;

            // Edge 34 (i=0, j=nY-1)
            pMesh->pedg[counter].p1=1+(nY-1)*nZ+(k-1);
            pMesh->pedg[counter].p2=1+(nY-1)*nZ+k;
            pMesh->pedg[counter].label=34;
            counter++;
        }

        // Saving the normals (pointing outwards the cube)
        counter=2*((nX-2)*(nY-2)+(nX-2)*(nZ-2)+(nY-2)*(nZ-2));
        if (pMesh->nnorm!=counter)
        {
            PRINT_ERROR("In meshCube: expecting ");
            fprintf(stderr,"%d normal vectors instead of ",counter);
            fprintf(stderr,"%d saved in the structure pointed ",pMesh->nnorm);
            fprintf(stderr,"by pMesh.\n");
            return 0;
        }

        counter=0;
        // Face 1 (j=0)
        for (i=1; i<nX-1; i++)
        {
            for (k=1; k<nZ-1; k++)
            {
                pMesh->pnorm[counter].x=0.;
                pMesh->pnorm[counter].y=-1.;
                pMesh->pnorm[counter].z=0.;
                pMesh->pnorm[counter].p=1+i*nY*nZ+k;
                counter++;
            }
        }

        // Face 2 (i=nX-1)
        for (j=1; j<nY-1; j++)
        {
            for (k=1; k<nZ-1; k++)
            {
                pMesh->pnorm[counter].x=1.;
                pMesh->pnorm[counter].y=0.;
                pMesh->pnorm[counter].z=0.;
                pMesh->pnorm[counter].p=1+((nX-1)*nY+j)*nZ+k;
                counter++;
            }
        }

        // Face 3 (j=nY-1)
        for (i=1; i<nX-1; i++)
        {
            for (k=1; k<nZ-1; k++)
            {
                pMesh->pnorm[counter].x=0.;
                pMesh->pnorm[counter].y=1.;
                pMesh->pnorm[counter].z=0.;
                pMesh->pnorm[counter].p=1+(i*nY+(nY-1))*nZ+k;
                counter++;
            }
        }

        // Face 4 (i=0)
        for (j=1; j<nY-1; j++)
        {
            for (k=1; k<nZ-1; k++)
            {
                pMesh->pnorm[counter].x=-1.;
                pMesh->pnorm[counter].y=0.;
                pMesh->pnorm[counter].z=0.;
                pMesh->pnorm[counter].p=1+j*nZ+k;
                counter++;
            }
        }

        // Face 5 (k=0)
         for (i=1; i<nX-1; i++)
        {
            for (j=1; j<nY-1; j++)
            {
                pMesh->pnorm[counter].x=0.;
                pMesh->pnorm[counter].y=0.;
                pMesh->pnorm[counter].z=-1.;
                pMesh->pnorm[counter].p=1+(i*nY+j)*nZ;
                counter++;
            }
        }

        // Face 6 (k=nZ-1)
         for (i=1; i<nX-1; i++)
        {
            for (j=1; j<nY-1; j++)
            {
                pMesh->pnorm[counter].x=0.;
                pMesh->pnorm[counter].y=0.;
                pMesh->pnorm[counter].z=1.;
                pMesh->pnorm[counter].p=1+(i*nY+j)*nZ+(nZ-1);
                counter++;
            }
        }

        // Saving the tangents
        counter=4*((nX-2)+(nY-2)+(nZ-2));
        if (pMesh->ntan!=counter)
        {
            PRINT_ERROR("In meshCube: expecting ");
            fprintf(stderr,"%d tangent vectors instead of ",counter);
            fprintf(stderr,"%d saved in the structure pointed ",pMesh->ntan);
            fprintf(stderr,"by pMesh.\n");
            return 0;
        }

        counter=0;
        for (i=1; i<nX-1; i++)
        {
            // Edge 1 (j=0, k=0)
            pMesh->ptan[counter].x=1.;
            pMesh->ptan[counter].y=0.;
            pMesh->ptan[counter].z=0.;
            pMesh->ptan[counter].p=1+i*nY*nZ;
            counter++;

            // Edge 2 (j=nY-1, k=0)
            pMesh->ptan[counter].x=1.;
            pMesh->ptan[counter].y=0.;
            pMesh->ptan[counter].z=0.;
            pMesh->ptan[counter].p=1+(i*nY+(nY-1))*nZ;
            counter++;

            // Edge 3 (j=nY-1, k=nZ-1)
            pMesh->ptan[counter].x=1.;
            pMesh->ptan[counter].y=0.;
            pMesh->ptan[counter].z=0.;
            pMesh->ptan[counter].p=1+(i*nY+(nY-1))*nZ+(nZ-1);
            counter++;

            // Edge 4 (j=0,k=nZ-1)
            pMesh->ptan[counter].x=1.;
            pMesh->ptan[counter].y=0.;
            pMesh->ptan[counter].z=0.;
            pMesh->ptan[counter].p=1+i*nY*nZ+(nZ-1);
            counter++;
        }

        for (j=1; j<nY-1; j++)
        {
            // Edge 5 (i=0, k=0)
            pMesh->ptan[counter].x=0.;
            pMesh->ptan[counter].y=1.;
            pMesh->ptan[counter].z=0.;
            pMesh->ptan[counter].p=1+j*nZ;
            counter++;

            // Edge 6 (i=nX-1, k=0)
            pMesh->ptan[counter].x=0.;
            pMesh->ptan[counter].y=1.;
            pMesh->ptan[counter].z=0.;
            pMesh->ptan[counter].p=1+((nX-1)*nY+j)*nZ;
            counter++;

            // Edge 7 (i=nX-1, k=nZ-1)
            pMesh->ptan[counter].x=0.;
            pMesh->ptan[counter].y=1.;
            pMesh->ptan[counter].z=0.;
            pMesh->ptan[counter].p=1+((nX-1)*nY+j)*nZ+(nZ-1);
            counter++;

            // Edge 8 (i=0, k=nZ-1)
            pMesh->ptan[counter].x=0.;
            pMesh->ptan[counter].y=1.;
            pMesh->ptan[counter].z=0.;
            pMesh->ptan[counter].p=1+j*nZ+(nZ-1);
            counter++;
        }

        for (k=1; k<nZ-1; k++)
        {
            // Edge 9 (i=0, j=0)
            pMesh->ptan[counter].x=0.;
            pMesh->ptan[counter].y=0.;
            pMesh->ptan[counter].z=1.;
            pMesh->ptan[counter].p=1+k;
            counter++;

            // Edge 10 (i=nX-1, j=0)
            pMesh->ptan[counter].x=0.;
            pMesh->ptan[counter].y=0.;
            pMesh->ptan[counter].z=1.;
            pMesh->ptan[counter].p=1+(nX-1)*nY*nZ+k;
            counter++;

            // Edge 11 (i=nX-1, j=nY-1)
            pMesh->ptan[counter].x=0.;
            pMesh->ptan[counter].y=0.;
            pMesh->ptan[counter].z=1.;
            pMesh->ptan[counter].p=1+((nX-1)*nY+(nY-1))*nZ+k;
            counter++;

            // Edge 12 (i=0, j=nY-1)
            pMesh->ptan[counter].x=0.;
            pMesh->ptan[counter].y=0.;
            pMesh->ptan[counter].z=1.;
            pMesh->ptan[counter].p=1+(nY-1)*nZ+k;
            counter++;
        }
    }

    return 1;
}

/* ************************************************************************** */
// The function writingMeshFile writes the mesh discretization data saved in the
// structure pointed by pMesh and writes it in a *.mesh file according to the
// parameters stored in the structure pointed by pParameters. If
// pParameters->name_mesh is set to NULL, a default name is generated by
// changing the *.info extension given in pParameters->name_info into the *.mesh
// one. If pParameters->name_mesh stores the name of a *.cube file, the same
// name is used by changing the *.cube extension into the *.mesh one. Otherwise,
// the pParameters->name_mesh variable must store the name of a valid *.mesh
// file which is used as the name of the file we aim to write (warning here:
// reset and overwrite the file if it already exists). It has the Parameters*,
// and Mesh* variables (both defined in main.h) as input arguments and it
// returns one on success otherwise zero is returned if an error is encountered
/* ************************************************************************** */
int writingMeshFile(Parameters* pParameters, Mesh* pMesh)
{
    size_t lengthName=0;
    char *fileLocation=NULL;
    int i=0, iMax=0;
    double delta=0.;
    FILE *meshFile=NULL;

    Point *pVertex=NULL;
    Vector *pNormal=NULL, *pTangent=NULL;
    Edge *pEdge=NULL;
    Triangle *pTriangle=NULL;
    Tetrahedron *pTetrahedron=NULL;
    Quadrilateral *pQuadrilateral=NULL;
    Hexahedron *pHexahedron=NULL;

    // Check if the input variables are not pointing to NULL
    if (pParameters==NULL || pMesh==NULL)
    {
        PRINT_ERROR("In writingMeshFile: one of the input variables ");
        fprintf(stderr,"(pParameters=%p, ",(void*)pParameters);
        fprintf(stderr,"pMesh=%p) does not have a valid ",(void*)pMesh);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Check if memory has been allocated for the different types of elements
    if (pParameters->opt_mode>0)
    {
        if (pMesh->pver==NULL || pMesh->ptri==NULL || pMesh->ptet==NULL ||
                                       (pMesh->nnorm>0 && pMesh->pnorm==NULL) ||
                                         (pMesh->ntan>0 && pMesh->ptan==NULL) ||
                                           (pMesh->nedg>0 && pMesh->pedg==NULL))
        {
            PRINT_ERROR("In writingMeshFile: one of the following pointer\n");
            fprintf(stderr,"pMesh->pver=%p\n",(void*)pMesh->pver);
            if (pMesh->nnorm>0)
            {
                fprintf(stderr,"pMesh->pnorm=%p\n",(void*)pMesh->pnorm);
            }
            if (pMesh->ntan>0)
            {
                fprintf(stderr,"pMesh->ptan=%p\n",(void*)pMesh->ptan);
            }
            if (pMesh->nedg>0)
            {
                fprintf(stderr,"pMesh->pedg=%p\n",(void*)pMesh->pedg);
            }
            fprintf(stderr,"pMesh->ptri=%p\n",(void*)pMesh->ptri);
            fprintf(stderr,"pMesh->ptet=%p\n",(void*)pMesh->ptet);
            fprintf(stderr,"does not have a valid allocated adress.\n");
            return 0;
        }
    }
    else
    {
        if (pMesh->pver==NULL || pMesh->pqua==NULL || pMesh->phex==NULL)
        {
            PRINT_ERROR("In writingMeshFile: one of the following pointer\n");
            fprintf(stderr,"pMesh->pver=%p\n",(void*)pMesh->pver);
            fprintf(stderr,"pMesh->pqua=%p\n",(void*)pMesh->pqua);
            fprintf(stderr,"pMesh->phex=%p\n",(void*)pMesh->phex);
            fprintf(stderr,"does not have a valid allocated adress.\n");
            return 0;
        }
    }

    // Check pParameters->name_length variable
    if (pParameters->name_length<7)
    {
        PRINT_ERROR("In writingMeshFile: the variable name_length ");
        fprintf(stderr,"(=%d) of the structure ",pParameters->name_length);
        fprintf(stderr,"pointed by pParameters must be an integer (strictly) ");
        fprintf(stderr,"greater than six (to store at least something more ");
        fprintf(stderr,"than the *.mesh extension).\n");
        return 0;
    }

    // Dynamically allocate memory for the local fileLocation variable
    // calloc function returns a pointer to the allocated memory, otherwise NULL
    lengthName=pParameters->name_length;
    fileLocation=(char*)calloc(lengthName,sizeof(char));
    if (fileLocation==NULL)
    {
        PRINT_ERROR("In writingMeshFile: could not allocate memory for the ");
        fprintf(stderr,"char* fileLocation (local) variable.\n");
        return 0;
    }

    if (pParameters->name_mesh==NULL)
    {
        // Allocate memory for pParameters->name_mesh and check if it worked
        lengthName=pParameters->name_length;
        pParameters->name_mesh=(char*)calloc(lengthName,sizeof(char));
        if (pParameters->name_mesh==NULL)
        {
            PRINT_ERROR("In writingMeshFile: could not allocate memory for ");
            fprintf(stderr,"the char* pParameters->name_mesh variable.\n");
            free(fileLocation); // free function does not return any value
            fileLocation=NULL;
            return 0;
        }

        // Check pParameters->name_info variable (used to set the default name)
        if (!checkStringFromLength(pParameters->name_info,7,
                                                      pParameters->name_length))
        {
            PRINT_ERROR("In writingMeshFile: checkStringFromLength function ");
            fprintf(stderr,"returned zero, which is not the expected value ");
            fprintf(stderr,"here, after having checked that the char* ");
            fprintf(stderr,"name_info variable of the structure pointed by ");
            fprintf(stderr,"pParameters is not a string of length (strictly) ");
            fprintf(stderr,"less than %d (and more ",pParameters->name_length);
            fprintf(stderr,"than 5 in order to store at least something more ");
            fprintf(stderr,"than the *.info extension).\n");
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        // Check if the *.info file name ends with the ".info" extension
        // strlen function returns the length of the string (not including '\0')
        lengthName=strlen(pParameters->name_info);
        if (pParameters->name_info[lengthName-5]!='.' ||
                                    pParameters->name_info[lengthName-4]!='i' ||
                                    pParameters->name_info[lengthName-3]!='n' ||
                                    pParameters->name_info[lengthName-2]!='f' ||
                                    pParameters->name_info[lengthName-1]!='o' ||
                                       pParameters->name_info[lengthName]!='\0')
        {
            PRINT_ERROR("In writingMeshFile: ");
            fprintf(stderr,"%s file name does not ",pParameters->name_info);
            fprintf(stderr,"end with with the '.info' extension.\n");
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        // Copy the *.info name into pParameters->name_mesh and change for the
        // *.mesh extension: strncpy returns a pointer to the string
        strncpy(pParameters->name_mesh,pParameters->name_info,
                                                      pParameters->name_length);
        pParameters->name_mesh[lengthName-5]='.';
        pParameters->name_mesh[lengthName-4]='m';
        pParameters->name_mesh[lengthName-3]='e';
        pParameters->name_mesh[lengthName-2]='s';
        pParameters->name_mesh[lengthName-1]='h';
        pParameters->name_mesh[lengthName]='\0';

        // Copy the default *.mesh name into fileLocation
        strncpy(fileLocation,pParameters->name_mesh,pParameters->name_length);
    }
    else
    {
        // Check pParameters->name_mesh variable
        if (!checkStringFromLength(pParameters->name_mesh,7,
                                                      pParameters->name_length))
        {
            PRINT_ERROR("In writingMeshFile: checkStringFromLength function ");
            fprintf(stderr,"returned zero, which is not the expected value ");
            fprintf(stderr,"here, after having checked that the char* ");
            fprintf(stderr,"name_mesh variable of the structure pointed by ");
            fprintf(stderr,"pParameters is not a string of length (strictly) ");
            fprintf(stderr,"less than %d (and more ",pParameters->name_length);
            fprintf(stderr,"than 5 in order to store at least something more ");
            fprintf(stderr,"than the *.mesh or *.cube extension).\n");
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        // Check if the *.mesh file name ends with the *.cube extension or not
        lengthName=strlen(pParameters->name_mesh);
        if (pParameters->name_mesh[lengthName-5]=='.' &&
                                    pParameters->name_mesh[lengthName-4]=='c' &&
                                    pParameters->name_mesh[lengthName-3]=='u' &&
                                    pParameters->name_mesh[lengthName-2]=='b' &&
                                    pParameters->name_mesh[lengthName-1]=='e' &&
                                       pParameters->name_mesh[lengthName]=='\0')
        {
            strncpy(fileLocation,pParameters->name_mesh,
                                                      pParameters->name_length);
            fileLocation[lengthName-5]='.';
            fileLocation[lengthName-4]='m';
            fileLocation[lengthName-3]='e';
            fileLocation[lengthName-2]='s';
            fileLocation[lengthName-1]='h';
            fileLocation[lengthName]='\0';

            // Update the name of the *.cube file so it has the *.mesh extension
            strncpy(pParameters->name_mesh,fileLocation,
                                                      pParameters->name_length);
        }
        else if (pParameters->name_mesh[lengthName-5]=='.' &&
                                    pParameters->name_mesh[lengthName-4]=='m' &&
                                    pParameters->name_mesh[lengthName-3]=='e' &&
                                    pParameters->name_mesh[lengthName-2]=='s' &&
                                    pParameters->name_mesh[lengthName-1]=='h' &&
                                       pParameters->name_mesh[lengthName]=='\0')
        {
            strncpy(fileLocation,pParameters->name_mesh,
                                                      pParameters->name_length);
        }
        else
        {
            PRINT_ERROR("In writingMeshFile: ");
            fprintf(stderr,"%s file name does not end ",pParameters->name_mesh);
            fprintf(stderr,"with with the '.mesh' or '.cube' extension.\n");
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }
    }

    // In the case of hexahedra, if pParameters->trick_matrix is set to zero
    // temporarily rename label of boundary hexahedra that are not correctly set
    if (pParameters->opt_mode==-1)
    {
        if (!pParameters->trick_matrix)
        {
            if (pMesh->nadj>0 && pMesh->padj!=NULL)
            {
                iMax=pMesh->nadj;
                for (i=0; i<iMax; i++)
                {
                    if (pMesh->phex[pMesh->padj[i].hexout-1].label==3)
                    {
                        pMesh->phex[pMesh->padj[i].hexout-1].label=-3;
                    }
                    if (pMesh->phex[pMesh->padj[i].hexin-1].label==2)
                    {
                        pMesh->phex[pMesh->padj[i].hexin-1].label=-2;
                    }
                }
            }
        }
    }

    // Opening *.mesh file (warning: reset and overwrite file if already exists)
    // fopen returns a FILE pointer on success, otherwise NULL is returned
    fprintf(stdout,"\nOpening %s file. ",fileLocation);
    meshFile=fopen(fileLocation,"w+");
    if (meshFile==NULL)
    {
        PRINT_ERROR("In writingMeshFile: we were not able to open and write ");
        fprintf(stderr,"into the %s file",fileLocation);
        free(fileLocation);
        fileLocation=NULL;
        return 0;
    }
    fprintf(stdout,"Writing mesh data. ");

    // Writing *.mesh file starting by Vertices
    fprintf(meshFile,"MeshVersionFormatted 2\n\nDimension 3\n");

    iMax=pMesh->nver;
    if (iMax<1)
    {
        PRINT_ERROR("In writingMeshFile: expecting a positive number of ");
        fprintf(stderr,"vertices instead of %d stored in the structure ",iMax);
        fprintf(stderr,"pointed by pMesh.\n");
        fprintf(meshFile,"\nEnd");
        closeTheFile(&meshFile);
        free(fileLocation);
        fileLocation=NULL;
        return 0;
    }

    fprintf(meshFile,"\nVertices\n%d\n",iMax);
    for (i=0; i<iMax; i++)
    {
        pVertex=&pMesh->pver[i];
        if (pVertex->x>pParameters->x_max || pVertex->x<pParameters->x_min ||
               pVertex->y>pParameters->y_max || pVertex->y<pParameters->y_min ||
                 pVertex->z>pParameters->z_max || pVertex->z<pParameters->z_min)
        {
            PRINT_ERROR("In writingMeshFile: the coordinates ");
            fprintf(stderr,"(%lf,%lf,%lf) ",pVertex->x,pVertex->y,pVertex->z);
            fprintf(stderr,"of the %d-th vertex shows that the ",i+1);
            fprintf(stderr,"corresponding point does not belong to the ");
            fprintf(stderr,"computational bounding box ");
            fprintf(stderr,"[%lf,%lf]x",pParameters->x_min,pParameters->x_max);
            fprintf(stderr,"[%lf,%lf]x",pParameters->y_min,pParameters->y_max);
            fprintf(stderr,"[%lf,%lf] ",pParameters->z_min,pParameters->z_max);
            fprintf(stderr,"associated with the mesh discretization stored ");
            fprintf(stderr,"in the structure pointed by pMesh.\n");
            fprintf(meshFile,"\nEnd");
            closeTheFile(&meshFile);
            free(fileLocation);
            fileLocation=NULL;
           return 0;
        }
        fprintf(meshFile,"%.8le %.8le ",pVertex->x,pVertex->y);
        fprintf(meshFile,"%.8le %d \n",pVertex->z,pVertex->label);
    }

    if (pParameters->opt_mode<=0)
    {
        // Check discretization in the first-coordinate direction
        delta=(pParameters->x_max-pParameters->x_min)/
                                                   (double)(pParameters->n_x-1);
        if (pParameters->n_x<3 || pParameters->x_min>=pParameters->x_max ||
                                    DEF_ABS(pParameters->delta_x-delta)>=1.e-16)
        {
            PRINT_ERROR("In writingMeshFile: incorrect discretization of the ");
            fprintf(stderr,"computational box in the first-coordinate ");
            fprintf(stderr," direction: expecting\n");
            fprintf(stderr,"(pParameters->n_x=%d) > 2\n",pParameters->n_x);
            fprintf(stderr,"(pParameters->x_min=%lf) < ",pParameters->x_min);
            fprintf(stderr,"(pParameters->x_max=%lf)\n",pParameters->x_max);
            fprintf(stderr,"(pParameters->delta_x=");
            fprintf(stderr,"%.18lf) == %.18lf ",pParameters->delta_x,delta);
            fprintf(stderr,"(up to 15 decimals).\n");
            fprintf(meshFile,"\nEnd");
            closeTheFile(&meshFile);
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        // Check discretization in the second-coordinate direction
        delta=(pParameters->y_max-pParameters->y_min)/
                                                   (double)(pParameters->n_y-1);
        if (pParameters->n_y<3 || pParameters->y_min>=pParameters->y_max ||
                                    DEF_ABS(pParameters->delta_y-delta)>=1.e-16)
        {
            PRINT_ERROR("In writingMeshFile: incorrect discretization of the ");
            fprintf(stderr,"computational box in the second-coordinate ");
            fprintf(stderr,"direction: expecting\n");
            fprintf(stderr,"(pParameters->n_y=%d) > 2\n",pParameters->n_y);
            fprintf(stderr,"(pParameters->y_min=%lf) < ",pParameters->y_min);
            fprintf(stderr,"(pParameters->y_max=%lf)\n",pParameters->y_max);
            fprintf(stderr,"(pParameters->delta_y=");
            fprintf(stderr,"%.18lf) == %.18lf ",pParameters->delta_y,delta);
            fprintf(stderr,"(up to 15 decimals).\n");
            fprintf(meshFile,"\nEnd");
            closeTheFile(&meshFile);
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        // Check discretization in the third-coordinate direction
        delta=(pParameters->z_max-pParameters->z_min)/
                                                   (double)(pParameters->n_z-1);
        if (pParameters->n_z<3 || pParameters->z_min>=pParameters->z_max ||
                                    DEF_ABS(pParameters->delta_z-delta)>=1.e-16)
        {
            PRINT_ERROR("In writingMeshFile: incorrect discretization of the ");
            fprintf(stderr,"computational box in the third-coordinate ");
            fprintf(stderr,"direction: expecting\n");
            fprintf(stderr,"(pParameters->n_z=%d) > 2\n",pParameters->n_z);
            fprintf(stderr,"(pParameters->z_min=%lf) < ",pParameters->z_min);
            fprintf(stderr,"(pParameters->z_max=%lf)\n",pParameters->z_max);
            fprintf(stderr,"(pParameters->delta_z=");
            fprintf(stderr,"%.18lf) == %.18lf ",pParameters->delta_z,delta);
            fprintf(stderr,"(up to 15 decimals).\n");
            fprintf(meshFile,"\nEnd");
            closeTheFile(&meshFile);
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        // Check the number of vertices in the hexahedral mesh
        iMax=pParameters->n_x*pParameters->n_y*pParameters->n_z;
        if (pMesh->nver!=iMax)
        {
            PRINT_ERROR("In writingMeshFile: expecting ");
            fprintf(stderr,"%d vertices instead of %d saved ",iMax,pMesh->nver);
            fprintf(stderr,"in the structure pointed by pMesh.\n");
            fprintf(meshFile,"\nEnd");
            closeTheFile(&meshFile);
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        // Check number of quadrilateral then write their descriptions
        iMax=(pParameters->n_x-1)*(pParameters->n_y-1);
        iMax+=(pParameters->n_x-1)*(pParameters->n_z-1);
        iMax+=(pParameters->n_y-1)*(pParameters->n_z-1);
        iMax*=2;
        if (pMesh->nqua<iMax)
        {
            PRINT_ERROR("In writingMeshFile: expecting at least ");
            fprintf(stderr,"%d quadrilaterals instead of %d ",iMax,pMesh->nqua);
            fprintf(stderr,"saved in the structure pointed by pMesh.\n");
            fprintf(meshFile,"\nEnd");
            closeTheFile(&meshFile);
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        iMax=pMesh->nqua;
        fprintf(meshFile,"\nQuadrilaterals\n%d\n",iMax);
        for (i=0; i<iMax; i++)
        {
            pQuadrilateral=&pMesh->pqua[i];
            if (pQuadrilateral->p1<1 || pQuadrilateral->p1>pMesh->nver ||
                       pQuadrilateral->p2<1 || pQuadrilateral->p2>pMesh->nver ||
                       pQuadrilateral->p3<1 || pQuadrilateral->p3>pMesh->nver ||
                         pQuadrilateral->p4<1 || pQuadrilateral->p4>pMesh->nver)
            {
                PRINT_ERROR("In writingMeshFile: the point references ");
                fprintf(stderr,"(%d,%d,",pQuadrilateral->p1,pQuadrilateral->p2);
                fprintf(stderr,"%d,%d) ",pQuadrilateral->p3,pQuadrilateral->p4);
                fprintf(stderr,"associated with the vertices of the ");
                fprintf(stderr,"%d-th quadrilateral must be positive ",i+1);
                fprintf(stderr,"integers not (strictly) greater than the ");
                fprintf(stderr,"total number of vertices %d ",pMesh->nver);
                fprintf(stderr,"related to the mesh discretization stored ");
                fprintf(stderr,"in the structure pointed by pMesh.\n");
                fprintf(meshFile,"\nEnd");
                closeTheFile(&meshFile);
                free(fileLocation);
                fileLocation=NULL;
                return 0;
            }
            fprintf(meshFile,"%d %d ",pQuadrilateral->p1,pQuadrilateral->p2);
            fprintf(meshFile,"%d %d ",pQuadrilateral->p3,pQuadrilateral->p4);
            fprintf(meshFile,"%d \n",pQuadrilateral->label);
        }

        // Check number of hexahedra then write their descriptions
        iMax=(pParameters->n_x-1)*(pParameters->n_y-1)*(pParameters->n_z-1);
        if (pMesh->nhex!=iMax)
        {
            PRINT_ERROR("In writingMeshFile: expecting ");
            fprintf(stderr,"%d hexahedra instead of %d ",iMax,pMesh->nhex);
            fprintf(stderr,"saved in the structure pointed by pMesh.\n");
            fprintf(meshFile,"\nEnd");
            closeTheFile(&meshFile);
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        fprintf(meshFile,"\nHexahedra\n%d\n",iMax);
        for (i=0; i<iMax; i++)
        {
            pHexahedron=&pMesh->phex[i];
            if (pHexahedron->p1<1 || pHexahedron->p1>pMesh->nver ||
                             pHexahedron->p2<1 || pHexahedron->p2>pMesh->nver ||
                             pHexahedron->p3<1 || pHexahedron->p3>pMesh->nver ||
                             pHexahedron->p4<1 || pHexahedron->p4>pMesh->nver ||
                             pHexahedron->p5<1 || pHexahedron->p5>pMesh->nver ||
                             pHexahedron->p6<1 || pHexahedron->p6>pMesh->nver ||
                             pHexahedron->p7<1 || pHexahedron->p7>pMesh->nver ||
                               pHexahedron->p8<1 || pHexahedron->p8>pMesh->nver)
            {
                PRINT_ERROR("In writingMeshFile: the point references ");
                fprintf(stderr,"(%d,%d,",pHexahedron->p1,pHexahedron->p2);
                fprintf(stderr,"(%d,%d,",pHexahedron->p3,pHexahedron->p4);
                fprintf(stderr,"(%d,%d,",pHexahedron->p5,pHexahedron->p6);
                fprintf(stderr,"%d,%d) ",pHexahedron->p7,pHexahedron->p8);
                fprintf(stderr,"associated with the vertices of the ");
                fprintf(stderr,"%d-th hexahedron must be positive ",i+1);
                fprintf(stderr,"integers not (strictly) greater than the ");
                fprintf(stderr,"total number of vertices %d ",pMesh->nver);
                fprintf(stderr,"related to the mesh discretization stored ");
                fprintf(stderr,"in the structure pointed by pMesh.\n");
                fprintf(meshFile,"\nEnd");
                closeTheFile(&meshFile);
                free(fileLocation);
                fileLocation=NULL;
                return 0;
            }
            fprintf(meshFile,"%d %d ",pHexahedron->p1,pHexahedron->p2);
            fprintf(meshFile,"%d %d ",pHexahedron->p3,pHexahedron->p4);
            fprintf(meshFile,"%d %d ",pHexahedron->p5,pHexahedron->p6);
            fprintf(meshFile,"%d %d ",pHexahedron->p7,pHexahedron->p8);

            // Change the negative label into their previous value
            if (pHexahedron->label==-3)
            {
                fprintf(meshFile,"%d \n",2);
            }
            else if (pHexahedron->label==-2)
            {
                fprintf(meshFile,"%d \n",3);
            }
            else
            {
                fprintf(meshFile,"%d \n",pHexahedron->label);
            }
        }

        // if pParameters->trick_matrix is set to zero, then reset the labels of
        // boundary hexahedra that were not properly set to their original value
        if (pParameters->opt_mode==-1)
        {
            if (!pParameters->trick_matrix)
            {
                if (pMesh->nadj>0 && pMesh->padj!=NULL)
                {
                    iMax=pMesh->nadj;
                    for (i=0; i<iMax; i++)
                    {
                        if (pMesh->phex[pMesh->padj[i].hexout-1].label==-3)
                        {
                            pMesh->phex[pMesh->padj[i].hexout-1].label=3;
                        }
                        if (pMesh->phex[pMesh->padj[i].hexin-1].label==-2)
                        {
                             pMesh->phex[pMesh->padj[i].hexin-1].label=2;
                        }
                    }
                }
            }
        }
    }
    else
    {
        // Check number of triangles then write their descriptions
        iMax=pMesh->ntri;
        if (iMax<1)
        {
            PRINT_ERROR("In writingMeshFile: expecting a positive number of ");
            fprintf(stderr,"triangles instead of %d stored in the ",iMax);
            fprintf(stderr,"structure pointed by pMesh.\n");
            fprintf(meshFile,"\nEnd");
            closeTheFile(&meshFile);
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        fprintf(meshFile,"\nTriangles\n%d\n",iMax);
        for (i=0; i<iMax; i++)
        {
            pTriangle=&pMesh->ptri[i];
            if (pTriangle->p1<1 || pTriangle->p1>pMesh->nver ||
                                 pTriangle->p2<1 || pTriangle->p2>pMesh->nver ||
                                   pTriangle->p3<1 || pTriangle->p3>pMesh->nver)
            {
                PRINT_ERROR("In writingMeshFile: the point references ");
                fprintf(stderr,"(%d,%d,",pTriangle->p1,pTriangle->p2);
                fprintf(stderr,"%d) ",pTriangle->p3);
                fprintf(stderr,"associated with the vertices of the ");
                fprintf(stderr,"%d-th triangle must be positive ",i+1);
                fprintf(stderr,"integers not (strictly) greater than the ");
                fprintf(stderr,"total number of vertices %d ",pMesh->nver);
                fprintf(stderr,"related to the mesh discretization stored ");
                fprintf(stderr,"in the structure pointed by pMesh.\n");
                fprintf(meshFile,"\nEnd");
                closeTheFile(&meshFile);
                free(fileLocation);
                fileLocation=NULL;
                return 0;
            }
            fprintf(meshFile,"%d %d ",pTriangle->p1,pTriangle->p2);
            fprintf(meshFile,"%d %d \n",pTriangle->p3,pTriangle->label);
        }

        // Check number of edges/ridges then write their descriptions
        iMax=pMesh->nedg;
        if (iMax<0)
        {
            PRINT_ERROR("In writingMeshFile: expecting a non-negative number ");
            fprintf(stderr,"of edges instead of %d stored in the ",iMax);
            fprintf(stderr,"structure pointed by pMesh.\n");
            fprintf(meshFile,"\nEnd");
            closeTheFile(&meshFile);
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        if (iMax>1)
        {
            fprintf(meshFile,"\nEdges\n%d\n",iMax);
            for (i=0; i<iMax; i++)
            {
                pEdge=&pMesh->pedg[i];
                if (pEdge->p1<1 || pEdge->p1>pMesh->nver || pEdge->p2<1 ||
                                                          pEdge->p2>pMesh->nver)
                {
                    PRINT_ERROR("In writingMeshFile: the point references ");
                    fprintf(stderr,"(%d,%d,) associated ",pEdge->p1,pEdge->p2);
                    fprintf(stderr,"with the vertices of the %d-th edge ",i+1);
                    fprintf(stderr,"must be positive integers not (strictly) ");
                    fprintf(stderr,"greater than the total number of ");
                    fprintf(stderr,"vertices %d related to the ",pMesh->nver);
                    fprintf(stderr,"mesh discretization stored in the ");
                    fprintf(stderr,"structure pointed by pMesh.\n");
                    fprintf(meshFile,"\nEnd");
                    closeTheFile(&meshFile);
                    free(fileLocation);
                    fileLocation=NULL;
                    return 0;
                }
                fprintf(meshFile,"%d %d ",pEdge->p1,pEdge->p2);
                fprintf(meshFile,"%d \n",pEdge->label);
            }

            fprintf(meshFile,"\nRidges\n%d\n",iMax);
            for (i=0; i<iMax; i++)
            {
                fprintf(meshFile,"%d \n",i+1);
            }
        }

        // Check number of corners/requiredVertices and write their descriptions
        iMax=pMesh->ncor;
        if (iMax<0)
        {
            PRINT_ERROR("In writingMeshFile: expecting a non-negative number ");
            fprintf(stderr,"of corners instead of %d stored in the ",iMax);
            fprintf(stderr,"structure pointed by pMesh.\n");
            fprintf(meshFile,"\nEnd");
            closeTheFile(&meshFile);
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }
        else if (iMax!=8)
        {
            if (pParameters->verbose)
            {
                fprintf(stdout,"\nWarning in writingMeshFile function: the ");
                fprintf(stdout,"number of corners should be 8 (corresponding ");
                fprintf(stderr,"to the corners of the computational box) ");
                fprintf(stdout,"instead of %d saved in the structure ",iMax);
                fprintf(stdout,"pointed by pMesh.\n");
            }
        }

        iMax=0;
        for (i=0; i<iMax; i++)
        {
            pVertex=&pMesh->pver[i];
            if (pVertex->label>100)
            {
                iMax++;
            }
        }

        if (iMax==8)
        {
            fprintf(meshFile,"\nCorners\n%d\n",8);
            iMax=pMesh->nver;
            for (i=0; i<iMax; i++)
            {
                pVertex=&pMesh->pver[i];
                if (pVertex->label>100)
                {
                    fprintf(meshFile,"%d \n",i+1);
                }
            }
            fprintf(meshFile,"\nRequiredVertices\n%d\n",8);
            for (i=0; i<iMax; i++)
            {
                pVertex=&pMesh->pver[i];
                if (pVertex->label>100)
                {
                    fprintf(meshFile,"%d \n",i+1);
                }
            }
        }

        // Check number of tetrahedra then write their descriptions
        iMax=pMesh->ntet;
        if (iMax<1)
        {
            PRINT_ERROR("In writingMeshFile: expecting a positive number of ");
            fprintf(stderr,"tetrahedra instead of %d stored in the ",iMax);
            fprintf(stderr,"structure pointed by pMesh.\n");
            fprintf(meshFile,"\nEnd");
            closeTheFile(&meshFile);
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        fprintf(meshFile,"\nTetrahedra\n%d\n",iMax);
        for (i=0; i<iMax; i++)
        {
            pTetrahedron=&pMesh->ptet[i];
            if (pTetrahedron->p1<1 || pTetrahedron->p1>pMesh->nver ||
                           pTetrahedron->p2<1 || pTetrahedron->p2>pMesh->nver ||
                           pTetrahedron->p3<1 || pTetrahedron->p3>pMesh->nver ||
                             pTetrahedron->p4<1 || pTetrahedron->p4>pMesh->nver)
            {
                PRINT_ERROR("In writingMeshFile: the point references ");
                fprintf(stderr,"(%d,%d,",pTetrahedron->p1,pTetrahedron->p2);
                fprintf(stderr,"%d,%d) ",pTetrahedron->p3,pTetrahedron->p4);
                fprintf(stderr,"associated with the vertices of the ");
                fprintf(stderr,"%d-th tetrahedron must be positive ",i+1);
                fprintf(stderr,"integers not (strictly) greater than the ");
                fprintf(stderr,"total number of vertices %d ",pMesh->nver);
                fprintf(stderr,"related to the mesh discretization stored ");
                fprintf(stderr,"in the structure pointed by pMesh.\n");
                fprintf(meshFile,"\nEnd");
                closeTheFile(&meshFile);
                free(fileLocation);
                fileLocation=NULL;
                return 0;
            }
            fprintf(meshFile,"%d %d ",pTetrahedron->p1,pTetrahedron->p2);
            fprintf(meshFile,"%d %d ",pTetrahedron->p3,pTetrahedron->p4);
            fprintf(meshFile,"%d \n",pTetrahedron->label);
        }

        // Check number of normal vectors then write their descriptions
        iMax=pMesh->nnorm;
        if (iMax<0)
        {
            PRINT_ERROR("In writingMeshFile: expecting a non-negative number ");
            fprintf(stderr,"of normal vectors instead of %d stored in ",iMax);
            fprintf(stderr,"the structure pointed by pMesh.\n");
            fprintf(meshFile,"\nEnd");
            closeTheFile(&meshFile);
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        if (iMax>1)
        {
            fprintf(meshFile,"\nNormals\n%d\n",iMax);
            for (i=0; i<iMax; i++)
            {
                pNormal=&pMesh->pnorm[i];
                fprintf(meshFile,"%.8le %.8le ",pNormal->x,pNormal->y);
                fprintf(meshFile,"%.8le \n",pNormal->z);
            }
            fprintf(meshFile,"\nNormalAtVertices\n%d\n",iMax);
            for (i=0; i<iMax; i++)
            {
                pNormal=&pMesh->pnorm[i];
                if (pNormal->p<1 || pNormal->p>pMesh->nver)
                {
                    PRINT_ERROR("In writingMeshFile: the point reference ");
                    fprintf(stderr,"(=%d) associated with the ",pNormal->p);
                    fprintf(stderr,"vertex where the %d-th normal vector ",i+1);
                    fprintf(stderr,"originated from must be positive ");
                    fprintf(stderr,"integers not (strictly) greater than the ");
                    fprintf(stderr,"total number of vertices %d ",pMesh->nver);
                    fprintf(stderr,"related to the mesh discretization ");
                    fprintf(stderr,"stored in the structure pointed by ");
                    fprintf(stderr,"pMesh.\n");
                    fprintf(meshFile,"\nEnd");
                    closeTheFile(&meshFile);
                    free(fileLocation);
                    fileLocation=NULL;
                    return 0;
                }
                fprintf(meshFile,"%d %d \n",pNormal->p,i+1);
            }
        }

        // Check number of tnagent vectors then write their descriptions
        iMax=pMesh->ntan;
        if (iMax<0)
        {
            PRINT_ERROR("In writingMeshFile: expecting a non-negative number ");
            fprintf(stderr,"of tangent vectors instead of %d stored in ",iMax);
            fprintf(stderr,"the structure pointed by pMesh.\n");
            fprintf(meshFile,"\nEnd");
            closeTheFile(&meshFile);
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        if (iMax>1)
        {
            fprintf(meshFile,"\nTangents\n%d\n",iMax);
            for (i=0; i<iMax; i++)
            {
                pTangent=&pMesh->ptan[i];
                if (pTangent->p<1 || pTangent->p>pMesh->nver)
                {
                    PRINT_ERROR("In writingMeshFile: the point reference ");
                    fprintf(stderr,"(=%d) associated with the ",pTangent->p);
                    fprintf(stderr,"vertex where the %d-th tangent ",i+1);
                    fprintf(stderr,"vector originated from must be positive ");
                    fprintf(stderr,"integers not (strictly) greater than the ");
                    fprintf(stderr,"total number of vertices %d ",pMesh->nver);
                    fprintf(stderr,"related to the mesh discretization ");
                    fprintf(stderr,"stored in the structure pointed by ");
                    fprintf(stderr,"pMesh.\n");
                    fprintf(meshFile,"\nEnd");
                    closeTheFile(&meshFile);
                    free(fileLocation);
                    fileLocation=NULL;
                    return 0;
                }
                fprintf(meshFile,"%.8le %.8le ",pTangent->x,pTangent->y);
                fprintf(meshFile,"%.8le \n",pTangent->z);
            }
            fprintf(meshFile,"\nTangentAtVertices\n%d\n",iMax);
            for (i=0; i<iMax; i++)
            {
                pTangent=&pMesh->ptan[i];
                fprintf(meshFile,"%d %d \n",pTangent->p,i+1);
            }
        }
    }
    fprintf(meshFile,"\nEnd");

    // Closing the *.mesh file: fclose function returns zero if the input FILE*
    // variable is successfully closed, otherwise EOF (end-of-file) is returned
    if (fclose(meshFile))
    {
        PRINT_ERROR("In writingMeshFile: the ");
        fprintf(stderr,"%s file has not been closed properly.\n",fileLocation);
        meshFile=NULL;
        free(fileLocation);
        fileLocation=NULL;
        return 0;
    }
    meshFile=NULL;
    fprintf(stdout,"Closing file.\n");

    // Free the memory dynamically allocated for fileLocation
    free(fileLocation);
    fileLocation=NULL;

    return 1;
}

/* ************************************************************************** */
// The function getMeshFormat determines if the file located at fileLocation
// exists and if its name ends by the *.mesh extension or the *.cube one, with a
// length (strictly) lower than the nameLength variable, and more than five (to
// be able to store at least something more than the *.mesh or *.cube
// extension). It has the char* fileLocation and int nameLength variables as
// input arguments, and it return zero if an error occurs, otherwise 1 (resp.
// -1) is returned for the *.mesh (resp. *.cube) format.
/* ************************************************************************** */
int getMeshFormat(char* fileLocation, int nameLength)
{
    size_t length=0;
    int returnValue=0;

    // Check that nameLength is (strictly) greater than six
    if (nameLength<7)
    {
        PRINT_ERROR("In getMeshFormat: the input nameLength ");
        fprintf(stderr,"(=%d) variable should be an integer ",nameLength);
        fprintf(stderr,"(strictly) greater than six.\n");
        return 0;
    }

    // Check if the file exists at fileLocation
    if (initialFileExists(fileLocation,nameLength)!=1)
    {
        PRINT_ERROR("In getMeshFormat: the initialFileExists function ");
        fprintf(stderr,"did not returned one, which was the expected value ");
        fprintf(stderr,"here.\n");
        return 0;
    }

    // Check that its size can at least contain the *.mesh or *.cube extension
    // strlen returns the length of the string, but not including the char '\0'
    length=strlen(fileLocation);
    if (length<6)
    {
        PRINT_ERROR("In getMeshFormat: the input fileLocation variable ");
        fprintf(stderr,"(=%s) should have a length that at ",fileLocation);
        fprintf(stderr,"least equals six in order to end with something more ");
        fprintf(stderr,"than the '.mesh' or '.cube' extension.\n");
        return 0;
    }

    // Distinguish the *.mesh from *.cube format
    if (fileLocation[length-5]=='.' && fileLocation[length-4]=='m' &&
                   fileLocation[length-3]=='e' && fileLocation[length-2]=='s' &&
                      fileLocation[length-1]=='h' && fileLocation[length]=='\0')
    {
        returnValue=1;
    }
    else if (fileLocation[length-5]=='.' && fileLocation[length-4]=='c' &&
                   fileLocation[length-3]=='u' && fileLocation[length-2]=='b' &&
                      fileLocation[length-1]=='e' && fileLocation[length]=='\0')
    {
        returnValue=-1;
    }
    else
    {
        PRINT_ERROR("In getMeshFormat: the input fileLocation variable ");
        fprintf(stderr,"(=%s) does not end with the '.mesh' ",fileLocation);
        fprintf(stderr,"or '.cube' extension.\n");
        returnValue=0;
    }

    return returnValue;
}

/* ************************************************************************** */
// The function readMeshFileAndAllocateMesh reads the *.mesh file at the
// location given by the name_mesh variable of the structure pointed by
// pParameters (warning here: the file must exist), checks that it has the
// *.mesh format syntax, and dynamically allocate some memory for the structure
// pointed by pMesh, storing the mesh data into the structure pointed by pMesh,
// updating also some values stored in the structure pointed by pParmaters. It
// has the Parameter* and Mesh* variables (both defined in main.h) as input
// arguments and it returns zero if an error occurs, otherwise one on success
/* ************************************************************************** */
int readMeshFileAndAllocateMesh(Parameters* pParameters, Mesh* pMesh)
{
    size_t length=0;
    char readStringIn[21]={'\0'}, *readStringOut=NULL, keyword[4]={'\0'};
    int readIntegerIn=0, readIntegerOut=0, i=0, j=0, k=0;
    double readDouble=0.;
    FILE *meshFile=NULL;

    // Check if the input variables are not pointing to NULL
    if (pParameters==NULL || pMesh==NULL)
    {
        PRINT_ERROR("In readMeshFileAndAllocateMesh: one of the input ");
        fprintf(stderr,"variables (pParameters=%p, ",(void*)pParameters);
        fprintf(stderr,"pMesh=%p) does not have a valid ",(void*)pMesh);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Set the structure to zero for variables and NULL for pointers
    initializeMeshStructure(pMesh);

    // Check if the file must has been previously created
    if (getMeshFormat(pParameters->name_mesh,pParameters->name_length)!=1)
    {
        PRINT_ERROR("In readMeshFileAndAllocateMesh: getMeshFormat function ");
        fprintf(stderr,"did not return one, which was the expected value ");
        fprintf(stderr,"here.\n");
        return 0;
    }

    // Opening *.mesh file (warning: reset and overwrite file if already exists)
    // fopen returns a FILE pointer on success, otherwise NULL is returned
    if (pParameters->opt_mode!=1 || pParameters->verbose)
    {
        fprintf(stdout,"\nOpening %s file. ",pParameters->name_mesh);
    }
    meshFile=fopen(pParameters->name_mesh,"r");
    if (meshFile==NULL)
    {
        PRINT_ERROR("In readMeshFileAndAllocateMesh: we were not able to ");
        fprintf(stderr,"read the %s file.\n",pParameters->name_mesh);
        return 0;
    }
    if (pParameters->opt_mode!=1 || pParameters->verbose)
    {
        fprintf(stdout,"Reading mesh data. ");
    }

    // Read the precision (expecting MeshVersionFormatted 2): in general
    // 1=single, 2=double, MeshVersionUnformatted for binary (.meshb) file
    // (not considered here). We recall that fgets returns a null pointer if it
    // failed, otherwise it returns the same readStringIn parameters while
    // fscanf function returns the number of input items successfully matched
    // and assigned, which can be fewer than provided for. Important remark:
    //  white space characters, such as blanks and newline characters, causes
    // fscanf (scanf and sscanf) to read, but not to store, all consecutive
    // white space characters in the input up to the next character that is not
    // white space. One white space character in format-string matches any
    // combination of white space characters in the input.
    readStringOut=fgets(readStringIn,21,meshFile);
    readIntegerOut=fscanf(meshFile," %d ",&readIntegerIn);

    // strcmp returns 0 if the two strings are equal, otherwise <0 (resp. >0)
    // if the 1st string argument is shorter (resp. longer) than the 2nd one
    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In readMeshFileAndAllocateMesh: wrong return of the ");
        fprintf(stderr,"standard fgets %p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) c-function while attempting to ",readIntegerOut);
        fprintf(stderr,"read the precision of the value in the mesh file.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    if (strcmp(readStringIn,"MeshVersionFormatted") || readIntegerIn!=2)
    {
        PRINT_ERROR("In readMeshFileAndAllocateMesh: expecting ");
        fprintf(stderr,"MeshVersionFormatted 2 instead of %s ",readStringIn);
        fprintf(stderr,"%d.\n",readIntegerIn);
        closeTheFile(&meshFile);
        return 0;
    }

    // Readthe space dimension (expecting Dimension 3)
    readStringOut=fgets(readStringIn,10,meshFile);
    readIntegerOut=fscanf(meshFile," %d ",&readIntegerIn);

    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In readMeshFileAndAllocateMesh: wrong return of the ");
        fprintf(stderr,"standard fgets %p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) c-function while attempting to ",readIntegerOut);
        fprintf(stderr,"read the dimension of the mesh discretization.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    if (strcmp(readStringIn,"Dimension") || readIntegerIn!=3)
    {
        PRINT_ERROR("In readMeshFileAndAllocateMesh: expecting Dimension 3 ");
        fprintf(stderr,"instead of %s %d.\n",readStringIn,readIntegerIn);
        closeTheFile(&meshFile);
        return 0;
    }

    // Read the number of Vertices and store it in pMesh->nver (we impose here
    // that the keyword Vertices appears first (i.e. just after the dimension in
    // the *.mesh file so as to be able to check then the validity of integers
    // referring to the vertex positions of the element vertices in the mesh
    readStringOut=fgets(readStringIn,9,meshFile);
    readIntegerOut=fscanf(meshFile," %d ",&readIntegerIn);

    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In readMeshFileAndAllocateMesh: wrong return of the ");
        fprintf(stderr,"standard fgets %p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) c-function while attempting to ",readIntegerOut);
        fprintf(stderr,"read the total number of vertices in the mesh.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    if (strcmp(readStringIn,"Vertices") || readIntegerIn<1)
    {
        PRINT_ERROR("In readMeshFileAndAllocateMesh: expecting Vertices Nver ");
        fprintf(stderr,"instead of %s %d ",readStringIn,readIntegerIn);
        fprintf(stderr,"(a positive integer value).\n");
        closeTheFile(&meshFile);
        return 0;
    }
    pMesh->nver=readIntegerIn;

    // Dynamically allocate memory for vertices and check if it worked
    // calloc function returns a pointer to the allocated memory, otherwise NULL
    length=readIntegerIn;
    pMesh->pver=(Point*)calloc(length,sizeof(Point));
    if (pMesh->pver==NULL)
    {
        PRINT_ERROR("In readMeshFileAndAllocateMesh: could not allocate ");
        fprintf(stderr,"memory for %d vertices in the ",readIntegerIn);
        fprintf(stderr,"structure pointed by pMesh.\n");
        closeTheFile(&meshFile);
        return 0;
    }

    // Saving Vertices in pMesh->pver and check if it works
    for (k=0; k<readIntegerIn; k++)
    {
        j=0;
        j+=fscanf(meshFile,"%lf ",&readDouble);
        pMesh->pver[k].x=readDouble;

        j+=fscanf(meshFile,"%lf ",&readDouble);
        pMesh->pver[k].y=readDouble;

        j+=fscanf(meshFile,"%lf ",&readDouble);
        pMesh->pver[k].z=readDouble;

        j+=fscanf(meshFile,"%d ",&readIntegerOut);
        pMesh->pver[k].label=readIntegerOut;

        pMesh->pver[k].value=0.;
        if (j!=4)
        {
            PRINT_ERROR("In readMeshFileAndAllocateMesh: we were not able ");
            fprintf(stderr,"to properly save the Point data associated with ");
            fprintf(stderr,"with the %d-th vertex of the mesh (only %d ",k+1,j);
            fprintf(stderr,"values properly saved instead of four).\n");
            closeTheFile(&meshFile);
            return 0;
        }
    }

    // Distinguish the hexahedral meshes from the tetrahedral ones
    if (pParameters->opt_mode<=0)
    {
        // Read 3 letters and study the different cases until the End keyword
        i=0;
        do {
            readStringOut=fgets(keyword,4,meshFile);
            if (readStringOut==NULL)
            {
                PRINT_ERROR("In readMeshFileAndAllocateMesh: wrong return ");
                fprintf(stderr,"%p of the standard ",(void*)readStringOut);
                fprintf(stderr,"fgets c-function while attempting to read ");
                fprintf(stderr,"the first three letters of a *.mesh-format ");
                fprintf(stderr,"keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }
            i++;

            if (!strcmp(keyword,"Qua"))
            {
                // Read the number of quadrilaterals and store it in pMesh->nqua
                readStringOut=fgets(readStringIn,12,meshFile);
                readIntegerOut=fscanf(meshFile," %d ",&readIntegerIn);

                if (readStringOut==NULL || readIntegerOut!=1)
                {
                    PRINT_ERROR("In readMeshFileAndAllocateMesh: wrong ");
                    fprintf(stderr,"return of the standard fgets ");
                    fprintf(stderr,"%p or fscanf ",(void*)readStringOut);
                    fprintf(stderr,"(=%d) c-function while ",readIntegerOut);
                    fprintf(stderr,"attempting to read the total number of ");
                    fprintf(stderr,"(boundary) quadrilaterals in the mesh.\n");
                    closeTheFile(&meshFile);
                    return 0;
                }

                if (strcmp(readStringIn,"drilaterals") || readIntegerIn<1)
                {
                    PRINT_ERROR("In readMeshFileAndAllocateMesh: expecting ");
                    fprintf(stderr,"Quadrilaterals Nqua instead of ");
                    fprintf(stderr,"Qua%s %d ",readStringIn,readIntegerIn);
                    fprintf(stderr,"(a positive integer value).\n");
                    closeTheFile(&meshFile);
                    return 0;
                }
                pMesh->nqua=readIntegerIn;

                // Allocating memory for quadrilaterals and check if it worked
                length=readIntegerIn;
                pMesh->pqua=(Quadrilateral*)calloc(length,
                                                         sizeof(Quadrilateral));
                if (pMesh->pqua==NULL)
                {
                    PRINT_ERROR("In readMeshFileAndAllocateMesh: could not ");
                    fprintf(stderr,"allocate memory for %d ",readIntegerIn);
                    fprintf(stderr,"quadrilaterals in the structure pointed ");
                    fprintf(stderr,"by pMesh.\n");
                    closeTheFile(&meshFile);
                    return 0;
                }

                // Save quadrilaterals in pMesh->pqua and check if it worked
                for (k=0; k<readIntegerIn; k++)
                {
                    j=0;
                    j+=fscanf(meshFile,"%d ",&readIntegerOut);
                    if (readIntegerOut<1 || readIntegerOut>pMesh->nver)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: the ");
                        fprintf(stderr,"first vertex of the %d-th ",k+1);
                        fprintf(stderr,"quadrilateral is referred to as ");
                        fprintf(stderr,"%d, which should be ",readIntegerOut);
                        fprintf(stderr,"a positive integer not (strictly) ");
                        fprintf(stderr,"greater than the total number of ");
                        fprintf(stderr,"points %d in the mesh.\n",pMesh->nver);
                        closeTheFile(&meshFile);
                        return 0;
                    }
                    pMesh->pqua[k].p1=readIntegerOut;

                    j+=fscanf(meshFile,"%d ",&readIntegerOut);
                    if (readIntegerOut<1 || readIntegerOut>pMesh->nver)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: the ");
                        fprintf(stderr,"second vertex of the %d-th ",k+1);
                        fprintf(stderr,"quadrilateral is referred to as ");
                        fprintf(stderr,"%d, which should be ",readIntegerOut);
                        fprintf(stderr,"a positive integer not (strictly) ");
                        fprintf(stderr,"greater than the total number of ");
                        fprintf(stderr,"points %d in the mesh.\n",pMesh->nver);
                        closeTheFile(&meshFile);
                        return 0;
                    }
                    pMesh->pqua[k].p2=readIntegerOut;

                    j+=fscanf(meshFile,"%d ",&readIntegerOut);
                    if (readIntegerOut<1 || readIntegerOut>pMesh->nver)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: the ");
                        fprintf(stderr,"third vertex of the %d-th ",k+1);
                        fprintf(stderr,"quadrilateral is referred to as ");
                        fprintf(stderr,"%d, which should be ",readIntegerOut);
                        fprintf(stderr,"a positive integer not (strictly) ");
                        fprintf(stderr,"greater than the total number of ");
                        fprintf(stderr,"points %d in the mesh.\n",pMesh->nver);
                        closeTheFile(&meshFile);
                        return 0;
                    }
                    pMesh->pqua[k].p3=readIntegerOut;

                    j+=fscanf(meshFile,"%d ",&readIntegerOut);
                    if (readIntegerOut<1 || readIntegerOut>pMesh->nver)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: the ");
                        fprintf(stderr,"fourth vertex of the %d-th ",k+1);
                        fprintf(stderr,"quadrilateral is referred to as ");
                        fprintf(stderr,"%d, which should be ",readIntegerOut);
                        fprintf(stderr,"a positive integer not (strictly) ");
                        fprintf(stderr,"greater than the total number of ");
                        fprintf(stderr,"points %d in the mesh.\n",pMesh->nver);
                        closeTheFile(&meshFile);
                        return 0;
                    }
                    pMesh->pqua[k].p4=readIntegerOut;

                    j+=fscanf(meshFile,"%d ",&readIntegerOut);
                    pMesh->pqua[k].label=readIntegerOut;
                    if (j!=5)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: we were ");
                        fprintf(stderr,"not able to properly save the ");
                        fprintf(stderr,"Quadrilateral data associated with ");
                        fprintf(stderr,"with the %d-th vertex of the ",k+1);
                        fprintf(stderr,"mesh (only %d values properly ",j);
                        fprintf(stderr,"saved instead of five).\n");
                        closeTheFile(&meshFile);
                        return 0;
                    }
                }
            }
            else if (!strcmp(keyword,"Hex"))
            {
                // Read the number of Hexahedra and store it in pMesh->nhex
                readStringOut=fgets(readStringIn,7,meshFile);
                readIntegerOut=fscanf(meshFile," %d ",&readIntegerIn);

                if (readStringOut==NULL || readIntegerOut!=1)
                {
                    PRINT_ERROR("In readMeshFileAndAllocateMesh: wrong ");
                    fprintf(stderr,"return of the standard fgets ");
                    fprintf(stderr,"%p or fscanf ",(void*)readStringOut);
                    fprintf(stderr,"(=%d) c-function while ",readIntegerOut);
                    fprintf(stderr,"attempting to read the total number of ");
                    fprintf(stderr,"hexahedra in the mesh.\n");
                    closeTheFile(&meshFile);
                    return 0;
                }

                if (strcmp(readStringIn,"ahedra") || readIntegerIn<1)
                {
                    PRINT_ERROR("In readMeshFileAndAllocateMesh: expecting ");
                    fprintf(stderr,"Hexahedra Nhex instead of ");
                    fprintf(stderr,"Hex%s %d ",readStringIn,readIntegerIn);
                    fprintf(stderr,"(a positive integer value).\n");
                    closeTheFile(&meshFile);
                    return 0;
                }
                pMesh->nhex=readIntegerIn;

                // Allocating memory for hexahedra and check if it works
                length=readIntegerIn;
                pMesh->phex=(Hexahedron*)calloc(length,sizeof(Hexahedron));
                if (pMesh->phex==NULL)
                {
                    PRINT_ERROR("In readMeshFileAndAllocateMesh: could not ");
                    fprintf(stderr,"allocate memory for %d ",readIntegerIn);
                    fprintf(stderr,"hexahedra in the structure pointed ");
                    fprintf(stderr,"by pMesh.\n");
                    closeTheFile(&meshFile);
                    return 0;
                }

                // Save hexahedra in pMesh->phex and check if it worked
                for (k=0; k<readIntegerIn; k++)
                {
                    j=0;
                    j+=fscanf(meshFile,"%d ",&readIntegerOut);
                    if (readIntegerOut<1 || readIntegerOut>pMesh->nver)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: the ");
                        fprintf(stderr,"first vertex of the %d-th ",k+1);
                        fprintf(stderr,"hexahedron is referred to as ");
                        fprintf(stderr,"%d, which should be ",readIntegerOut);
                        fprintf(stderr,"a positive integer not (strictly) ");
                        fprintf(stderr,"greater than the total number of ");
                        fprintf(stderr,"points %d in the mesh.\n",pMesh->nver);
                        closeTheFile(&meshFile);
                        return 0;
                    }
                    pMesh->phex[k].p1=readIntegerOut;

                    j+=fscanf(meshFile,"%d ",&readIntegerOut);
                    if (readIntegerOut<1 || readIntegerOut>pMesh->nver)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: the ");
                        fprintf(stderr,"second vertex of the %d-th ",k+1);
                        fprintf(stderr,"hexahedron is referred to as ");
                        fprintf(stderr,"%d, which should be ",readIntegerOut);
                        fprintf(stderr,"a positive integer not (strictly) ");
                        fprintf(stderr,"greater than the total number of ");
                        fprintf(stderr,"points %d in the mesh.\n",pMesh->nver);
                        closeTheFile(&meshFile);
                        return 0;
                    }
                    pMesh->phex[k].p2=readIntegerOut;

                    j+=fscanf(meshFile,"%d ",&readIntegerOut);
                    if (readIntegerOut<1 || readIntegerOut>pMesh->nver)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: the ");
                        fprintf(stderr,"third vertex of the %d-th ",k+1);
                        fprintf(stderr,"hexahedron is referred to as ");
                        fprintf(stderr,"%d, which should be ",readIntegerOut);
                        fprintf(stderr,"a positive integer not (strictly) ");
                        fprintf(stderr,"greater than the total number of ");
                        fprintf(stderr,"points %d in the mesh.\n",pMesh->nver);
                        closeTheFile(&meshFile);
                        return 0;
                    }
                    pMesh->phex[k].p3=readIntegerOut;

                    j+=fscanf(meshFile,"%d ",&readIntegerOut);
                    if (readIntegerOut<1 || readIntegerOut>pMesh->nver)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: the ");
                        fprintf(stderr,"fourth vertex of the %d-th ",k+1);
                        fprintf(stderr,"hexahedron is referred to as ");
                        fprintf(stderr,"%d, which should be ",readIntegerOut);
                        fprintf(stderr,"a positive integer not (strictly) ");
                        fprintf(stderr,"greater than the total number of ");
                        fprintf(stderr,"points %d in the mesh.\n",pMesh->nver);
                        closeTheFile(&meshFile);
                        return 0;
                    }
                    pMesh->phex[k].p4=readIntegerOut;

                    j+=fscanf(meshFile,"%d ",&readIntegerOut);
                    if (readIntegerOut<1 || readIntegerOut>pMesh->nver)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: the ");
                        fprintf(stderr,"fifth vertex of the %d-th ",k+1);
                        fprintf(stderr,"hexahedron is referred to as ");
                        fprintf(stderr,"%d, which should be ",readIntegerOut);
                        fprintf(stderr,"a positive integer not (strictly) ");
                        fprintf(stderr,"greater than the total number of ");
                        fprintf(stderr,"points %d in the mesh.\n",pMesh->nver);
                        closeTheFile(&meshFile);
                        return 0;
                    }
                    pMesh->phex[k].p5=readIntegerOut;

                    j+=fscanf(meshFile,"%d ",&readIntegerOut);
                    if (readIntegerOut<1 || readIntegerOut>pMesh->nver)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: the ");
                        fprintf(stderr,"sixth vertex of the %d-th ",k+1);
                        fprintf(stderr,"hexahedron is referred to as ");
                        fprintf(stderr,"%d, which should be ",readIntegerOut);
                        fprintf(stderr,"a positive integer not (strictly) ");
                        fprintf(stderr,"greater than the total number of ");
                        fprintf(stderr,"points %d in the mesh.\n",pMesh->nver);
                        closeTheFile(&meshFile);
                        return 0;
                    }
                    pMesh->phex[k].p6=readIntegerOut;

                    j+=fscanf(meshFile,"%d ",&readIntegerOut);
                    if (readIntegerOut<1 || readIntegerOut>pMesh->nver)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: the ");
                        fprintf(stderr,"seventh vertex of the %d-th ",k+1);
                        fprintf(stderr,"hexahedron is referred to as ");
                        fprintf(stderr,"%d, which should be ",readIntegerOut);
                        fprintf(stderr,"a positive integer not (strictly) ");
                        fprintf(stderr,"greater than the total number of ");
                        fprintf(stderr,"points %d in the mesh.\n",pMesh->nver);
                        closeTheFile(&meshFile);
                        return 0;
                    }
                    pMesh->phex[k].p7=readIntegerOut;

                    j+=fscanf(meshFile,"%d ",&readIntegerOut);
                    if (readIntegerOut<1 || readIntegerOut>pMesh->nver)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: the ");
                        fprintf(stderr,"eighth vertex of the %d-th ",k+1);
                        fprintf(stderr,"hexahedron is referred to as ");
                        fprintf(stderr,"%d, which should be ",readIntegerOut);
                        fprintf(stderr,"a positive integer not (strictly) ");
                        fprintf(stderr,"greater than the total number of ");
                        fprintf(stderr,"points %d in the mesh.\n",pMesh->nver);
                        closeTheFile(&meshFile);
                        return 0;
                    }
                    pMesh->phex[k].p8=readIntegerOut;

                    j+=fscanf(meshFile,"%d ",&readIntegerOut);
                    pMesh->phex[k].label=readIntegerOut;
                    if (j!=9)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: we were ");
                        fprintf(stderr,"not able to properly save the ");
                        fprintf(stderr,"Hexahedron data associated with ");
                        fprintf(stderr,"with the %d-th vertex of the ",k+1);
                        fprintf(stderr,"mesh (only %d values properly ",j);
                        fprintf(stderr,"saved instead of nine).\n");
                        closeTheFile(&meshFile);
                        return 0;
                    }
                }
            }
            else if (!strcmp(keyword,"End"))
            {
                i=3;
            }
            else
            {
                PRINT_ERROR("In readMeshFileAndAllocateMesh: the ");
                fprintf(stderr,"%d-th string read (=%s) does not ",i,keyword);
                fprintf(stderr,"correspond to any known keyword in the ");
                fprintf(stderr,"hexahedral mesh structure pointed by pMesh.\n");
                closeTheFile(&meshFile);
                return 0;
            }
        } while (i<3);
    }
    else
    {
        // Read 3 letters and study the different cases until the End keyword
        i=0;
        do {
            readStringOut=fgets(keyword,4,meshFile);
            if (readStringOut==NULL)
            {
                PRINT_ERROR("In readMeshFileAndAllocateMesh: wrong return ");
                fprintf(stderr,"%p of the standard ",(void*)readStringOut);
                fprintf(stderr,"fgets c-function while attempting to read ");
                fprintf(stderr,"the first three letters of a *.mesh-format ");
                fprintf(stderr,"keyword.\n");
                closeTheFile(&meshFile);
                return 0;
            }
            i++;

            if (!strcmp(keyword,"Tri"))
            {
                // Read the number of Triangles and store it in pMesh->ntri
                readStringOut=fgets(readStringIn,7,meshFile);
                readIntegerOut=fscanf(meshFile," %d ",&readIntegerIn);

                if (readStringOut==NULL || readIntegerOut!=1)
                {
                    PRINT_ERROR("In readMeshFileAndAllocateMesh: wrong ");
                    fprintf(stderr,"return of the standard fgets ");
                    fprintf(stderr,"%p or fscanf ",(void*)readStringOut);
                    fprintf(stderr,"(=%d) c-function while ",readIntegerOut);
                    fprintf(stderr,"attempting to read the total number of ");
                    fprintf(stderr,"triangles in the mesh.\n");
                    closeTheFile(&meshFile);
                    return 0;
                }

                if (strcmp(readStringIn,"angles") || readIntegerIn<1)
                {
                    PRINT_ERROR("In readMeshFileAndAllocateMesh: expecting ");
                    fprintf(stderr,"Triangles Ntri instead of ");
                    fprintf(stderr,"Tri%s %d ",readStringIn,readIntegerIn);
                    fprintf(stderr,"(a positive integer value).\n");
                    closeTheFile(&meshFile);
                    return 0;
                }
                pMesh->ntri=readIntegerIn;

                // Allocating memory for triangles and check if it worked
                length=readIntegerIn;
                pMesh->ptri=(Triangle*)calloc(length,sizeof(Triangle));
                if (pMesh->ptri==NULL)
                {
                    PRINT_ERROR("In readMeshFileAndAllocateMesh: could not ");
                    fprintf(stderr,"allocate memory for %d ",readIntegerIn);
                    fprintf(stderr,"triangles in the structure pointed ");
                    fprintf(stderr,"by pMesh.\n");
                    closeTheFile(&meshFile);
                    return 0;
                }

                // Saving triangles in pMesh->ptri and check if it worked
                for (k=0; k<readIntegerIn; k++)
                {
                    j=0;
                    j+=fscanf(meshFile,"%d ",&readIntegerOut);
                    if (readIntegerOut<1 || readIntegerOut>pMesh->nver)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: the ");
                        fprintf(stderr,"first vertex of the %d-th ",k+1);
                        fprintf(stderr,"triangle is referred to as ");
                        fprintf(stderr,"%d, which should be ",readIntegerOut);
                        fprintf(stderr,"a positive integer not (strictly) ");
                        fprintf(stderr,"greater than the total number of ");
                        fprintf(stderr,"points %d in the mesh.\n",pMesh->nver);
                        closeTheFile(&meshFile);
                        return 0;
                    }
                    pMesh->ptri[k].p1=readIntegerOut;

                    j+=fscanf(meshFile,"%d ",&readIntegerOut);
                    if (readIntegerOut<1 || readIntegerOut>pMesh->nver)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: the ");
                        fprintf(stderr,"second vertex of the %d-th ",k+1);
                        fprintf(stderr,"triangle is referred to as ");
                        fprintf(stderr,"%d, which should be ",readIntegerOut);
                        fprintf(stderr,"a positive integer not (strictly) ");
                        fprintf(stderr,"greater than the total number of ");
                        fprintf(stderr,"points %d in the mesh.\n",pMesh->nver);
                        closeTheFile(&meshFile);
                        return 0;
                    }
                    pMesh->ptri[k].p2=readIntegerOut;

                    j+=fscanf(meshFile,"%d ",&readIntegerOut);
                    if (readIntegerOut<1 || readIntegerOut>pMesh->nver)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: the ");
                        fprintf(stderr,"third vertex of the %d-th ",k+1);
                        fprintf(stderr,"triangle is referred to as ");
                        fprintf(stderr,"%d, which should be ",readIntegerOut);
                        fprintf(stderr,"a positive integer not (strictly) ");
                        fprintf(stderr,"greater than the total number of ");
                        fprintf(stderr,"points %d in the mesh.\n",pMesh->nver);
                        closeTheFile(&meshFile);
                        return 0;
                    }
                    pMesh->ptri[k].p3=readIntegerOut;

                    j+=fscanf(meshFile,"%d ",&readIntegerOut);
                    pMesh->ptri[k].label=readIntegerOut;
                    if (j!=4)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: we were ");
                        fprintf(stderr,"not able to properly save the ");
                        fprintf(stderr,"Triangle data associated with ");
                        fprintf(stderr,"with the %d-th vertex of the ",k+1);
                        fprintf(stderr,"mesh (only %d values properly ",j);
                        fprintf(stderr,"saved instead of four).\n");
                        closeTheFile(&meshFile);
                        return 0;
                    }
                }
            }
            else if (!strcmp(keyword,"Edg"))
            {
                // Read the number of edges and store it in pMesh->nedg
                readStringOut=fgets(readStringIn,3,meshFile);
                readIntegerOut=fscanf(meshFile," %d ",&readIntegerIn);

                if (readStringOut==NULL || readIntegerOut!=1)
                {
                    PRINT_ERROR("In readMeshFileAndAllocateMesh: wrong ");
                    fprintf(stderr,"return of the standard fgets ");
                    fprintf(stderr,"%p or fscanf ",(void*)readStringOut);
                    fprintf(stderr,"(=%d) c-function while ",readIntegerOut);
                    fprintf(stderr,"attempting to read the total number of ");
                    fprintf(stderr,"edges in the mesh.\n");
                    closeTheFile(&meshFile);
                    return 0;
                }

                if (strcmp(readStringIn,"es") || readIntegerIn<1)
                {
                    PRINT_ERROR("In readMeshFileAndAllocateMesh: expecting ");
                    fprintf(stderr,"Edges Nedg instead of ");
                    fprintf(stderr,"Edg%s %d ",readStringIn,readIntegerIn);
                    fprintf(stderr,"(a positive integer value).\n");
                    closeTheFile(&meshFile);
                    return 0;
                }

                if (pMesh->nedg!=0)
                {
                    if (readIntegerIn!=pMesh->nedg)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: the ");
                        fprintf(stderr,"number of ridges (=%d) ",pMesh->nedg);
                        fprintf(stderr,"should not be different from the ");
                        fprintf(stderr,"number of edges (=%d) ",readIntegerIn);
                        fprintf(stderr,"in the mesh discretization.\n");
                        closeTheFile(&meshFile);
                        return 0;
                    }
                }
                else
                {
                    pMesh->nedg=readIntegerIn;

                    // Allocating memory for edges and check if it worked
                    length=readIntegerIn;
                    pMesh->pedg=(Edge*)calloc(length,sizeof(Edge));
                    if (pMesh->pedg==NULL)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: could ");
                        fprintf(stderr,"not allocate memory for ");
                        fprintf(stderr,"%d edges in the ",readIntegerIn);
                        fprintf(stderr,"structure pointed by pMesh.\n");
                        closeTheFile(&meshFile);
                        return 0;
                    }
                }

                // Save edges in pMesh->pedg and check if it worked
                for (k=0; k<readIntegerIn; k++)
                {
                    j=0;
                    j+=fscanf(meshFile,"%d ",&readIntegerOut);
                    if (readIntegerOut<1 || readIntegerOut>pMesh->nver)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: the ");
                        fprintf(stderr,"first vertex of the %d-th ",k+1);
                        fprintf(stderr,"edge is referred to as ");
                        fprintf(stderr,"%d, which should be ",readIntegerOut);
                        fprintf(stderr,"a positive integer not (strictly) ");
                        fprintf(stderr,"greater than the total number of ");
                        fprintf(stderr,"points %d in the mesh.\n",pMesh->nver);
                        closeTheFile(&meshFile);
                        return 0;
                    }
                    pMesh->pedg[k].p1=readIntegerOut;

                    j+=fscanf(meshFile,"%d ",&readIntegerOut);
                    if (readIntegerOut<1 || readIntegerOut>pMesh->nver)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: the ");
                        fprintf(stderr,"second vertex of the %d-th ",k+1);
                        fprintf(stderr,"edge is referred to as ");
                        fprintf(stderr,"%d, which should be ",readIntegerOut);
                        fprintf(stderr,"a positive integer not (strictly) ");
                        fprintf(stderr,"greater than the total number of ");
                        fprintf(stderr,"points %d in the mesh.\n",pMesh->nver);
                        closeTheFile(&meshFile);
                        return 0;
                    }
                    pMesh->pedg[k].p2=readIntegerOut;

                    j+=fscanf(meshFile,"%d ",&readIntegerOut);
                    pMesh->pedg[k].label=readIntegerOut;
                    if (j!=3)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: we were ");
                        fprintf(stderr,"not able to properly save the ");
                        fprintf(stderr,"Edge data associated with ");
                        fprintf(stderr,"with the %d-th vertex of the ",k+1);
                        fprintf(stderr,"mesh (only %d values properly ",j);
                        fprintf(stderr,"saved instead of three).\n");
                        closeTheFile(&meshFile);
                        return 0;
                    }
                }
            }
            else if (!strcmp(keyword,"Rid"))
            {
                // Read the number of Ridges and store it in pMesh->nedg
                readStringOut=fgets(readStringIn,4,meshFile);
                readIntegerOut=fscanf(meshFile," %d ",&readIntegerIn);

                if (readStringOut==NULL || readIntegerOut!=1)
                {
                    PRINT_ERROR("In readMeshFileAndAllocateMesh: wrong ");
                    fprintf(stderr,"return of the standard fgets ");
                    fprintf(stderr,"%p or fscanf ",(void*)readStringOut);
                    fprintf(stderr,"(=%d) c-function while ",readIntegerOut);
                    fprintf(stderr,"attempting to read the total number of ");
                    fprintf(stderr,"ridges in the mesh.\n");
                    closeTheFile(&meshFile);
                    return 0;
                }

                if (strcmp(readStringIn,"ges") || readIntegerIn<1)
                {
                    PRINT_ERROR("In readMeshFileAndAllocateMesh: expecting ");
                    fprintf(stderr,"Ridges Nri instead of ");
                    fprintf(stderr,"Rid%s %d ",readStringIn,readIntegerIn);
                    fprintf(stderr,"(a positive integer value).\n");
                    closeTheFile(&meshFile);
                    return 0;
                }

                if (pMesh->nedg!=0)
                {
                    if (readIntegerIn!=pMesh->nedg)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: the ");
                        fprintf(stderr,"number of edges (=%d) ",pMesh->nedg);
                        fprintf(stderr,"should not be different from the ");
                        fprintf(stderr,"number of ridges (=%d) ",readIntegerIn);
                        fprintf(stderr,"in the mesh discretization.\n");
                        closeTheFile(&meshFile);
                        return 0;
                    }
                }
                else
                {
                    pMesh->nedg=readIntegerIn;

                    // Allocating memory for ridges and check if it worked
                    length=readIntegerIn;
                    pMesh->pedg=(Edge*)calloc(length,sizeof(Edge));
                    if (pMesh->pedg==NULL)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: could ");
                        fprintf(stderr,"not allocate memory for ");
                        fprintf(stderr,"%d ridges in the ",readIntegerIn);
                        fprintf(stderr,"structure pointed by pMesh.\n");
                        closeTheFile(&meshFile);
                        return 0;
                    }
                }

                // Checking if the ridges are well stored in increasing order
                for (k=0; k<readIntegerIn; k++)
                {
                    j=fscanf(meshFile,"%d ",&readIntegerOut);
                    if (j!=1)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: wrong ");
                        fprintf(stderr,"return (=%d) of the standard ",j);
                        fprintf(stderr,"fscanf c-function in the attempt of ");
                        fprintf(stderr,"reading the %d-th ridge of the ",k+1);
                        fprintf(stderr,"mesh discretization.\n");
                        closeTheFile(&meshFile);
                        return 0;
                    }
                    if (readIntegerOut!=k+1)
                    {
                        if (pParameters->verbose)
                        {
                            fprintf(stdout,"\nWarning in ");
                            fprintf(stdout,"readMeshFileAndAllocateMesh ");
                            fprintf(stdout,"function: the ridges of the mesh ");
                            fprintf(stdout,"discretization are not stored in ");
                            fprintf(stdout,"increasing order (the %d-th ",k+1);
                            fprintf(stdout,"ridge is referred to as ");
                            fprintf(stdout,"%d",readIntegerOut);
                            if (k>0)
                            {
                                fprintf(stdout," whereas the %d-th one ",k);
                                fprintf(stdout," was correctly numbered %d ",k);
                                fprintf(stdout,"as well as all the previous ");
                                fprintf(stdout,"ones");
                            }
                            fprintf(stdout,").\n");
                        }
                    }
                }
            }
            else if (!strcmp(keyword,"Cor"))
            {
                // Read the number of Corners and store it in pMesh->ncor
                readStringOut=fgets(readStringIn,5,meshFile);
                readIntegerOut=fscanf(meshFile," %d ",&readIntegerIn);

                if (readStringOut==NULL || readIntegerOut!=1)
                {
                    PRINT_ERROR("In readMeshFileAndAllocateMesh: wrong ");
                    fprintf(stderr,"return of the standard fgets ");
                    fprintf(stderr,"%p or fscanf ",(void*)readStringOut);
                    fprintf(stderr,"(=%d) c-function while ",readIntegerOut);
                    fprintf(stderr,"attempting to read the total number of ");
                    fprintf(stderr,"corners in the mesh.\n");
                    closeTheFile(&meshFile);
                    return 0;
                }

                if (strcmp(readStringIn,"ners") || readIntegerIn<1 ||
                                                      readIntegerIn>pMesh->nver)
                {
                    PRINT_ERROR("In readMeshFileAndAllocateMesh: expecting ");
                    fprintf(stderr,"Corners Ncor instead of ");
                    fprintf(stderr,"Cor%s %d ",readStringIn,readIntegerIn);
                    fprintf(stderr,"(a positive integer value that cannot be ");
                    fprintf(stderr,"(strictly) greater than the total number ");
                    fprintf(stderr,"of points %d in the mesh).\n",pMesh->nver);
                    closeTheFile(&meshFile);
                    return 0;
                }

                if (pMesh->ncor!=0)
                {
                    if (readIntegerIn!=pMesh->ncor)
                    {
                        if (pParameters->verbose)
                        {
                            fprintf(stdout,"\nWarning in ");
                            fprintf(stdout,"readMeshFileAndAllocateMesh ");
                            fprintf(stdout,"function: the number of required ");
                            fprintf(stdout,"vertices (=%d) ",pMesh->ncor);
                            fprintf(stdout,"should not be different from the ");
                            fprintf(stdout,"number of corners ");
                            fprintf(stdout,"(=%d) in the mesh ",readIntegerIn);
                            fprintf(stdout,"discretization.\n");
                        }
                    }
                }
                pMesh->ncor=readIntegerIn;

                // Check the number of corners by scanning all the values
                for (k=0; k<readIntegerIn; k++)
                {
                    j=fscanf(meshFile,"%d ",&readIntegerOut);
                    if (j!=1)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: wrong ");
                        fprintf(stderr,"return (=%d) of the standard ",j);
                        fprintf(stderr,"fscanf c-function in the attempt of ");
                        fprintf(stderr,"reading the %d-th corner of the ",k+1);
                        fprintf(stderr,"mesh discretization.\n");
                        closeTheFile(&meshFile);
                        return 0;
                    }
                }
            }
            else if (!strcmp(keyword,"Req"))
            {
                // Read number of RequiredVertices and store it in pMesh->ncor
                readStringOut=fgets(readStringIn,14,meshFile);
                readIntegerOut=fscanf(meshFile," %d ",&readIntegerIn);

                if (readStringOut==NULL || readIntegerOut!=1)
                {
                    PRINT_ERROR("In readMeshFileAndAllocateMesh: wrong ");
                    fprintf(stderr,"return of the standard fgets ");
                    fprintf(stderr,"%p or fscanf ",(void*)readStringOut);
                    fprintf(stderr,"(=%d) c-function while ",readIntegerOut);
                    fprintf(stderr,"attempting to read the total number of ");
                    fprintf(stderr,"required vertices in the mesh.\n");
                    closeTheFile(&meshFile);
                    return 0;
                }

                if (strcmp(readStringIn,"uiredVertices") || readIntegerIn<1 ||
                                                      readIntegerIn>pMesh->nver)
                {
                    PRINT_ERROR("In readMeshFileAndAllocateMesh: expecting ");
                    fprintf(stderr,"RequiredVertices Nreq instead of ");
                    fprintf(stderr,"Req%s %d ",readStringIn,readIntegerIn);
                    fprintf(stderr,"(a positive integer value that cannot be ");
                    fprintf(stderr,"(strictly) greater than the total number ");
                    fprintf(stderr,"of points %d in the mesh).\n",pMesh->nver);
                    closeTheFile(&meshFile);
                    return 0;
                }

                if (pMesh->ncor!=0)
                {
                    if (readIntegerIn!=pMesh->ncor)
                    {
                        if (pParameters->verbose)
                        {
                            fprintf(stdout,"\nWarning in ");
                            fprintf(stdout,"readMeshFileAndAllocateMesh ");
                            fprintf(stdout,"function: the number of corners ");
                            fprintf(stdout,"(=%d) should not be ",pMesh->ncor);
                            fprintf(stdout,"different from the number of ");
                            fprintf(stdout,"required vertices ");
                            fprintf(stdout,"(=%d) in the mesh ",readIntegerIn);
                            fprintf(stdout,"discretization.\n");
                        }
                    }
                }
                else
                {
                    pMesh->ncor=readIntegerIn;
                }

                // Check the number of required vertices
                for (k=0; k<readIntegerIn; k++)
                {
                    j=fscanf(meshFile,"%d ",&readIntegerOut);
                    if (j!=1)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: wrong ");
                        fprintf(stderr,"return (=%d) of the standard ",j);
                        fprintf(stderr,"fscanf c-function in the attempt of ");
                        fprintf(stderr,"reading the %d-th required ",k+1);
                        fprintf(stderr,"vertices of the mesh ");
                        fprintf(stderr,"discretization.\n");
                        closeTheFile(&meshFile);
                        return 0;
                    }
                }
            }
            else if (!strcmp(keyword,"Tet"))
            {
                // Read the number of tetrahedra and store it in pMesh->ntet
                readStringOut=fgets(readStringIn,8,meshFile);
                readIntegerOut=fscanf(meshFile," %d ",&readIntegerIn);

                if (readStringOut==NULL || readIntegerOut!=1)
                {
                    PRINT_ERROR("In readMeshFileAndAllocateMesh: wrong ");
                    fprintf(stderr,"return of the standard fgets ");
                    fprintf(stderr,"%p or fscanf ",(void*)readStringOut);
                    fprintf(stderr,"(=%d) c-function while ",readIntegerOut);
                    fprintf(stderr,"attempting to read the total number of ");
                    fprintf(stderr,"tetrahedra in the mesh.\n");
                    closeTheFile(&meshFile);
                    return 0;
                }

                if (strcmp(readStringIn,"rahedra") || readIntegerIn<1)
                {
                    PRINT_ERROR("In readMeshFileAndAllocateMesh: expecting ");
                    fprintf(stderr,"Tetrahedra Ntet instead of ");
                    fprintf(stderr,"Tet%s %d ",readStringIn,readIntegerIn);
                    fprintf(stderr,"(a positive integer value).\n");
                    closeTheFile(&meshFile);
                    return 0;
                }
                pMesh->ntet=readIntegerIn;

                // Allocating memory for tetrahedra and check if it worked
                length=readIntegerIn;
                pMesh->ptet=(Tetrahedron*)calloc(length,sizeof(Tetrahedron));
                if (pMesh->ptet==NULL)
                {
                    PRINT_ERROR("In readMeshFileAndAllocateMesh: could not ");
                    fprintf(stderr,"allocate memory for %d ",readIntegerIn);
                    fprintf(stderr,"tetrahedra in the structure pointed ");
                    fprintf(stderr,"by pMesh.\n");
                    closeTheFile(&meshFile);
                    return 0;
                }

                // Save tetrahedra in pMesh->ptet and check if it worked
                for (k=0; k<readIntegerIn; k++)
                {
                    j=0;
                    j+=fscanf(meshFile,"%d ",&readIntegerOut);
                    if (readIntegerOut<1 || readIntegerOut>pMesh->nver)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: the ");
                        fprintf(stderr,"first vertex of the %d-th ",k+1);
                        fprintf(stderr,"tetrahedron is referred to as ");
                        fprintf(stderr,"%d, which should be ",readIntegerOut);
                        fprintf(stderr,"a positive integer not (strictly) ");
                        fprintf(stderr,"greater than the total number of ");
                        fprintf(stderr,"points %d in the mesh.\n",pMesh->nver);
                        closeTheFile(&meshFile);
                        return 0;
                    }
                    pMesh->ptet[k].p1=readIntegerOut;

                    j+=fscanf(meshFile,"%d ",&readIntegerOut);
                    if (readIntegerOut<1 || readIntegerOut>pMesh->nver)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: the ");
                        fprintf(stderr,"second vertex of the %d-th ",k+1);
                        fprintf(stderr,"tetrahedron is referred to as ");
                        fprintf(stderr,"%d, which should be ",readIntegerOut);
                        fprintf(stderr,"a positive integer not (strictly) ");
                        fprintf(stderr,"greater than the total number of ");
                        fprintf(stderr,"points %d in the mesh.\n",pMesh->nver);
                        closeTheFile(&meshFile);
                        return 0;
                    }
                    pMesh->ptet[k].p2=readIntegerOut;

                    j+=fscanf(meshFile,"%d ",&readIntegerOut);
                    if (readIntegerOut<1 || readIntegerOut>pMesh->nver)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: the ");
                        fprintf(stderr,"third vertex of the %d-th ",k+1);
                        fprintf(stderr,"tetrahedron is referred to as ");
                        fprintf(stderr,"%d, which should be ",readIntegerOut);
                        fprintf(stderr,"a positive integer not (strictly) ");
                        fprintf(stderr,"greater than the total number of ");
                        fprintf(stderr,"points %d in the mesh.\n",pMesh->nver);
                        closeTheFile(&meshFile);
                        return 0;
                    }
                    pMesh->ptet[k].p3=readIntegerOut;

                    j+=fscanf(meshFile,"%d ",&readIntegerOut);
                    if (readIntegerOut<1 || readIntegerOut>pMesh->nver)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: the ");
                        fprintf(stderr,"fourth vertex of the %d-th ",k+1);
                        fprintf(stderr,"tetrahedron is referred to as ");
                        fprintf(stderr,"%d, which should be ",readIntegerOut);
                        fprintf(stderr,"a positive integer not (strictly) ");
                        fprintf(stderr,"greater than the total number of ");
                        fprintf(stderr,"points %d in the mesh.\n",pMesh->nver);
                        closeTheFile(&meshFile);
                        return 0;
                    }
                    pMesh->ptet[k].p4=readIntegerOut;

                    j+=fscanf(meshFile,"%d ",&readIntegerOut);
                    pMesh->ptet[k].label=readIntegerOut;
                    if (j!=5)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: we were ");
                        fprintf(stderr,"not able to properly save the ");
                        fprintf(stderr,"Tetrahedron data associated with ");
                        fprintf(stderr,"with the %d-th vertex of the ",k+1);
                        fprintf(stderr,"mesh (only %d values properly ",j);
                        fprintf(stderr,"saved instead of five).\n");
                        closeTheFile(&meshFile);
                        return 0;
                    }
                }
            }
            else if (!strcmp(keyword,"Nor"))
            {
                readStringOut=fgets(readStringIn,5,meshFile);
                if (readStringOut==NULL)
                {
                    PRINT_ERROR("In readMeshFileAndAllocateMesh: wrong ");
                    fprintf(stderr,"return %p of the ",(void*)readStringOut);
                    fprintf(stderr,"standard fgets c-function in the attempt ");
                    fprintf(stderr,"of reading the keywords related to the ");
                    fprintf(stderr,"normal vectors of the mesh ");
                    fprintf(stderr,"discretization.\n");
                    closeTheFile(&meshFile);
                    return 0;
                }

                if (!strcmp(readStringIn,"malA"))
                {
                    // Read number of NormalAtVertices, store it in pMesh->nnorm
                    readStringOut=fgets(readStringIn,10,meshFile);
                    readIntegerOut=fscanf(meshFile," %d ",&readIntegerIn);

                    if (readStringOut==NULL || readIntegerOut!=1)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: wrong ");
                        fprintf(stderr,"return of the standard fgets ");
                        fprintf(stderr,"%p or ",(void*)readStringOut);
                        fprintf(stderr,"fscanf (=%d) ",readIntegerOut);
                        fprintf(stderr,"c-function while attempting to read ");
                        fprintf(stderr,"the total number of normal vectors ");
                        fprintf(stderr,"prescribed at some vertices of the ");
                        fprintf(stderr,"mesh.\n");
                        closeTheFile(&meshFile);
                        return 0;
                    }

                    if (strcmp(readStringIn,"tVertices") || readIntegerIn<1 ||
                                                      readIntegerIn>pMesh->nver)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: ");
                        fprintf(stderr,"expecting NormalAtVertices Nnorm ");
                        fprintf(stderr,"instead of NormalA%s ",readStringIn);
                        fprintf(stderr,"%d (a positive integer ",readIntegerIn);
                        fprintf(stderr,"value that cannot be (strictly) ");
                        fprintf(stderr,"greater than the total number of ");
                        fprintf(stderr,"points %d in the mesh).\n",pMesh->nver);
                        closeTheFile(&meshFile);
                        return 0;
                    }

                    if (pMesh->nnorm!=0)
                    {
                        if (readIntegerIn!=pMesh->nnorm)
                        {
                            PRINT_ERROR("In readMeshFileAndAllocateMesh: ");
                            fprintf(stderr,"the total number of normal ");
                            fprintf(stderr,"vectors (=%d) ",pMesh->nnorm);
                            fprintf(stderr,"should not be different from the ");
                            fprintf(stderr,"number of normal vectors ");
                            fprintf(stderr,"(=%d) prescribed ",readIntegerIn);
                            fprintf(stderr,"at some vertices of the mesh.\n");
                            closeTheFile(&meshFile);
                            return 0;
                        }
                    }
                    else
                    {
                        pMesh->nnorm=readIntegerIn;

                        // Allocate and check memory for NormalAtVertices
                        length=readIntegerIn;
                        pMesh->pnorm=(Vector*)calloc(length,sizeof(Vector));
                        if (pMesh->pnorm==NULL)
                        {
                            PRINT_ERROR("In readMeshFileAndAllocateMesh: ");
                            fprintf(stderr,"could not allocate memory for ");
                            fprintf(stderr,"%d normal vectors ",readIntegerIn);
                            fprintf(stderr,"prescribed at some vertices of ");
                            fprintf(stderr,"the mesh structure pointed by ");
                            fprintf(stderr,"pMesh.\n");
                            closeTheFile(&meshFile);
                            return 0;
                        }
                    }

                    // Save NormalAtVertices in pMesh->pnorm then check if it
                    // worked and if they are stored in increasing order
                    for (k=0; k<readIntegerIn; k++)
                    {
                        j=0;
                        j+=fscanf(meshFile,"%d ",&readIntegerOut);
                        if (readIntegerOut<1 || readIntegerOut>pMesh->nver)
                        {
                            PRINT_ERROR("In readMeshFileAndAllocateMesh: the ");
                            fprintf(stderr,"vertex associated with the ");
                            fprintf(stderr,"%d-th normal vector is ",k+1);
                            fprintf(stderr,"referred to as ");
                            fprintf(stderr,"%d, which should ",readIntegerOut);
                            fprintf(stderr,"be a positive integer not ");
                            fprintf(stderr,"(strictly) greater than the ");
                            fprintf(stderr,"total number of points ");
                            fprintf(stderr,"%d in the mesh.\n",pMesh->nver);
                            closeTheFile(&meshFile);
                            return 0;
                        }
                        pMesh->pnorm[k].p=readIntegerOut;

                        j+=fscanf(meshFile,"%d ",&readIntegerOut);
                        if (readIntegerOut!=k+1)
                        {
                            if (pParameters->verbose)
                            {
                                fprintf(stdout,"\nWarning in ");
                                fprintf(stdout,"readMeshFileAndAllocateMesh ");
                                fprintf(stdout,"function: the normal vectors ");
                                fprintf(stdout,"associated with some ");
                                fprintf(stderr,"vertices of the mesh ");
                                fprintf(stdout,"discretization are not ");
                                fprintf(stdout,"stored in increasing order ");
                                fprintf(stdout,"(the %d-th one is ",k+1);
                                fprintf(stdout,"referred to as ");
                                fprintf(stdout,"%d",readIntegerOut);
                                if (k>0)
                                {
                                    fprintf(stdout," whereas the %d-th one ",k);
                                    fprintf(stdout," was correctly numbered ");
                                    fprintf(stdout,"%d as well as all the ",k);
                                    fprintf(stdout,"previous ones");
                                }
                                fprintf(stdout,").\n");
                            }
                        }

                        if (j!=2)
                        {
                            PRINT_ERROR("In readMeshFileAndAllocateMesh: ");
                            fprintf(stderr,"wrong return (=%d instead of ",j);
                            fprintf(stderr,"two) of the standard fscanf ");
                            fprintf(stderr,"c-function in the attempt of ");
                            fprintf(stderr,"reading the %d-th ",k+1);
                            fprintf(stderr,"normal vector prescribed at a ");
                            fprintf(stderr,"vertex of the mesh.\n");
                            closeTheFile(&meshFile);
                            return 0;
                        }
                    }
                }
                else if (!strcmp(readStringIn,"mals"))
                {
                    // Read and store number of normal vectors in pMesh->nnorm
                    readIntegerOut=fscanf(meshFile," %d ",&readIntegerIn);
                    if (readIntegerOut!=1)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: wrong ");
                        fprintf(stderr,"return (=%d) of the ",readIntegerOut);
                        fprintf(stderr,"standard fscanf c-function in the ");
                        fprintf(stderr,"attempt of reading the number of ");
                        fprintf(stderr,"normal vectors.\n");
                        closeTheFile(&meshFile);
                        return 0;
                    }

                    if (readIntegerIn<1 || readIntegerIn>pMesh->nver)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: ");
                        fprintf(stderr,"expecting a positive integer not ");
                        fprintf(stderr,"(strictly) greater than the total ");
                        fprintf(stderr,"number of points %d in ",pMesh->nver);
                        fprintf(stderr,"the mesh instead of %d ",readIntegerIn);
                        fprintf(stderr,"for the total number of normal ");
                        fprintf(stderr,"vectors.\n");
                        closeTheFile(&meshFile);
                        return 0;
                    }

                    if (pMesh->nnorm!=0)
                    {
                        if (readIntegerIn!=pMesh->nnorm)
                        {
                            PRINT_ERROR("In readMeshFileAndAllocateMesh: ");
                            fprintf(stderr,"the number of normal vectors ");
                            fprintf(stderr,"(=%d) prescribed ",pMesh->nnorm);
                            fprintf(stderr,"at some vertices of the mesh ");
                            fprintf(stderr,"should not be different from the ");
                            fprintf(stderr,"total number of normal vectors ");
                            fprintf(stderr,"(=%d) defined in ",readIntegerIn);
                            fprintf(stderr,"the mesh.\n");
                            closeTheFile(&meshFile);
                            return 0;
                        }
                    }
                    else
                    {
                        pMesh->nnorm=readIntegerIn;

                        // Allocate and check memory for the normal vectors
                        length=readIntegerIn;
                        pMesh->pnorm=(Vector*)calloc(length,sizeof(Vector));
                        if (pMesh->pnorm==NULL)
                        {
                            PRINT_ERROR("In readMeshFileAndAllocateMesh: ");
                            fprintf(stderr,"could not allocate memory for ");
                            fprintf(stderr,"%d normal vectors ",readIntegerIn);
                            fprintf(stderr,"in the structure pointed by ");
                            fprintf(stderr,"pMesh.\n");
                            closeTheFile(&meshFile);
                            return 0;
                        }
                    }

                    // Save and check the normal vectors in pMesh->pnorm
                    for (k=0; k<readIntegerIn; k++)
                    {
                        j=0;
                        j+=fscanf(meshFile,"%lf ",&readDouble);
                        pMesh->pnorm[k].x=readDouble;

                        j+=fscanf(meshFile,"%lf ",&readDouble);
                        pMesh->pnorm[k].y=readDouble;

                        j+=fscanf(meshFile,"%lf ",&readDouble);
                        pMesh->pnorm[k].z=readDouble;

                        if (j!=3)
                        {
                            PRINT_ERROR("In readMeshFileAndAllocateMesh: ");
                            fprintf(stderr,"wrong return (=%d instead of ",j);
                            fprintf(stderr,"three) of the standard fscanf ");
                            fprintf(stderr,"c-function in the attempt of ");
                            fprintf(stderr,"reading the displacement ");
                            fprintf(stderr,"of the %d-th normal vector ",k+1);
                            fprintf(stderr,"prescribed at a vertex of the ");
                            fprintf(stderr,"mesh.\n");
                            closeTheFile(&meshFile);
                            return 0;
                        }
                    }
                }
                else
                {
                    PRINT_ERROR("In readMeshFileAndAllocateMesh: expecting ");
                    fprintf(stderr,"the keyword Normals or NormalAtVertices ");
                    fprintf(stderr,"instead of Nor%s.\n",readStringIn);
                    closeTheFile(&meshFile);
                    return 0;
                }
            }
            else if (!strcmp(keyword,"Tan"))
            {
                readStringOut=fgets(readStringIn,6,meshFile);
                if (readStringOut==NULL)
                {
                    PRINT_ERROR("In readMeshFileAndAllocateMesh: wrong ");
                    fprintf(stderr,"return %p of the ",(void*)readStringOut);
                    fprintf(stderr,"standard fgets c-function in the attempt ");
                    fprintf(stderr,"of reading the keywords related to the ");
                    fprintf(stderr,"tangent vectors of the mesh ");
                    fprintf(stderr,"discretization.\n");
                    closeTheFile(&meshFile);
                    return 0;
                }

                if (!strcmp(readStringIn,"gentA"))
                {
                    // Read number of TangentAtVertices, store it in pMesh->ntan
                    readStringOut=fgets(readStringIn,10,meshFile);
                    readIntegerOut=fscanf(meshFile," %d ",&readIntegerIn);

                    if (readStringOut==NULL || readIntegerOut!=1)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: wrong ");
                        fprintf(stderr,"return of the standard fgets ");
                        fprintf(stderr,"%p or ",(void*)readStringOut);
                        fprintf(stderr,"fscanf (=%d) ",readIntegerOut);
                        fprintf(stderr,"c-function while attempting to read ");
                        fprintf(stderr,"the total number of tangent vectors ");
                        fprintf(stderr,"prescribed at some vertices of the ");
                        fprintf(stderr,"mesh.\n");
                        closeTheFile(&meshFile);
                        return 0;
                    }

                    if (strcmp(readStringIn,"tVertices") || readIntegerIn<1 ||
                                                      readIntegerIn>pMesh->nver)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: ");
                        fprintf(stderr,"expecting TangentAtVertices Ntan ");
                        fprintf(stderr,"instead of TangentA%s ",readStringIn);
                        fprintf(stderr,"%d (a positive integer ",readIntegerIn);
                        fprintf(stderr,"value that cannot be (strictly) ");
                        fprintf(stderr,"greater than the total number of ");
                        fprintf(stderr,"points %d in the mesh).\n",pMesh->nver);
                        closeTheFile(&meshFile);
                        return 0;
                    }

                    if (pMesh->ntan!=0)
                    {
                        if (readIntegerIn!=pMesh->ntan)
                        {
                            PRINT_ERROR("In readMeshFileAndAllocateMesh: ");
                            fprintf(stderr,"the total number of tangent ");
                            fprintf(stderr,"vectors (=%d) should ",pMesh->ntan);
                            fprintf(stderr,"not be different from the number ");
                            fprintf(stderr,"of tangent vectors ");
                            fprintf(stderr,"(=%d) prescribed ",readIntegerIn);
                            fprintf(stderr,"at some vertices of the mesh.\n");
                            closeTheFile(&meshFile);
                            return 0;
                        }
                    }
                    else
                    {
                        pMesh->ntan=readIntegerIn;

                        // Allocate and check memory for TangentAtVertices
                        length=readIntegerIn;
                        pMesh->ptan=(Vector*)calloc(length,sizeof(Vector));
                        if (pMesh->ptan==NULL)
                        {
                            PRINT_ERROR("In readMeshFileAndAllocateMesh: ");
                            fprintf(stderr,"could not allocate memory for ");
                            fprintf(stderr,"%d tangent vectors ",readIntegerIn);
                            fprintf(stderr,"prescribed at some vertices of ");
                            fprintf(stderr,"the mesh structure pointed by ");
                            fprintf(stderr,"pMesh.\n");
                            closeTheFile(&meshFile);
                            return 0;
                        }
                    }

                    // Save TangentAtVertices in pMesh->ptan then check if it
                    // worked and if they are stored in increasing order
                    for (k=0; k<readIntegerIn; k++)
                    {
                        j=0;
                        j+=fscanf(meshFile,"%d ",&readIntegerOut);
                        if (readIntegerOut<1 || readIntegerOut>pMesh->nver)
                        {
                            PRINT_ERROR("In readMeshFileAndAllocateMesh: the ");
                            fprintf(stderr,"vertex associated with the ");
                            fprintf(stderr,"%d-th tangent vector is ",k+1);
                            fprintf(stderr,"referred to as ");
                            fprintf(stderr,"%d, which should ",readIntegerOut);
                            fprintf(stderr,"be a positive integer not ");
                            fprintf(stderr,"(strictly) greater than the ");
                            fprintf(stderr,"total number of points ");
                            fprintf(stderr,"%d in the mesh.\n",pMesh->nver);
                            closeTheFile(&meshFile);
                            return 0;
                        }
                        pMesh->ptan[k].p=readIntegerOut;

                        j+=fscanf(meshFile,"%d ",&readIntegerOut);
                        if (readIntegerOut!=k+1)
                        {
                            if (pParameters->verbose)
                            {
                                fprintf(stdout,"\nWarning in ");
                                fprintf(stdout,"readMeshFileAndAllocateMesh ");
                                fprintf(stdout,"function: the tangent ");
                                fprintf(stdout,"vectors associated with some ");
                                fprintf(stderr,"vertices of the mesh ");
                                fprintf(stdout,"discretization are not ");
                                fprintf(stdout,"stored in increasing order ");
                                fprintf(stdout,"(the %d-th one is ",k+1);
                                fprintf(stdout,"referred to as ");
                                fprintf(stdout,"%d",readIntegerOut);
                                if (k>0)
                                {
                                    fprintf(stdout," whereas the %d-th one ",k);
                                    fprintf(stdout," was correctly numbered ");
                                    fprintf(stdout,"%d as well as all the ",k);
                                    fprintf(stdout,"previous ones");
                                }
                                fprintf(stdout,").\n");
                            }
                        }

                        if (j!=2)
                        {
                            PRINT_ERROR("In readMeshFileAndAllocateMesh: ");
                            fprintf(stderr,"wrong return (=%d instead of ",j);
                            fprintf(stderr,"two) of the standard fscanf ");
                            fprintf(stderr,"c-function in the attempt of ");
                            fprintf(stderr,"reading the %d-th ",k+1);
                            fprintf(stderr,"tangent vector prescribed at ");
                            fprintf(stderr,"a vertex of the mesh.\n");
                            closeTheFile(&meshFile);
                            return 0;
                        }
                    }
                }
                else if (!strcmp(readStringIn,"gents"))
                {
                    // Read and store number of tangent vectors in pMesh->ntan
                    readIntegerOut=fscanf(meshFile," %d ",&readIntegerIn);
                    if (readIntegerOut!=1)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: wrong ");
                        fprintf(stderr,"return (=%d) of the ",readIntegerOut);
                        fprintf(stderr,"standard fscanf c-function in the ");
                        fprintf(stderr,"attempt of reading the number of ");
                        fprintf(stderr,"tangent vectors.\n");
                        closeTheFile(&meshFile);
                        return 0;
                    }

                    if (readIntegerIn<1 || readIntegerIn>pMesh->nver)
                    {
                        PRINT_ERROR("In readMeshFileAndAllocateMesh: ");
                        fprintf(stderr,"expecting a positive integer not ");
                        fprintf(stderr,"(strictly) greater than the total ");
                        fprintf(stderr,"number of points %d in ",pMesh->nver);
                        fprintf(stderr,"the mesh instead of %d ",readIntegerIn);
                        fprintf(stderr,"for the total number of tangent ");
                        fprintf(stderr,"vectors.\n");
                        closeTheFile(&meshFile);
                        return 0;
                    }

                    if (pMesh->ntan!=0)
                    {
                        if (readIntegerIn!=pMesh->ntan)
                        {
                            PRINT_ERROR("In readMeshFileAndAllocateMesh: ");
                            fprintf(stderr,"the number of tangent vectors ");
                            fprintf(stderr,"(=%d) prescribed ",pMesh->ntan);
                            fprintf(stderr,"at some vertices of the mesh ");
                            fprintf(stderr,"should not be different from the ");
                            fprintf(stderr,"total number of tangent vectors ");
                            fprintf(stderr,"(=%d) defined in ",readIntegerIn);
                            fprintf(stderr,"the mesh.\n");
                            closeTheFile(&meshFile);
                            return 0;
                        }
                    }
                    else
                    {
                        pMesh->ntan=readIntegerIn;

                        // Allocate and check memory for the tangent vectors
                        length=readIntegerIn;
                        pMesh->ptan=(Vector*)calloc(length,sizeof(Vector));
                        if (pMesh->ptan==NULL)
                        {
                            PRINT_ERROR("In readMeshFileAndAllocateMesh: ");
                            fprintf(stderr,"could not allocate memory for ");
                            fprintf(stderr,"%d tangent vectors ",readIntegerIn);
                            fprintf(stderr,"in the structure pointed by ");
                            fprintf(stderr,"pMesh.\n");
                            closeTheFile(&meshFile);
                            return 0;
                        }
                    }

                    // Save and check the tangent vectors in pMesh->ptan
                    for (k=0; k<readIntegerIn; k++)
                    {
                        j=0;
                        j+=fscanf(meshFile,"%lf ",&readDouble);
                        pMesh->ptan[k].x=readDouble;

                        j+=fscanf(meshFile,"%lf ",&readDouble);
                        pMesh->ptan[k].y=readDouble;

                        j+=fscanf(meshFile,"%lf ",&readDouble);
                        pMesh->ptan[k].z=readDouble;

                        if (j!=3)
                        {
                            PRINT_ERROR("In readMeshFileAndAllocateMesh: ");
                            fprintf(stderr,"wrong return (=%d instead of ",j);
                            fprintf(stderr,"three) of the standard fscanf ");
                            fprintf(stderr,"c-function in the attempt of ");
                            fprintf(stderr,"reading the displacement ");
                            fprintf(stderr,"of the %d-th tangent vector ",k+1);
                            fprintf(stderr,"prescribed at a vertex of the ");
                            fprintf(stderr,"mesh.\n");
                            closeTheFile(&meshFile);
                            return 0;
                        }
                    }
                }
                else
                {
                    PRINT_ERROR("In readMeshFileAndAllocateMesh: expecting ");
                    fprintf(stderr,"the keyword Tangents or ");
                    fprintf(stderr,"TangentAtVertices instead of ");
                    fprintf(stderr,"Tan%s.\n",readStringIn);
                    closeTheFile(&meshFile);
                    return 0;
                }
            }
            else if (!strcmp(keyword,"End"))
            {
                i=11;
            }
            else
            {
                PRINT_ERROR("In readMeshFileAndAllocateMesh: the ");
                fprintf(stderr,"%d-th string read (=%s) does not ",i,keyword);
                fprintf(stderr,"correspond to any known keyword in the ");
                fprintf(stderr,"tetrahedral mesh structure pointed by ");
                fprintf(stderr,"pMesh.\n");
                closeTheFile(&meshFile);
                return 0;
            }
        } while (i<11);
    }

    // Testing if file is well closed
    if (fclose(meshFile))
    {
        PRINT_ERROR("In readMeshFileAndAllocateMesh: the");
        fprintf(stderr,"%s file has not been closed ",pParameters->name_mesh);
        fprintf(stderr,"properly.\n");
        meshFile=NULL;
        return 0;
    }
    meshFile=NULL;
    if (pParameters->opt_mode!=1 || pParameters->verbose)
    {
        fprintf(stdout,"Closing file.\n");
    }

    if (pParameters->opt_mode!=1 || pParameters->verbose)
    {
        fprintf(stdout,"\nMesh data successfully loaded:\n");
        if (pParameters->opt_mode>0)
        {
            fprintf(stdout,"%d vertices, ",pMesh->nver);
            fprintf(stdout,"%d tetrahedra ",pMesh->ntet);
            fprintf(stdout,"and %d boundary triangles\n",pMesh->ntri);
            fprintf(stdout,"(%d corners, %d edges, ",pMesh->ncor,pMesh->nedg);
            fprintf(stdout,"%d tangent and ",pMesh->ntan);
            fprintf(stdout,"%d normal vectors).\n",pMesh->nnorm);
        }
        else
        {
            fprintf(stdout,"%d vertices, ",pMesh->nver);
            fprintf(stdout,"%d hexahedra ",pMesh->nhex);
            fprintf(stdout,"and %d boundary quadrilaterals.\n",pMesh->nqua);
        }
    }

    return 1;
}

/* ************************************************************************** */
// The function updateDiscretizationParameters looks at the mesh dicretization
// recently stored in the structure pointed by pMesh and first update the x_min,
// y_min,  z_min, x_max, y_max, and z_max variables of the structure pointed by
// pParameters, which corresponds to the extremal values bounding the
// computational box. In addition, if the opt_mode variable of the structure
// pointed by pParameters is not positive, the mesh has an hexahedral structure
// and we update the discretization parameters stored in the delta_x, delta_y,
// delta_z, n_x, n_y and n_z variables of the structure pointed by pParameters.
// It has the Parameters* and Mesh* variable (both defined in main.h) as input
// arguments and it returns one on success, otherwise zero if an error is found
/* ************************************************************************** */
int updateDiscretizationParameters(Parameters* pParameters, Mesh* pMesh)
{
    int nX=0, nY=0, nZ=0, i=0, iMax=0, j=0, k=0, boolean;
    double deltaX=0., deltaY=0., deltaZ=0., xMin=0., yMin=0., zMin=0., xMax=0.;
    double yMax=0., zMax=0., cx=0., cy=0., cz=0.;
    Point *pVertex=NULL;

    // Check if the input pParameters or pMesh variable is pointing to NULL
    if (pParameters==NULL || pMesh==NULL)
    {
        PRINT_ERROR("In updateDiscretizationParameters: at least one of the ");
        fprintf(stderr,"input pParameters %p or ",(void*)pParameters);
        fprintf(stderr,"pParameters %p variable does not have a ",(void*)pMesh);
        fprintf(stderr,"valid address.\n");
        return 0;
    }

    // Check the pMesh->pver variable
    if (pMesh->pver==NULL)
    {
        PRINT_ERROR("In updateDiscretizationParameters: no Point data have ");
        fprintf(stderr,"been saved in the structure pointed by pMesh since ");
        fprintf(stderr,"its pver variable is pointing to the ");
        fprintf(stderr,"%p address.\n",(void*)pMesh->pver);
        return 0;
    }

    // Check the number of vertices
    if (pMesh->nver<9)
    {
        PRINT_ERROR("In updateDiscretizationParameters: we were expecting ");
        fprintf(stderr,"at least 9(=3x3x3) instead of %d for the ",pMesh->nver);
        fprintf(stderr,"total number of vertices stored in the structure ");
        fprintf(stderr,"pointed by pMesh.\n");
        return 0;
    }

    // Getting the bounding computational box
    xMin=pMesh->pver[0].x;
    yMin=pMesh->pver[0].y;
    zMin=pMesh->pver[0].z;

    xMax=xMin;
    yMax=yMin;
    zMax=zMin;

    iMax=pMesh->nver;
    for (i=1; i<iMax; i++)
    {
        cx=pMesh->pver[i].x;
        if (cx<xMin)
        {
            xMin=cx;
        }
        else if (cx>xMax)
        {
            xMax=cx;
        }

        cy=pMesh->pver[i].y;
        if (cy<yMin)
        {
            yMin=cy;
        }
        else if (cy>yMax)
        {
            yMax=cy;
        }

        cz=pMesh->pver[i].z;
        if (cz<zMin)
        {
            zMin=cz;
        }
        else if (cz>zMax)
        {
            zMax=cz;
        }
    }

    // Check that the bounding computational box is not empty
    if (xMin>=xMax || yMin>=yMax || zMin>=zMax)
    {
        PRINT_ERROR("In updateDiscretizationParameters: expecting\n");
        fprintf(stderr,"(xMin==%lf) < (xMax==%lf)\n",xMin,xMax);
        fprintf(stderr,"(yMin==%lf) < (yMax==%lf)\n",yMin,yMax);
        fprintf(stderr,"(zMin==%lf) < (zMax==%lf)\n",zMin,zMax);
        fprintf(stderr,"for the bounding computational box associated with");
        fprintf(stderr,"the mesh discretization stored in the structure ");
        fprintf(stderr,"pointed by pMesh.\n");
        return 0;
    }

    // Update the new bounding box parameters
    pParameters->x_min=xMin;
    pParameters->y_min=yMin;
    pParameters->z_min=zMin;

    pParameters->x_max=xMax;
    pParameters->y_max=yMax;
    pParameters->z_max=zMax;

    if (pParameters->verbose)
    {
        fprintf(stdout,"\nThe bounding computational box associated with the ");
        fprintf(stdout,"mesh has been updated:\n[%lf,%lf]x",xMin,xMax);
        fprintf(stdout,"[%lf,%lf]x[%lf,%lf].\n",yMin,yMax,zMin,zMax);
    }

    // Detect an hexahedral structure in the vertex description
    boolean=0;
    deltaX=DEF_ABS(pMesh->pver[1].x-pMesh->pver[0].x);
    deltaY=DEF_ABS(pMesh->pver[1].y-pMesh->pver[0].y);
    deltaZ=DEF_ABS(pMesh->pver[1].z-pMesh->pver[0].z);
    if (deltaX<1.e-15 && deltaY<1.e-15)
    {
        cx=DEF_ABS(pMesh->pver[2].x-pMesh->pver[1].x);
        cy=DEF_ABS(pMesh->pver[2].y-pMesh->pver[1].y);
        cz=DEF_ABS(pMesh->pver[2].z-pMesh->pver[1].z);
        if (cx<1.e-15 && cy<1.e-15 && DEF_ABS(cz-deltaZ)<1.e-15)
        {
            boolean=1;
            if (pParameters->verbose)
            {
                fprintf(stdout,"Hexahedral grid structure detected in the ");
                fprintf(stdout,"mesh.\n");
            }
        }
    }

    if (boolean)
    {
        // Get some guess for the n_x, n_y and n_z variables of pMesh
        iMax=pMesh->nver;

        i=0;
        do {
            i++;
            deltaY=DEF_ABS(pMesh->pver[i].y-pMesh->pver[i-1].y);
        } while (deltaY<1.e-16 && i<iMax);
        nZ=i;

        i=0;
        do {
            i++;
            deltaX=DEF_ABS(pMesh->pver[i].x-pMesh->pver[i-1].x);
        } while (deltaX<1.e-16 && i<iMax);
        nY=i/nZ;

        if (i%nZ!=0)
        {
            PRINT_ERROR("In updateDiscretizationParameters: expecting ");
            fprintf(stderr,"%d instead of %d for the local variable ",nY*nZ,i);
            fprintf(stderr,"i, in the attempt of updating the n_y and n_z ");
            fprintf(stderr,"variables of the structure pointed by pMesh.\n");
            return 0;
        }
        nX=iMax/i;

        if (iMax%i!=0)
        {
            PRINT_ERROR("In updateDiscretizationParameters: expecting ");
            fprintf(stderr,"%d instead of %d for the local ",nX*nY*nZ,iMax);
            fprintf(stderr,"variable i, in the attempt of updating the n_x ");
            fprintf(stderr,"variable of the structure pointed by pMesh.\n");
            return 0;
        }

        if (nX<3 || nY<3 || nZ<3)
        {
            PRINT_ERROR("In updateDiscretizationParameters: expecting ");
            fprintf(stderr,"that the values (nX=%d, nY=%d, nZ=%d) ",nX,nY,nZ);
            fprintf(stderr,"suggested for the n_x, n_y and n_z variable of ");
            fprintf(stderr,"the structure pointed by pMesh would be integers ");
            fprintf(stderr,"(strictly) greater than two.\n");
            return 0;
        }

        // Get a guess for the delta_x, delta_y and delta_z variables of pMesh
        deltaX=(xMax-xMin)/(double)(nX-1);
        deltaY=(yMax-yMin)/(double)(nY-1);
        deltaZ=(zMax-zMin)/(double)(nZ-1);

        // Check if the guessed values are correct
        for (i=0; i<nX; i++)
        {
            for (j=0; j<nY; j++)
            {
                for (k=0; k<nZ; k++)
                {
                    iMax=(i*nY+j)*nZ+k;
                    pVertex=&pMesh->pver[iMax];

                    cx=xMin+i*deltaX;
                    cy=yMin+j*deltaY;
                    cz=zMin+k*deltaZ;

                    if (DEF_ABS(pVertex->x-cx)>=1.e-14)
                    {
                        PRINT_ERROR("updateDiscretizationParameters: ");
                        fprintf(stderr,"expecting %.18lf instead of ",cx);
                        fprintf(stderr,"%.18lf (up to 15 ",pVertex->x);
                        fprintf(stderr,"decimals) for the first coordinate ");
                        fprintf(stderr,"of the %d-th vertex ",iMax+1);
                        fprintf(stderr,"associated with the hexahedral grid ");
                        fprintf(stderr,"structure detected and pointed by ");
                        fprintf(stderr,"pMesh.\n");
                        return 0;
                    }

                    if (DEF_ABS(pVertex->y-cy)>=1.e-14)
                    {
                        PRINT_ERROR("updateDiscretizationParameters: ");
                        fprintf(stderr,"expecting %.18lf instead of ",cy);
                        fprintf(stderr,"%.18lf (up to 15 ",pVertex->y);
                        fprintf(stderr,"decimals) for the second coordinate ");
                        fprintf(stderr,"of the %d-th vertex ",iMax+1);
                        fprintf(stderr,"associated with the hexahedral grid ");
                        fprintf(stderr,"structure detected and pointed by ");
                        fprintf(stderr,"pMesh.\n");
                        return 0;
                    }

                    if (DEF_ABS(pVertex->z-cz)>=1.e-14)
                    {
                        PRINT_ERROR("updateDiscretizationParameters: ");
                        fprintf(stderr,"expecting %.18lf instead of ",cz);
                        fprintf(stderr,"%.18lf (up to 15 ",pVertex->z);
                        fprintf(stderr,"decimals) for the third coordinate ");
                        fprintf(stderr,"of the %d-th vertex ",iMax+1);
                        fprintf(stderr,"associated with the hexahedral grid ");
                        fprintf(stderr,"structure detected and pointed by ");
                        fprintf(stderr,"pMesh.\n");
                        return 0;
                    }
                }
            }
        }

        // Update the discretization parameters accordingly
        pParameters->n_x=nX;
        pParameters->n_y=nY;
        pParameters->n_z=nZ;

        pParameters->delta_x=deltaX;
        pParameters->delta_y=deltaY;
        pParameters->delta_z=deltaZ;

        if (pParameters->verbose)
        {
                fprintf(stdout,"Discretization parameters were updated ");
                fprintf(stdout,"accordingly:\n(%lf)x(%lf)x",deltaX,deltaY);
                fprintf(stdout,"(%lf) with (%d)x(%d)x(%d) ",deltaZ,nX,nY,nZ);
                fprintf(stdout,"points.\n");
        }
    }
    else
    {
        if (pParameters->opt_mode<=0)
        {
            PRINT_ERROR("In updateDiscretizationParameters: no hexahedral ");
            fprintf(stderr,"grid structure was found in the description ");
            fprintf(stderr,"of the mesh vertices, which is not possible ");
            fprintf(stderr,"here since the pParameters->opt_mode variable ");
            fprintf(stderr,"(=%d) is not positive.\n",pParameters->opt_mode);
            return 0;
        }
        else
        {
            if (pParameters->verbose)
            {
                fprintf(stdout,"No hexahedral grid structure found in the ");
                fprintf(stdout,"mesh.\n");
            }
        }
    }

    return 1;
}

/* ************************************************************************** */
// The function checkCompatibilityOfChemicalData opens the *.chem file whose
// path name is stored in the fileLocation variable, a string of length
// (strictly less than the nameLength variable). Then, it reads the number of
// nuclei and their descriptions to finally compare them to the one stored in
// the array (of size nNcul) pointed by pNucl. It has the Nucleus* (defined in
// main.h), the char* fileLocation and two int (nameLength and pNucl) variables
// as input arguments and it returns one on succes otherwise zero is returned
/* ************************************************************************** */
int checkCompatibilityOfChemicalData(char* fileLocation, int nameLength,
                                                      Nucleus* pNucl, int nNucl)
{
    size_t length=0;
    char *readStringOut=NULL, readStringIn[18]={'\0'};
    int readIntegerIn=0, readIntegerOut=0, i=0;
    double readDouble=0.;
    FILE *chemicalFile=NULL;

    // Check that pNucl is not pointing to NULL and that nNucl is positive
    if (pNucl==NULL)
    {
        PRINT_ERROR("In checkCompatibilityOfChemicalData: the input pNucl ");
        fprintf(stderr,"variable is pointing to the %p ",(void*)pNucl);
        fprintf(stderr,"address.\n");
        return 0;
    }

    if (nNucl<1)
    {
        PRINT_ERROR("In checkCompatibilityOfChemicalData: the input nNucl ");
        fprintf(stderr,"variable (=%d) should be a positive integer.\n",nNucl);
        return 0;
    }

    // Check that fileLocation is storing the name of an existing *.chem file
    if (nameLength<7)
    {
        PRINT_ERROR("In checkCompatibilityOfChemicalData: the input ");
        fprintf(stderr,"nameLength (=%d) variable should be an ",nameLength);
        fprintf(stderr,"integer (strictly) greater than six.\n");
        return 0;
    }

    if (initialFileExists(fileLocation,nameLength)!=1)
    {
        PRINT_ERROR("In checkCompatibilityOfChemicalData: initialFileExists ");
        fprintf(stderr,"function did not returned one, which was the ");
        fprintf(stderr,"expected value here.\n");
        return 0;
    }

    // strlen returns the length of the string, but not including the char '\0'
    length=strlen(fileLocation);
    if (length<6)
    {
        PRINT_ERROR("In checkCompatibilityOfChemicalData: the input ");
        fprintf(stderr,"fileLocation variable (=%s) should have ",fileLocation);
        fprintf(stderr,"a length that at least equals six in order to end ");
        fprintf(stderr,"with something more than the '.chem' extension.\n");
        return 0;
    }

    if (fileLocation[length-5]!='.' || fileLocation[length-4]!='c' ||
                   fileLocation[length-3]!='h' || fileLocation[length-2]!='e' ||
                      fileLocation[length-1]!='m' || fileLocation[length]!='\0')
    {
        PRINT_ERROR("In checkCompatibilityOfChemicalData: the input ");
        fprintf(stderr,"fileLocation variable (=%s) does not ",fileLocation);
        fprintf(stderr,"end with the '.chem' extension.\n");
        return 0;
    }

    // Check if the *.chem file is well opened: fopen function returns a FILE
    // pointer on success, otherwise NULL is returned
    fprintf(stdout,"Opening %s file. ",fileLocation);
    chemicalFile=fopen(fileLocation,"r");
    if (chemicalFile==NULL)
    {
        PRINT_ERROR("In checkCompatibilityOfChemicalData: could not ");
        fprintf(stderr,"read the content of %s file.\n",fileLocation);
        return 0;
    }
    fprintf(stdout,"Checking Nucleus data. ");

    // fscanf function returns the number of input items successfully matched
    // and assigned, which can be fewer than provided for. Important remark:
    // white space characters, such as blanks and newline characters, causes
    // fscanf (scanf and sscanf) to read, but not to store, all consecutive
    // white space characters in the input up to the next character that is not
    // white space. One white space character in format-string matches any
    // combination of white space characters in the input
    fscanf(chemicalFile," ");

    // Read number of molecular orbitals: fgets returns NULL if it fails,
    // otherwise the same (char*) input address
    readStringOut=fgets(readStringIn,18,chemicalFile);
    readIntegerOut=fscanf(chemicalFile," %d ",&readIntegerIn);
    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In checkCompatibilityOfChemicalData: wrong return ");
        fprintf(stderr,"of the fgets %p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) function while attempting to ",readIntegerOut);
        fprintf(stderr,"read the number of molecular orbitals.\n");
        closeTheFile(&chemicalFile);
        return 0;
    }

    // strcmp returns 0 if the two strings are equal, otherwise <0 (resp. >0)
    // if the 1st string argument is shorter (resp. longer) than the 2nd one
    if (strcmp(readStringIn,"MolecularOrbitals") || readIntegerIn<1)
    {
        PRINT_ERROR("In checkCompatibilityOfChemicalData: expecting ");
        fprintf(stderr,"MolecularOrbitals Nmorb instead of %s ",readStringIn);
        fprintf(stderr,"%d (positive value).\n",readIntegerIn);
        closeTheFile(&chemicalFile);
        return 0;
    }

    // Read number of primitives
    readStringOut=fgets(readStringIn,11,chemicalFile);
    readIntegerOut=fscanf(chemicalFile," %d ",&readIntegerIn);
    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In checkCompatibilityOfChemicalData: wrong return ");
        fprintf(stderr,"of the fgets %p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) function while attempting to ",readIntegerOut);
        fprintf(stderr,"read the number of primitives.\n");
        closeTheFile(&chemicalFile);
        return 0;
    }

    if (strcmp(readStringIn,"Primitives") || readIntegerIn<1)
    {
        PRINT_ERROR("In checkCompatibilityOfChemicalData: expecting ");
        fprintf(stderr,"Primitives Ngauss instead of %s ",readStringIn);
        fprintf(stderr,"%d (positive value).\n",readIntegerIn);
        closeTheFile(&chemicalFile);
        return 0;
    }

    // Read the number of nuclei and compare it to the nNucl variable
    readStringOut=fgets(readStringIn,7,chemicalFile);
    readIntegerOut=fscanf(chemicalFile," %d ",&readIntegerIn);
    if (readStringOut==NULL || readIntegerOut!=1)
    {
        PRINT_ERROR("In checkCompatibilityOfChemicalData: wrong return ");
        fprintf(stderr,"of the fgets %p or fscanf ",(void*)readStringOut);
        fprintf(stderr,"(=%d) function while attempting to ",readIntegerOut);
        fprintf(stderr,"read the number of nuclei.\n");
        closeTheFile(&chemicalFile);
        return 0;
    }

    if (strcmp(readStringIn,"Nuclei") || readIntegerIn!=nNucl)
    {
        PRINT_ERROR("In checkCompatibilityOfChemicalData: expecting ");
        fprintf(stderr,"Nuclei %d instead of %s ",nNucl,readStringIn);
        fprintf(stderr,"%d.\n",readIntegerIn);
        closeTheFile(&chemicalFile);
        return 0;
    }

    // Saving the Nucleus data in pMesh->pnucl and check if it worked
    for (i=0; i<nNucl; i++)
    {
        readIntegerOut=fscanf(chemicalFile,"%lf ",&readDouble);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In checkCompatibilityOfChemicalData: we were ");
            fprintf(stderr,"not able to properly read the first-coordinate ");
            fprintf(stderr,"position of the %d-th Nucleus.\n",i+1);
            closeTheFile(&chemicalFile);
            return 0;
        }

        if (DEF_ABS(pNucl[i].x-readDouble)>=1.e-4)
        {
            PRINT_ERROR("In checkCompatibilityOfChemicalData: expecting ");
            fprintf(stderr,"%lf instead of %lf for the ",pNucl[i].x,readDouble);
            fprintf(stderr,"first coordinate of the %d-th Nucleus.\n",i+1);
            closeTheFile(&chemicalFile);
            return 0;
        }

        readIntegerOut=fscanf(chemicalFile,"%lf ",&readDouble);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In checkCompatibilityOfChemicalData: we were ");
            fprintf(stderr,"not able to properly read the second-coordinate ");
            fprintf(stderr,"position of the %d-th Nucleus.\n",i+1);
            closeTheFile(&chemicalFile);
            return 0;
        }

        if (DEF_ABS(pNucl[i].y-readDouble)>=1.e-4)
        {
            PRINT_ERROR("In checkCompatibilityOfChemicalData: expecting ");
            fprintf(stderr,"%lf instead of %lf for the ",pNucl[i].y,readDouble);
            fprintf(stderr,"second coordinate of the %d-th Nucleus.\n",i+1);
            closeTheFile(&chemicalFile);
            return 0;
        }

        readIntegerOut=fscanf(chemicalFile,"%lf ",&readDouble);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In checkCompatibilityOfChemicalData: we were ");
            fprintf(stderr,"not able to properly read the third-coordinate ");
            fprintf(stderr,"position of the %d-th Nucleus.\n",i+1);
            closeTheFile(&chemicalFile);
            return 0;
        }

        if (DEF_ABS(pNucl[i].z-readDouble)>=1.e-4)
        {
            PRINT_ERROR("In checkCompatibilityOfChemicalData: expecting ");
            fprintf(stderr,"%lf instead of %lf for the ",pNucl[i].z,readDouble);
            fprintf(stderr,"third coordinate of the %d-th Nucleus.\n",i+1);
            closeTheFile(&chemicalFile);
            return 0;
        }

        readIntegerOut=fscanf(chemicalFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In checkCompatibilityOfChemicalData: wrong ");
            fprintf(stderr,"(=%d) return of the fscanf ",readIntegerOut);
            fprintf(stderr,"function while attempting to read the charge of ");
            fprintf(stderr,"the %d-th Nucleus structure.\n",i+1);
            closeTheFile(&chemicalFile);
            return 0;
        }

        if (readIntegerIn<1 || readIntegerIn>100)
        {
            PRINT_ERROR("In checkCompatibilityOfChemicalData: the ");
            fprintf(stderr,"charge (=%d) of the %d-th ",readIntegerIn,i+1);
            fprintf(stderr,"Nucleus must be a positive integer that is not ");
            fprintf(stderr,"(strictly) greater than one hundred.\n");
            closeTheFile(&chemicalFile);
            return 0;
        }

        if (pNucl[i].charge!=readIntegerIn)
        {
            PRINT_ERROR("In checkCompatibilityOfChemicalData: expecting ");
            fprintf(stderr,"%d instead of %d ",pNucl[i].charge,readIntegerIn);
            fprintf(stderr,"for the charge of the %d-th Nucleus.\n",i+1);
            closeTheFile(&chemicalFile);
            return 0;
        }
    }

    // Closing the *.chem file: fclose function returns zero if the input FILE*
    // variable is successfully closed, otherwise EOF (end-of-file) is returned
    if (fclose(chemicalFile))
    {
        PRINT_ERROR("In checkCompatibilityOfChemicalData: the ");
        fprintf(stderr,"%s file has not been closed properly.\n",fileLocation);
        chemicalFile=NULL;
        return 0;
    }
    fprintf(stdout,"Closing file.\n");
    chemicalFile=NULL;

    return 1;
}

/* ************************************************************************** */
// The function readCubeFileAndAllocateMesh reads the *.cube file at the
// location given by the name_mesh variable of the structure pointed by
// pParameters (warning here: the file must exist), checks that it has the file
// has the *.cube format syntax, update the discretization values in the
// structure pointed by pParameters, then dynamically allocates some memory for
// the structure pointed by pMesh, building and storing the mesh data there.
// It has the Parameter* and Mesh* variables (both defined in main.h) as input
// arguments and returns zero if an error occurs, otherwise one on success
/* ************************************************************************** */
int readCubeFileAndAllocateMesh(Parameters* pParameters, Mesh* pMesh)
{
    size_t length=0;
    int readIntegerIn=0, readIntegerOut=0, readChar=0, *pLabel=NULL, i=0;
    int iMax=0, nNucl=0, nX=0, nY=0, nZ=0;
    double readDouble=0.;
    Nucleus *pNucl=NULL;
    FILE *cubeFile=NULL;

    // Check if the input variables are not pointing to NULL
    if (pParameters==NULL || pMesh==NULL)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: one of the input ");
        fprintf(stderr,"variables (pParameters=%p, ",(void*)pParameters);
        fprintf(stderr,"pMesh=%p) does not have a valid ",(void*)pMesh);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Set the structure to zero for variables and NULL for pointers
    initializeMeshStructure(pMesh);

    // Check the *.cube file exists with the *.cube extension
    if (getMeshFormat(pParameters->name_mesh,pParameters->name_length)!=-1)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: getMeshFormat function ");
        fprintf(stderr,"did not return minus one, which was the expected ");
        fprintf(stderr,"value here.\n");
        return 0;
    }

    // Opening *.cube file (warning: reset and overwrite file if already exists)
    // fopen returns a FILE pointer on success, otherwise NULL is returned
    fprintf(stdout,"\nOpening %s file. ",pParameters->name_mesh);
    cubeFile=fopen(pParameters->name_mesh,"r");
    if (cubeFile==NULL)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: we were not able to ");
        fprintf(stderr,"read the %s file.\n",pParameters->name_mesh);
        return 0;
    }
    fprintf(stdout,"Reading cube data. ");

    // Skip the first 2 lines of the *.cube file: fgetc returns the character
    // read as unsigned char cast to an int or EOF on the end of file or error
    do {
        readChar=fgetc(cubeFile);
    } while (readChar!='\n' && readChar!=EOF);
    if (readChar==EOF)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh problem ");
        fprintf(stderr,"encountered with the (standard) fgetc c-function or ");
        fprintf(stderr,"end-of-file reached without any data read in ");
        fprintf(stderr,"%s file.\n",pParameters->name_mesh);
        closeTheFile(&cubeFile);
        return 0;
    }

    do {
        readChar=fgetc(cubeFile);
    } while (readChar!='\n' && readChar!=EOF);
    if (readChar==EOF)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: problem ");
        fprintf(stderr,"encountered with the (standard) fgetc c-function or ");
        fprintf(stderr,"end-of-file reached without any data read in ");
        fprintf(stderr,"%s file.\n",pParameters->name_mesh);
        closeTheFile(&cubeFile);
        return 0;
    }

    // Read the number of nuclei and store it in the nNucl variable
    // fscanf function returns the number of input items successfully matched
    // and assigned, which can be fewer than provided for. Important remark:
    //  white space characters, such as blanks and newline characters, causes
    // fscanf (scanf and sscanf) to read, but not to store, all consecutive
    // white space characters in the input up to the next character that is not
    // white space. One white space character in format-string matches any
    // combination of white space characters in the input.
    readIntegerOut=fscanf(cubeFile," %d ",&readIntegerIn);
    if (readIntegerOut!=1)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: wrong return ");
        fprintf(stderr,"(=%d) of the standard fscanf ",readIntegerOut);
        fprintf(stderr,"c-function in the attempt of reading the total ");
        fprintf(stderr,"number of nuclei.\n");
        closeTheFile(&cubeFile);
        return 0;
    }

    if (readIntegerIn<1)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: expecting a positive ");
        fprintf(stderr,"number of nuclei instead of %d for the ",readIntegerIn);
        fprintf(stderr,"total number of nuclei saved in the ");
        fprintf(stderr,"%s file.\n",pParameters->name_mesh);
        closeTheFile(&cubeFile);
        return 0;
    }
    nNucl=readIntegerIn;

    // Read the minimal vertex value in the first-coordinate direction
    readIntegerOut=fscanf(cubeFile,"%lf ",&readDouble);
    if (readIntegerOut!=1)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: wrong return ");
        fprintf(stderr,"(=%d) of the standard fscanf ",readIntegerOut);
        fprintf(stderr,"c-function in the attempt of reading the minimal ");
        fprintf(stderr,"vertex value in the first-coordinate direction.\n");
        closeTheFile(&cubeFile);
        return 0;
    }
    pParameters->x_min=readDouble;

    // Read the minimal vertex value in the second-coordinate direction
    readIntegerOut=fscanf(cubeFile,"%lf ",&readDouble);
    if (readIntegerOut!=1)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: wrong return ");
        fprintf(stderr,"(=%d) of the standard fscanf ",readIntegerOut);
        fprintf(stderr,"c-function in the attempt of reading the minimal ");
        fprintf(stderr,"vertex value in the second-coordinate direction.\n");
        closeTheFile(&cubeFile);
        return 0;
    }
    pParameters->y_min=readDouble;

    // Read the minimal vertex value in the third-coordinate direction
    readIntegerOut=fscanf(cubeFile,"%lf ",&readDouble);
    if (readIntegerOut!=1)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: wrong return ");
        fprintf(stderr,"(=%d) of the standard fscanf ",readIntegerOut);
        fprintf(stderr,"c-function in the attempt of reading the minimal ");
        fprintf(stderr,"vertex value in the third-coordinate direction.\n");
        closeTheFile(&cubeFile);
        return 0;
    }
    pParameters->z_min=readDouble;

    // Read number of discretization points in the first-coordinate direction
    readIntegerOut=fscanf(cubeFile,"%d ",&readIntegerIn);
    if (readIntegerOut!=1)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: wrong return ");
        fprintf(stderr,"(=%d) of the standard fscanf ",readIntegerOut);
        fprintf(stderr,"c-function in the attempt of reading the ");
        fprintf(stderr,"number of discretization points in the ");
        fprintf(stderr,"first-coordinate direction.\n");
        closeTheFile(&cubeFile);
        return 0;
    }

    if (readIntegerIn<2)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: expecting an integer ");
        fprintf(stderr,"(strictly) greater than two instead of ");
        fprintf(stderr,"%d (in order to have at least one ",readIntegerIn+1);
        fprintf(stderr,"normal/tangent vector on each face of the cube ");
        fprintf(stderr,"associated with the default initial computational ");
        fprintf(stderr,"box) in the attempt of reading the number of ");
        fprintf(stderr,"discretization points in the first-coordinate ");
        fprintf(stderr,"direction.\n");
        closeTheFile(&cubeFile);
        return 0;
    }
    nX=readIntegerIn;

    // Read the discretization step value in the first-coordinate direction
    readIntegerOut=fscanf(cubeFile,"%lf ",&readDouble);
    if (readDouble<=0.)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: expecting a positive ");
        fprintf(stderr,"value instead of %lf in the attempt of ",readDouble);
        fprintf(stderr,"reading the discretization step in the ");
        fprintf(stderr,"first-coordinate direction.\n");
        closeTheFile(&cubeFile);
        return 0;
    }
    pParameters->delta_x=readDouble;

    readIntegerOut+=fscanf(cubeFile,"%lf ",&readDouble);
    if (readDouble!=0.)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: only orthogonal ");
        fprintf(stderr,"basis are supported for the mesh discretization.\n");
        closeTheFile(&cubeFile);
        return 0;
    }

    readIntegerOut+=fscanf(cubeFile,"%lf ",&readDouble);
    if (readDouble!=0.)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: only orthogonal ");
        fprintf(stderr,"basis are supported for the mesh discretization.\n");
        closeTheFile(&cubeFile);
        return 0;
    }

    if (readIntegerOut!=3)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: wrong return ");
        fprintf(stderr,"(%d instead of three) of the standard ",readIntegerOut);
        fprintf(stderr,"fscanf c-function in the attempt of reading the ");
        fprintf(stderr,"discretization step in the first-coordinate ");
        fprintf(stderr,"direction.\n");
        closeTheFile(&cubeFile);
        return 0;
    }

    // Read number of discretization points in the second-coordinate direction
    readIntegerOut=fscanf(cubeFile,"%d ",&readIntegerIn);
    if (readIntegerOut!=1)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: wrong return ");
        fprintf(stderr,"(=%d) of the standard fscanf ",readIntegerOut);
        fprintf(stderr,"c-function in the attempt of reading the ");
        fprintf(stderr,"number of discretization points in the ");
        fprintf(stderr,"second-coordinate direction.\n");
        closeTheFile(&cubeFile);
        return 0;
    }

    if (readIntegerIn<2)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: expecting an integer ");
        fprintf(stderr,"(strictly) greater than two instead of ");
        fprintf(stderr,"%d (in order to have at least one ",readIntegerIn+1);
        fprintf(stderr,"normal/tangent vector on each face of the cube ");
        fprintf(stderr,"associated with the default initial computational ");
        fprintf(stderr,"box) in the attempt of reading the number of ");
        fprintf(stderr,"discretization points in the second-coordinate ");
        fprintf(stderr,"direction.\n");
        closeTheFile(&cubeFile);
        return 0;
    }
    nY=readIntegerIn;

    // Read the discretization step value in the second-coordinate direction
    readIntegerOut=fscanf(cubeFile,"%lf ",&readDouble);
    if (readDouble!=0.)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: only orthogonal ");
        fprintf(stderr,"basis are supported for the mesh discretization.\n");
        closeTheFile(&cubeFile);
        return 0;
    }

    readIntegerOut+=fscanf(cubeFile,"%lf ",&readDouble);
    if (readDouble<=0.)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: expecting a positive ");
        fprintf(stderr,"value instead of %lf in the attempt of ",readDouble);
        fprintf(stderr,"reading the discretization step in the ");
        fprintf(stderr,"second-coordinate direction.\n");
        closeTheFile(&cubeFile);
        return 0;
    }
    pParameters->delta_y=readDouble;

    readIntegerOut+=fscanf(cubeFile,"%lf ",&readDouble);
    if (readDouble!=0.)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: only orthogonal ");
        fprintf(stderr,"basis are supported for the mesh discretization.\n");
        closeTheFile(&cubeFile);
        return 0;
    }

    if (readIntegerOut!=3)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: wrong return ");
        fprintf(stderr,"(%d instead of three) of the standard ",readIntegerOut);
        fprintf(stderr,"fscanf c-function in the attempt of reading the ");
        fprintf(stderr,"discretization step in the second-coordinate ");
        fprintf(stderr,"direction.\n");
        closeTheFile(&cubeFile);
        return 0;
    }

    // Read number of discretization points in the third-coordinate direction
    readIntegerOut=fscanf(cubeFile,"%d ",&readIntegerIn);
    if (readIntegerOut!=1)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: wrong return ");
        fprintf(stderr,"(=%d) of the standard fscanf ",readIntegerOut);
        fprintf(stderr,"c-function in the attempt of reading the ");
        fprintf(stderr,"number of discretization points in the ");
        fprintf(stderr,"third-coordinate direction.\n");
        closeTheFile(&cubeFile);
        return 0;
    }

    if (readIntegerIn<2)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: expecting an integer ");
        fprintf(stderr,"(strictly) greater than two instead of ");
        fprintf(stderr,"%d (in order to have at least one ",readIntegerIn+1);
        fprintf(stderr,"normal/tangent vector on each face of the cube ");
        fprintf(stderr,"associated with the default initial computational ");
        fprintf(stderr,"box) in the attempt of reading the number of ");
        fprintf(stderr,"discretization points in the third-coordinate ");
        fprintf(stderr,"direction.\n");
        closeTheFile(&cubeFile);
        return 0;
    }
    nZ=readIntegerIn;

    // Read the discretization step value in the third-coordinate direction
    readIntegerOut=fscanf(cubeFile,"%lf ",&readDouble);
    if (readDouble!=0.)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: only orthogonal ");
        fprintf(stderr,"basis are supported for the mesh discretization.\n");
        closeTheFile(&cubeFile);
        return 0;
    }

    readIntegerOut+=fscanf(cubeFile,"%lf ",&readDouble);
    if (readDouble!=0.)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: only orthogonal ");
        fprintf(stderr,"basis are supported for the mesh discretization.\n");
        closeTheFile(&cubeFile);
        return 0;
    }

    readIntegerOut+=fscanf(cubeFile,"%lf ",&readDouble);
    if (readDouble<=0.)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: expecting a positive ");
        fprintf(stderr,"value instead of %lf in the attempt of ",readDouble);
        fprintf(stderr,"reading the discretization step in the ");
        fprintf(stderr,"third-coordinate direction.\n");
        closeTheFile(&cubeFile);
        return 0;
    }
    pParameters->delta_z=readDouble;

    if (readIntegerOut!=3)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: wrong return ");
        fprintf(stderr,"(%d instead of three) of the standard ",readIntegerOut);
        fprintf(stderr,"fscanf c-function in the attempt of reading the ");
        fprintf(stderr,"discretization step in the third-coordinate ");
        fprintf(stderr,"direction.\n");
        closeTheFile(&cubeFile);
        return 0;
    }

    // Update the computational domain (the *.cube file evaluates the points at
    // the center of the hexahedra whereas the *.mesh data stores their vertices
    pParameters->x_min-=.5*pParameters->delta_x;
    pParameters->y_min-=.5*pParameters->delta_y;
    pParameters->z_min-=.5*pParameters->delta_z;

    pParameters->n_x=nX+1;
    pParameters->n_y=nY+1;
    pParameters->n_z=nZ+1;

    pParameters->x_max=pParameters->x_min+nX*pParameters->delta_x;
    pParameters->y_max=pParameters->y_min+nY*pParameters->delta_y;
    pParameters->z_max=pParameters->z_min+nZ*pParameters->delta_z;

    // Dynamically allocate memory for the local pNucl variable
    length=nNucl;
    pNucl=(Nucleus*)calloc(length,sizeof(Nucleus));
    if (pNucl==NULL)
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: could not allocate ");
        fprintf(stderr,"memory for the local (Nucleus*) pNucl variable.\n");
        closeTheFile(&cubeFile);
        return 0;
    }

    iMax=nNucl;
    for (i=0; i<iMax; i++)
    {
        // Read the charge number of each nuclei as an integer value
        readIntegerOut=fscanf(cubeFile,"%d ",&readIntegerIn);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readCubeFileAndAllocateMesh: wrong return ");
            fprintf(stderr,"(=%d) of the standard fscanf ",readIntegerOut);
            fprintf(stderr,"c-function in the attempt of reading the ");
            fprintf(stderr,"charge number associated with the %d-th ",i+1);
            fprintf(stderr,"nuclei.\n");
            free(pNucl); // free function does not return any value
            pNucl=NULL;
            closeTheFile(&cubeFile);
            return 0;
        }

        if (readIntegerIn<1 || readIntegerIn>100)
        {
            PRINT_ERROR("In readCubeFileAndAllocateMesh: the charge number ");
            fprintf(stderr,"(=%d) associated with the ",readIntegerIn);
            fprintf(stderr,"%d-th Nucleus structure should be a positive ",i+1);
            fprintf(stderr,"integer not (strictly) greater than one ");
            fprintf(stderr,"hundred.\n");
            free(pNucl);
            pNucl=NULL;
            closeTheFile(&cubeFile);
            return 0;
        }
        pNucl[i].charge=readIntegerIn;

        // Read the charge number of each nuclei as a double value
        readIntegerOut=fscanf(cubeFile,"%lf ",&readDouble);
        if (readIntegerOut!=1)
        {
            PRINT_ERROR("In readCubeFileAndAllocateMesh: wrong return ");
            fprintf(stderr,"(=%d) of the standard fscanf ",readIntegerOut);
            fprintf(stderr,"c-function in the attempt of reading the ");
            fprintf(stderr,"charge number associated with the %d-th ",i+1);
            fprintf(stderr,"nuclei.\n");
            free(pNucl);
            pNucl=NULL;
            closeTheFile(&cubeFile);
            return 0;
        }

        if (readDouble!=(double)pNucl[i].charge)
        {
            PRINT_ERROR("In readCubeFileAndAllocateMesh: the charge number ");
            fprintf(stderr,"associated with the %d-th Nucleus structure ",i+1);
            fprintf(stderr,"read a double value (=%lf) should ",readDouble);
            fprintf(stderr,"correspond to its integer value version ");
            fprintf(stderr,"(=%d).\n",pNucl[i].charge);
            free(pNucl);
            pNucl=NULL;
            closeTheFile(&cubeFile);
            return 0;
        }

        // Read the three coordinates of each nuclei
        readIntegerOut=fscanf(cubeFile,"%lf ",&readDouble);
        pNucl[i].x=readDouble;

        readIntegerOut+=fscanf(cubeFile,"%lf ",&readDouble);
        pNucl[i].y=readDouble;

        readIntegerOut+=fscanf(cubeFile,"%lf ",&readDouble);
        pNucl[i].z=readDouble;

        if (readIntegerOut!=3)
        {
            PRINT_ERROR("In readCubeFileAndAllocateMesh: wrong return ");
            fprintf(stderr,"(%d instead of three) of the ",readIntegerOut);
            fprintf(stderr,"standard fscanfc-function in the attempt of ");
            fprintf(stderr,"reading the three coordinates associated with ");
            fprintf(stderr,"the %d-th nuclei.\n",i+1);
            free(pNucl);
            pNucl=NULL;
            closeTheFile(&cubeFile);
            return 0;
        }
    }

    // Distinguish the case where no label are saved after the Nucleus data
    readIntegerOut=fscanf(cubeFile,"%lf ",&readDouble);
    if (readIntegerOut!=1)
    {
        if (pParameters->verbose)
        {
            fprintf(stdout,"\nWarning in readCubeFileAndAllocateMesh ");
            fprintf(stdout,"function: no Hexahedron label has been saved ");
            fprintf(stdout,"in the %s file. By ",pParameters->name_mesh);
            fprintf(stdout,"default, the labels of the mesh that is going to ");
            fprintf(stdout,"by build will be set to zero.\n");
        }
    }
    else
    {
        // Allocate memory for the pLabel variable
        length=(pParameters->n_x-1)*(pParameters->n_y-1)*(pParameters->n_z-1);
        pLabel=(int*)calloc(length,sizeof(int));
        if (pLabel==NULL)
        {
            PRINT_ERROR("In readCubeFileAndAllocateMesh: could not allocate ");
            fprintf(stderr,"memory for the local (int*) pLabel variable that ");
            fprintf(stderr,"is intended to store the label associated with ");
            fprintf(stderr,"the mesh elements.\n");
            free(pNucl);
            pNucl=NULL;
            closeTheFile(&cubeFile);
            return 0;
        }
        pLabel[0]=(int)readDouble;
        iMax=length;
        for (i=1; i<iMax; i++)
        {
            readIntegerOut=fscanf(cubeFile,"%lf ",&readDouble);
            if (readIntegerOut!=1)
            {
                PRINT_ERROR("In readCubeFileAndAllocateMesh: wrong return ");
                fprintf(stderr,"(=%d) of the standard fscanf ",readIntegerOut);
                fprintf(stderr,"c-function in the attempt of reading the ");
                fprintf(stderr,"the %d-th label associated with the mesh ",i+1);
                fprintf(stderr,"elements.\n");
                free(pNucl);
                pNucl=NULL;
                free(pLabel);
                pLabel=NULL;
                closeTheFile(&cubeFile);
                return 0;
            }
            pLabel[i]=(int)readDouble;
        }
    }

    // Closing the *.cube file: fclose function returns zero if the input FILE*
    // variable is successfully closed, otherwise EOF (end-of-file) is returned
    if (fclose(cubeFile))
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: the ");
        fprintf(stderr,"%s file has not been closed ",pParameters->name_mesh);
        fprintf(stderr,"properly.\n");
        free(pNucl);
        pNucl=NULL;
        free(pLabel);
        pLabel=NULL;
        cubeFile=NULL;
        return 0;
    }
    cubeFile=NULL;
    fprintf(stdout,"Closing file.\n");

    // Check if the Nucleus data are compatible with the one of the *.chem file
    if (!checkCompatibilityOfChemicalData(pParameters->name_chem,
                                          pParameters->name_length,pNucl,nNucl))
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: ");
        fprintf(stderr,"checkCompatibilityOfChemicalData function returned ");
        fprintf(stderr,"zero instead of one. The Nucleus data stored in the ");
        fprintf(stderr,"%s file do not correspond to ",pParameters->name_mesh);
        fprintf(stderr,"the one written in %s file.\n",pParameters->name_chem);
        free(pNucl);
        pNucl=NULL;
        free(pLabel);
        pLabel=NULL;
        return 0;
    }
    free(pNucl);
    pNucl=NULL;

    // Computing the number of elements thanks to the discretization parameters
    nX=pParameters->n_x;
    nY=pParameters->n_y;
    nZ=pParameters->n_z;
    if (pParameters->opt_mode>0)
    {
        pMesh->nver=nX*nY*nZ;
        pMesh->ncor=8;
        pMesh->nnorm=2*((nX-2)*(nY-2)+(nX-2)*(nZ-2)+(nY-2)*(nZ-2));
        pMesh->ntan=4*((nX-2)+(nY-2)+(nZ-2));
        pMesh->nedg=4*((nX-1)+(nY-1)+(nZ-1));
        pMesh->ntri=4*((nX-1)*(nY-1)+(nX-1)*(nZ-1)+(nY-1)*(nZ-1));
        pMesh->ntet=6*(nX-1)*(nY-1)*(nZ-1);
    }
    else
    {
        pMesh->nver=nX*nY*nZ;
        pMesh->nqua=2*((nX-1)*(nY-1)+(nX-1)*(nZ-1)+(nY-1)*(nZ-1));
        pMesh->nhex=(nX-1)*(nY-1)*(nZ-1);
    }

    // Dynamically allocating memory for the Mesh structure
    if (!allocateInitialMeshMemory(pParameters,pMesh))
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: ");
        fprintf(stderr,"allocateInitialMeshMemory function returned zero ");
        fprintf(stderr,"instead of one.\n");
        free(pLabel);
        pLabel=NULL;
        return 0;
    }

    // Meshing the cube according to the discretization parameters
    if (!meshCube(pParameters,pMesh))
    {
        PRINT_ERROR("In readCubeFileAndAllocateMesh: meshCube function ");
        fprintf(stderr,"returned zero instead of one.\n");
        free(pLabel);
        pLabel=NULL;
        return 0;
    }

    // Updating the labels of the mesh elements by the ones stored in pLabel
    if (pLabel!=NULL)
    {
        if (pParameters->opt_mode>0)
        {
            iMax=pMesh->ntet;
            for (i=0; i<iMax; i++)
            {
                if (!pLabel[i/6])
                {
                    pMesh->ptet[i].label=2;
                }
                else
                {
                    pMesh->ptet[i].label=3;
                }
            }
        }
        else
        {
            iMax=pMesh->nhex;
            for (i=0; i<iMax; i++)
            {
                if (!(pLabel[i]))
                {
                    pMesh->phex[i].label=2;
                }
                else
                {
                    pMesh->phex[i].label=3;
                }
            }
        }
    }

    // free the memory allocated for pLabel
    free(pLabel);
    pLabel=NULL;

    fprintf(stdout,"\nMesh data successfully loaded:\n");
    if (pParameters->opt_mode>0)
    {
        fprintf(stdout,"%d vertices, %d tetrahedra ",pMesh->nver,pMesh->ntet);
        fprintf(stdout,"and %d boundary triangles\n",pMesh->ntri);
        fprintf(stdout,"(%d corners, %d edges, ",pMesh->ncor,pMesh->nedg);
        fprintf(stdout,"%d tangent and %d normal ",pMesh->ntan,pMesh->nnorm);
        fprintf(stdout,"vectors).\n");
    }
    else
    {
        fprintf(stdout,"%d vertices, %d hexahedra ",pMesh->nver,pMesh->nhex);
        fprintf(stdout,"and %d boundary quadrilaterals.\n",pMesh->nqua);
    }

    return 1;
}

/* ************************************************************************** */
// The function loadMesh looks at the pParameters->name_mesh variable. If it is
// set to NULL, depending on the pParamters->opt_mode (<=0/>0), we generate a
// default (hexahedral/tetrahedral) mesh which is stored in the structure
// pointed by pMesh and saved in a *.mesh and *.cube file. Otherwise, the
// function loadMesh simply loads the mesh data from the *.mesh/ *.cube file
// pointed by the pParameters->name_mesh variable and update some variables of
// the structure pointed by pParameters if necessary. It has the Parameters*
// and Mesh* variable (both defined in main.h) as input arguments and it returns
// one on success, minus one in the case where the user refuses to approve the
// suggested mesh discretization of the default initial computational box.
// Otherwise zero is returned if an error is encountered during the process
/* ************************************************************************** */
int loadMesh(Parameters* pParameters, Mesh* pMesh)
{
    size_t lengthName=0;
    char* fileLocation=NULL;
    int readChar=0, optMode=0;

    // Check if the input pParameters or pMesh variable is pointing to NULL
    if (pParameters==NULL || pMesh==NULL)
    {
        PRINT_ERROR("In loadMesh: at least one of the input pParameters ");
        fprintf(stderr,"%p or pParameters ",(void*)pParameters);
        fprintf(stderr,"%p variable does not have a ",(void*)pMesh);
        fprintf(stderr,"valid address.\n");
        return 0;
    }

    // This trick is used not to print the content of mmg3d output
    // (see adaptMeshWithMmg3dSoftware function in main.c file)
    if (pParameters->opt_mode==1)
    {
        optMode=1;
        pParameters->opt_mode=3;
    }

    switch (initialFileExists(pParameters->name_mesh,pParameters->name_length))
    {
        case -1:
            // Evaluate the number of vertices, elements, and boundary elements
            switch (initializeCubeDiscretization(pParameters,pMesh))
            {
                case 1:
                    break;

                // The user did not approve the suggested discretization
                case -1:
                    return -1;
                    break;

                default:
                    PRINT_ERROR("In loadMesh: initializeCubeDiscretization ");
                    fprintf(stderr,"function returned zero instead of ");
                    fprintf(stderr,"(+/-)one.\n");
                    return 0;
                    break;
            }

            // Dynamically allocating memory for the Mesh structure
            if (!allocateInitialMeshMemory(pParameters,pMesh))
            {
                PRINT_ERROR("In loadMesh: allocateInitialMeshMemory function ");
                fprintf(stderr,"returned zero instead of one.\n");
                return 0;
            }

            // Meshing the cube according to the discretization parameters
            if (!meshCube(pParameters,pMesh))
            {
                PRINT_ERROR("In loadMesh: meshCube function returned zero ");
                fprintf(stderr,"instead of one.\n");
                return 0;
            }

            // Saving the cube discretization in a *.mesh file
            if (!writingMeshFile(pParameters,pMesh))
            {
                PRINT_ERROR("In loadMesh: writingMeshFile function returned ");
                fprintf(stderr,"zero instead of one.\n");
                return 0;
            }

            // Allocate memory for *.sol file and remove it if it already exists
            // calloc returns a pointer to the allocated memory, or NULL
            lengthName=pParameters->name_length;
            fileLocation=(char*)calloc(lengthName,sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In loadMesh: could not allocate memory for the ");
                fprintf(stderr,"(local) char* fileLocation variable.\n");
                return 0;
            }
            // strncpy returns a pointer to the string (not used here)
            // strlen function returns the length of the string (not with '\0')
            strncpy(fileLocation,pParameters->name_mesh,lengthName);
            lengthName=strlen(fileLocation);
            fileLocation[lengthName-5]='.';
            fileLocation[lengthName-4]='s';
            fileLocation[lengthName-3]='o';
            fileLocation[lengthName-2]='l';
            fileLocation[lengthName-1]='\0';
            if (initialFileExists(fileLocation,pParameters->name_length)==1)
            {
                // remove returns 0 on success, otherwise -1
                // free function does not return any value
                if (remove(fileLocation))
                {
                    PRINT_ERROR("In loadMesh: wrong return (=-1) of the ");
                    fprintf(stderr,"standard remove c-function in the ");
                    fprintf(stderr,"attempt of removing the %s ",fileLocation);
                    fprintf(stderr,"file.\n");
                    free(fileLocation);
                    fileLocation=NULL;
                    return 0;
                }
            }

            // Make a preliminary isotropic adaptation for tetrahedral meshes
            if (pParameters->opt_mode>0)
            {
                fprintf(stdout,"Cleaning the allocated memory for a ");
                fprintf(stdout,"preliminary isotropic mesh adaptation.");
                freeMeshMemory(pMesh);

                if (!adaptMeshWithMmg3dSoftware(pParameters,"iso"))
                {
                    PRINT_ERROR("In loadMesh: adaptMeshWithMmg3dSoftware ");
                    fprintf(stderr,"function returned zero instead of one.\n");
                    free(fileLocation);
                    fileLocation=NULL;
                    return 0;
                }

                // Remove the *.sol file associated with the *.mesh file
                if (remove(fileLocation))
                {
                    PRINT_ERROR("In loadMesh: wrong return (=-1) of the ");
                    fprintf(stderr,"standard remove c-function in the ");
                    fprintf(stderr,"attempt of removing the %s ",fileLocation);
                    fprintf(stderr,"file.\n");
                    free(fileLocation);
                    fileLocation=NULL;
                    return 0;
                }

                // Read the new *.mesh file
                if (!readMeshFileAndAllocateMesh(pParameters,pMesh))
                {
                    PRINT_ERROR("In loadMesh: readMeshFileAndAllocateMesh ");
                    fprintf(stderr,"function returned zero instead of one.\n");
                    free(fileLocation);
                    fileLocation=NULL;
                    return 0;
                }
            }

            // Free the memory allocated for fileLocation
            free(fileLocation);
            fileLocation=NULL;
            break;

        case 1:
            // If the file has the *.cube format, we aim to create an *.mesh
            // file whereas if the file already has the *.mesh format, we want
            // to copy it in order not to alter the original one. For this
            // purpose, we will use the default path name given by the
            // pParameters->name_info variable, replacing the *.info extension
            // by the *.mesh one. If such a default name and the original one
            // are identical, a manual confirmation of the user is needed to
            // to potentially overwrite the *.mesh file later in the algorithm


            // Hence, we start by checking the pParameters->name_info variable
            if (!checkStringFromLength(pParameters->name_info,7,
                                                      pParameters->name_length))
            {
                PRINT_ERROR("In loadMesh: checkStringFromLength function ");
                fprintf(stderr,"returned zero, which is not the expected ");
                fprintf(stderr,"value here, after having checked that the ");
                fprintf(stderr,"char* name_info variable of the structure ");
                fprintf(stderr,"pointed by pParameters is not a string of ");
                fprintf(stderr,"length (strictly) less than ");
                fprintf(stderr,"%d (and more than 5 ",pParameters->name_length);
                fprintf(stderr,"in order to store at least something more ");
                fprintf(stderr,"than the *.info extension).\n");
                return 0;
            }

            // Check if the *.info name ends with the ".info" extension
            lengthName=strlen(pParameters->name_info);
            if (pParameters->name_info[lengthName-5]!='.' ||
                                    pParameters->name_info[lengthName-4]!='i' ||
                                    pParameters->name_info[lengthName-3]!='n' ||
                                    pParameters->name_info[lengthName-2]!='f' ||
                                    pParameters->name_info[lengthName-1]!='o' ||
                                       pParameters->name_info[lengthName]!='\0')
            {
                PRINT_ERROR("In loadMesh: ");
                fprintf(stderr,"%s file name does not ",pParameters->name_info);
                fprintf(stderr,"end with with the '.info' extension.\n");
                return 0;
            }

            // Allocate memory for the fileLocation variable
            lengthName=pParameters->name_length;
            fileLocation=(char*)calloc(lengthName,sizeof(char));
            if (fileLocation==NULL)
            {
                PRINT_ERROR("In loadMesh: could not allocate memory for the ");
                fprintf(stderr,"local char* fileLocation variable.\n");
                return 0;
            }

            strncpy(fileLocation,pParameters->name_info,lengthName);
            lengthName=strlen(fileLocation);
            fileLocation[lengthName-5]='.';
            fileLocation[lengthName-4]='m';
            fileLocation[lengthName-3]='e';
            fileLocation[lengthName-2]='s';
            fileLocation[lengthName-1]='h';
            fileLocation[lengthName]='\0';

            // Distinguigh the *.cube file from the *.mesh format
            switch (getMeshFormat(pParameters->name_mesh,
                                                      pParameters->name_length))
            {
                case 1:
                    fprintf(stdout,"\nMesh structure will be loaded from ");
                    fprintf(stdout,"%s file.",pParameters->name_mesh);

                    // Read the *.mesh file
                    if (!readMeshFileAndAllocateMesh(pParameters,pMesh))
                    {
                        PRINT_ERROR("In loadMesh: ");
                        fprintf(stderr,"readMeshFileAndAllocateMesh function ");
                        fprintf(stderr,"returned zero instead of one.\n");
                        free(fileLocation);
                        fileLocation=NULL;
                        return 0;
                    }

                    // Update the parameters related to the computational box
                    if (!updateDiscretizationParameters(pParameters,pMesh))
                    {
                        PRINT_ERROR("In loadMesh: ");
                        fprintf(stderr,"updateDiscretizationParameters ");
                        fprintf(stderr,"function returned zero instead of ");
                        fprintf(stderr,"one.\n");
                        free(fileLocation);
                        fileLocation=NULL;
                        return 0;
                    }

                    // strcmp function returns 0 if the two strings are equal,
                    // otherwise <0 (resp. >0) if the first string argument is
                    // shorter (resp. longer) than the second one
                    if (!strcmp(pParameters->name_mesh,fileLocation))
                    {
                        if (pParameters->verbose)
                        {
                            fprintf(stdout,"\nWarning in loadMesh function: ");
                            fprintf(stdout,"the prescribed *.mesh file ");
                            fprintf(stdout,"exists and has the same path ");
                            fprintf(stdout,"name (=%s) than the ",fileLocation);
                            fprintf(stdout,"default one (given by replacing ");
                            fprintf(stdout,"'.info' extension in the *.info ");
                            fprintf(stdout,"file name by the '.mesh' ");
                            fprintf(stdout,"extension.");
                        }

                        // If pParameters->save_print equals zero, then graphic
                        // and prompt mode is off
                        if (pParameters->save_print)
                        {
                            fprintf(stdout,"\nThe %s file may ",fileLocation);
                            fprintf(stdout,"be overwritten later. Do you ");
                            fprintf(stdout,"want to continue (y/n)? ");

                            // Manually confirm the potential overwritting:
                            // getcharreturns the character read as an unsigned
                            // char cast to an int or EOF in case of error
                            readChar=getchar();
                            switch (readChar)
                            {
                                case 'y':
                                    fprintf(stdout,"Ok, we can proceed ");
                                    fprintf(stdout,"further now.\n");
                                    break;

                                case 'n':
                                    fprintf(stdout,"Ok, leaving program.\n");
                                    free(fileLocation);
                                    fileLocation=NULL;
                                    return -1;
                                    break;

                                case EOF:
                                    PRINT_ERROR("In loadMesh: wrong return ");
                                    fprintf(stderr,"(=%d) of the ",EOF);
                                    fprintf(stderr,"standard getchar ");
                                    fprintf(stderr,"c-function in the ");
                                    fprintf(stderr,"attempt of reading your ");
                                    fprintf(stderr,"answer ('y'=yes or ");
                                    fprintf(stderr,"'n'=no).\n");
                                    free(fileLocation);
                                    fileLocation=NULL;
                                    return 0;
                                    break;

                                default:
                                    PRINT_ERROR("In loadMesh: expecting ");
                                    fprintf(stderr,"'y'(=yes) or 'n'(=no) ");
                                    fprintf(stderr,"instead of '%c' ",readChar);
                                    fprintf(stderr,"in your answer.\n");
                                    free(fileLocation);
                                    fileLocation=NULL;
                                    return 0;
                                    break;
                            }
                            // Clean the input buffer of the standard input
                            // stream (stdin) in order to avoid buffer overflow 
                            // later (warning here: be sure that it is not 
                            // empty otherwise it will wait undefinitively for
                            // a non-empty input)
                            while (readChar!='\n' && readChar!=EOF)
                            {
                                readChar=getchar();
                            }
                        }
                    }
                    else
                    {
                        if (initialFileExists(fileLocation,
                                                   pParameters->name_length)==1)
                        {
                            if (remove(fileLocation))
                            {
                                PRINT_ERROR("In loadMesh: wrong return (=-1) ");
                                fprintf(stderr,"of the standard remove ");
                                fprintf(stderr,"c-function in the attempt of ");
                                fprintf(stderr,"removing the ");
                                fprintf(stderr,"%s file.\n",fileLocation);
                                free(fileLocation);
                                fileLocation=NULL;
                                return 0;
                            }
                        }

                        if (!copyFileLocation(pParameters->name_mesh,
                                             pParameters->name_length,
                                                                  fileLocation))
                        {
                            PRINT_ERROR("In loadMesh: copyFileLocation ");
                            fprintf(stderr,"function returned zero instead ");
                            fprintf(stderr,"of one.\n");
                            free(fileLocation);
                            fileLocation=NULL;
                            return 0;
                        }

                        // Update the name of the (copy of the) *.mesh file
                        strncpy(pParameters->name_mesh,fileLocation,
                                                      pParameters->name_length);
                    }
                    break;

                case -1:
                    fprintf(stdout,"\nMesh structure will be loaded from ");
                    fprintf(stdout,"%s file.",pParameters->name_mesh);

                    // Read the *.cube file, generate mesh and save it in *.mesh
                    if (!readCubeFileAndAllocateMesh(pParameters,pMesh))
                    {
                        PRINT_ERROR("In loadMesh: ");
                        fprintf(stderr,"readCubeFileAndAllocateMesh function ");
                        fprintf(stderr,"returned zero instead of one.\n");
                        free(fileLocation);
                        fileLocation=NULL;
                        return 0;
                    }

                    // Update name of *.cube file into the default *.mesh one
                    strncpy(pParameters->name_mesh,fileLocation,
                                                      pParameters->name_length);

                    // Saving the cube discretization in a *.mesh file
                    if (!writingMeshFile(pParameters,pMesh))
                    {
                        PRINT_ERROR("In readCubeFileAndAllocateMesh: ");
                        fprintf(stderr,"writingMeshFile function returned ");
                        fprintf(stderr,"zero instead of one.\n");
                        free(fileLocation);
                        fileLocation=NULL;
                        return 0;
                    }
                    break;

                default:
                    PRINT_ERROR("In loadMesh: getMeshFormat function ");
                    fprintf(stderr,"returned zero instead of (+/-)one.\n");
                    free(fileLocation);
                    fileLocation=NULL;
                    return 0;
                    break;
            }

            // Free the memory allocated for fileLocation
            free(fileLocation);
            fileLocation=NULL;
            break;

        default:
            PRINT_ERROR("In loadMesh: initialFileExists function returned ");
            fprintf(stderr,"zero instead of (+/-)one.\n");
            return 0;
            break;
    }

    // This trick is used not to print the content of mmg3d output
    // (see adaptMeshWithMmg3dSoftware function in main.c file)
    if (pParameters->opt_mode==3 && optMode==1)
    {
        pParameters->opt_mode=1;
    }

    return 1;
}

/* ************************************************************************** */
// The function writingCubeFile combines the data stored in the structures
// pointed by pParameters, pChemicalSystem, and pMesh in order to save the mesh
// discretization in the *.cube format. If pParameters->name_mesh is set to NULL
// a default name is generated by changing the *.info extension given in
// pParameters->name_info into the *.cube one. If pParameters->name_mesh stores
// the name of a *.mesh file, the same name is used by changing the *.mesh
// extension into the *.cube one. Otherwise, the pParameters->name_mesh variable
// must store the name of a valid *.cube file which is used as the name of the
// file we aim to write (warning here: reset and overwrite the file if it
// already exists). It has the Parameters*, ChemicalSystem*, and Mesh* variables
// (both defined in main.h) as input arguments and it returns one on success
// otherwise zero is returned if an error is encountered in the process
/* ************************************************************************** */
int writingCubeFile(Parameters* pParameters, ChemicalSystem* pChemicalSystem,
                                                                    Mesh* pMesh)
{
    size_t lengthName=0;
    char *fileLocation=NULL;
    int i=0, iMax=0, boolean=0;
    double delta=0.;
    FILE *cubeFile=NULL;

    // Check if the input variables are not pointing to NULL
    if (pParameters==NULL || pChemicalSystem==NULL || pMesh==NULL)
    {
        PRINT_ERROR("In writingCubeFile: one of the input variables ");
        fprintf(stderr,"(pParameters=%p, ",(void*)pParameters);
        fprintf(stderr,"pChemicalSystem=%p, pMesh=",(void*)pChemicalSystem);
        fprintf(stderr,"%p) does not have a valid address.\n",(void*)pMesh);
        return 0;
    }

    // Check pParameters->name_length variable
    if (pParameters->name_length<7)
    {
        PRINT_ERROR("In writingCubeFile: the variable name_length ");
        fprintf(stderr,"(=%d) of the structure ",pParameters->name_length);
        fprintf(stderr,"pointed by pParameters must be an integer (strictly) ");
        fprintf(stderr,"greater than six (to store at least something more ");
        fprintf(stderr,"than the *.cube extension).\n");
        return 0;
    }

    // Dynamically allocate memory for the local fileLocation variable
    // calloc function returns a pointer to the allocated memory, otherwise NULL
    lengthName=pParameters->name_length;
    fileLocation=(char*)calloc(lengthName,sizeof(char));
    if (fileLocation==NULL)
    {
        PRINT_ERROR("In writingCubeFile: could not allocate memory for the ");
        fprintf(stderr,"char* fileLocation (local) variable.\n");
        return 0;
    }

    if (pParameters->name_mesh==NULL)
    {
        // Allocate memory for pParameters->name_mesh and check if it worked
        lengthName=pParameters->name_length;
        pParameters->name_mesh=(char*)calloc(lengthName,sizeof(char));
        if (pParameters->name_mesh==NULL)
        {
            PRINT_ERROR("In writingCubeFile: could not allocate memory for ");
            fprintf(stderr,"the char* pParameters->name_mesh variable.\n");
            free(fileLocation); // free function does not return any value
            fileLocation=NULL;
            return 0;
        }

        // Check pParameters->name_info variable (used to set the default name)
        if (!checkStringFromLength(pParameters->name_info,7,
                                                      pParameters->name_length))
        {
            PRINT_ERROR("In writingCubeFile: checkStringFromLength function ");
            fprintf(stderr,"returned zero, which is not the expected value ");
            fprintf(stderr,"here, after having checked that the char* ");
            fprintf(stderr,"name_info variable of the structure pointed by ");
            fprintf(stderr,"pParameters is not a string of length (strictly) ");
            fprintf(stderr,"less than %d (and more ",pParameters->name_length);
            fprintf(stderr,"than 5 in order to store at least something more ");
            fprintf(stderr,"than the *.info extension).\n");
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        // Check if the *.info file name ends with the ".info" extension
        // strlen function returns the length of the string (not including '\0')
        lengthName=strlen(pParameters->name_info);
        if (pParameters->name_info[lengthName-5]!='.' ||
                                    pParameters->name_info[lengthName-4]!='i' ||
                                    pParameters->name_info[lengthName-3]!='n' ||
                                    pParameters->name_info[lengthName-2]!='f' ||
                                    pParameters->name_info[lengthName-1]!='o' ||
                                       pParameters->name_info[lengthName]!='\0')
        {
            PRINT_ERROR("In writingCubeFile: ");
            fprintf(stderr,"%s file name does not ",pParameters->name_info);
            fprintf(stderr,"end with with the '.info' extension.\n");
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        // Copy the *.info name into pParameters->name_mesh and change for the
        // *.cube extension: strncpy returns a pointer to the string
        strncpy(pParameters->name_mesh,pParameters->name_info,
                                                      pParameters->name_length);
        pParameters->name_mesh[lengthName-5]='.';
        pParameters->name_mesh[lengthName-4]='c';
        pParameters->name_mesh[lengthName-3]='u';
        pParameters->name_mesh[lengthName-2]='b';
        pParameters->name_mesh[lengthName-1]='e';
        pParameters->name_mesh[lengthName]='\0';

        // Copy the default *.cube name into fileLocation
        strncpy(fileLocation,pParameters->name_mesh,pParameters->name_length);
    }
    else
    {
        // Check pParameters->name_mesh variable
        if (!checkStringFromLength(pParameters->name_mesh,7,
                                                      pParameters->name_length))
        {
            PRINT_ERROR("In writingCubeFile: checkStringFromLength function ");
            fprintf(stderr,"returned zero, which is not the expected value ");
            fprintf(stderr,"here, after having checked that the char* ");
            fprintf(stderr,"name_mesh variable of the structure pointed by ");
            fprintf(stderr,"pParameters is not a string of length (strictly) ");
            fprintf(stderr,"less than %d (and more ",pParameters->name_length);
            fprintf(stderr,"than 5 in order to store at least something more ");
            fprintf(stderr,"than the *.mesh or *.cube extension).\n");
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        // Check if the *.cube file name ends with the *.mesh extension or not
        lengthName=strlen(pParameters->name_mesh);
        if (pParameters->name_mesh[lengthName-5]=='.' &&
                                    pParameters->name_mesh[lengthName-4]=='m' &&
                                    pParameters->name_mesh[lengthName-3]=='e' &&
                                    pParameters->name_mesh[lengthName-2]=='s' &&
                                    pParameters->name_mesh[lengthName-1]=='h' &&
                                       pParameters->name_mesh[lengthName]=='\0')
        {
            strncpy(fileLocation,pParameters->name_mesh,
                                                      pParameters->name_length);
            fileLocation[lengthName-5]='.';
            fileLocation[lengthName-4]='c';
            fileLocation[lengthName-3]='u';
            fileLocation[lengthName-2]='b';
            fileLocation[lengthName-1]='e';
            fileLocation[lengthName]='\0';
        }
        else if (pParameters->name_mesh[lengthName-5]=='.' &&
                                    pParameters->name_mesh[lengthName-4]=='c' &&
                                    pParameters->name_mesh[lengthName-3]=='u' &&
                                    pParameters->name_mesh[lengthName-2]=='b' &&
                                    pParameters->name_mesh[lengthName-1]=='e' &&
                                       pParameters->name_mesh[lengthName]=='\0')
        {
            strncpy(fileLocation,pParameters->name_mesh,
                                                      pParameters->name_length);
        }
        else
        {
            PRINT_ERROR("In writingCubeFile: ");
            fprintf(stderr,"%s file name does not end ",pParameters->name_mesh);
            fprintf(stderr,"with with the '.mesh' or '.cube' extension.\n");
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }
    }

    // Opening *.cube file (warning: reset and overwrite file if already exists)
    // fopen returns a FILE pointer on success, otherwise NULL is returned
    fprintf(stdout,"\nOpening %s file. ",fileLocation);
    cubeFile=fopen(fileLocation,"w+");
    if (cubeFile==NULL)
    {
        PRINT_ERROR("In writingCubeFile: we were not able to open and write ");
        fprintf(stderr,"into the %s file",fileLocation);
        free(fileLocation);
        fileLocation=NULL;
        return 0;
    }
    fprintf(stdout,"Writing cube data. ");

    // Check the number of nuclei
    if (pChemicalSystem->nnucl<1)
    {
        PRINT_ERROR("In writingCubeFile: the number of nuclei stored in the ");
        fprintf(stderr,"structure pointed by pChemicalSystem should be ");
        fprintf(stderr,"positive instead of %d.\n",pChemicalSystem->nnucl);
        closeTheFile(&cubeFile);
        free(fileLocation);
        fileLocation=NULL;
        return 0;
    }

    // Check discretization in the first-coordinate direction
    boolean=(pParameters->n_x<3 || pParameters->x_min>=pParameters->x_max);
    if (pParameters->opt_mode<=0)
    {
        delta=(pParameters->x_max-pParameters->x_min)/
                                                   (double)(pParameters->n_x-1);
        boolean=(boolean || DEF_ABS(pParameters->delta_x-delta)>=1.e-16);
    }
    if (boolean)                    
    {
        PRINT_ERROR("In writingCubeFile: incorrect discretization of the ");
        fprintf(stderr,"cube in the first-coordinate direction: expecting\n");
        fprintf(stderr,"(pParameters->n_x=%d) > 2\n",pParameters->n_x);
        fprintf(stderr,"(pParameters->x_min=%lf) < ",pParameters->x_min);
        fprintf(stderr,"(pParameters->x_max=%lf)\n",pParameters->x_max);
        fprintf(stderr,"(pParameters->delta_x=%.18lf) ",pParameters->delta_x);
        fprintf(stderr,"== %.18lf (up to 15 decimals).\n",delta);
        closeTheFile(&cubeFile);
        free(fileLocation);
        fileLocation=NULL;
        return 0;
    }

    // Check discretization in the second-coordinate direction
    boolean=(pParameters->n_y<3 || pParameters->y_min>=pParameters->y_max);
    if (pParameters->opt_mode<=0)
    {
        delta=(pParameters->y_max-pParameters->y_min)/
                                                   (double)(pParameters->n_y-1);
        boolean=(boolean || DEF_ABS(pParameters->delta_y-delta)>=1.e-16);
    }
    if (boolean)
    {
        PRINT_ERROR("In writingCubeFile: incorrect discretization of the ");
        fprintf(stderr,"cube in the second-coordinate direction: expecting\n");
        fprintf(stderr,"(pParameters->n_y=%d) > 2\n",pParameters->n_y);
        fprintf(stderr,"(pParameters->y_min=%lf) < ",pParameters->y_min);
        fprintf(stderr,"(pParameters->y_max=%lf)\n",pParameters->y_max);
        fprintf(stderr,"(pParameters->delta_y=%.18lf) ",pParameters->delta_y);
        fprintf(stderr,"== %.18lf (up to 15 decimals).\n",delta);
        closeTheFile(&cubeFile);
        free(fileLocation);
        fileLocation=NULL;
        return 0;
    }

    // Check discretization in the third-coordinate direction
    boolean=(pParameters->n_z<3 || pParameters->z_min>=pParameters->z_max);
    if (pParameters->opt_mode<=0)
    {
        delta=(pParameters->z_max-pParameters->z_min)/
                                                   (double)(pParameters->n_z-1);
        boolean=(boolean || DEF_ABS(pParameters->delta_z-delta)>=1.e-16);
    }
    if (boolean)
    {
        PRINT_ERROR("In writingCubeFile: incorrect discretization of the ");
        fprintf(stderr,"cube in the third-coordinate direction: expecting\n");
        fprintf(stderr,"(pParameters->n_z=%d) > 2\n",pParameters->n_z);
        fprintf(stderr,"(pParameters->z_min=%lf) < ",pParameters->z_min);
        fprintf(stderr,"(pParameters->z_max=%lf)\n",pParameters->z_max);
        fprintf(stderr,"(pParameters->delta_z=%.18lf) ",pParameters->delta_z);
        fprintf(stderr,"== %.18lf (up to 15 decimals).\n",delta);
        closeTheFile(&cubeFile);
        free(fileLocation);
        fileLocation=NULL;
        return 0;
    }

    // Writing data using the *.cube syntax (careful here: in our algorithm, we
    // have considered the *.mesh format viewpoint where points are the vertices
    // of tetrahedra/hexahedra; whereas in the *.cube format the points are the
    // center of the corresponding hexahedra
    fprintf(cubeFile,"CubeFile\nNew grid\n    %d",pChemicalSystem->nnucl);
    fprintf(cubeFile,"    %.8le",pParameters->x_min+.5*pParameters->delta_x);
    fprintf(cubeFile,"    %.8le",pParameters->y_min+.5*pParameters->delta_y);
    fprintf(cubeFile,"    %.8le\n",pParameters->z_min+.5*pParameters->delta_z);
    fprintf(cubeFile,"    %d",pParameters->n_x-1);
    fprintf(cubeFile,"    %lf    %lf    %lf\n",pParameters->delta_x,0.,0.);
    fprintf(cubeFile,"    %d",pParameters->n_y-1);
    fprintf(cubeFile,"    %lf    %lf    %lf\n",0.,pParameters->delta_y,0.);
    fprintf(cubeFile,"    %d",pParameters->n_z-1);
    fprintf(cubeFile,"    %lf    %lf    %lf\n",0.,0.,pParameters->delta_z);

    // Writing the data stored in each Nucleus structure
    iMax=pChemicalSystem->nnucl;
    for (i=0; i<iMax; i++)
    {
        // Check and write the charge number associated with each Nucleus
        if (pChemicalSystem->pnucl[i].charge<1 ||
                                           pChemicalSystem->pnucl[i].charge>100)
        {
            PRINT_ERROR("In writingCubeFile: the charge number ");
            fprintf(stderr,"(=%d) ",pChemicalSystem->pnucl[i].charge);
            fprintf(stderr,"associated with the %d Nucleus of the ",i+1);
            fprintf(stderr,"structure pointed by pChemicalSystem should be a ");
            fprintf(stderr,"positive integer that cannot be (strictly) ");
            fprintf(stderr,"greater than one hundred.\n");
            closeTheFile(&cubeFile);
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }
        fprintf(cubeFile,"    %d",pChemicalSystem->pnucl[i].charge);
        fprintf(cubeFile,"    %lf",(double)pChemicalSystem->pnucl[i].charge);
        fprintf(cubeFile,"    %.8le",pChemicalSystem->pnucl[i].x);
        fprintf(cubeFile,"    %.8le",pChemicalSystem->pnucl[i].y);
        fprintf(cubeFile,"    %.8le\n",pChemicalSystem->pnucl[i].z);
    }

    // Volumetric data initialized to zero
    iMax=(pParameters->n_x-1)*(pParameters->n_y-1)*(pParameters->n_z-1);
    if (pMesh->nhex!=0)
    {
        if (pMesh->phex==NULL)
        {
            PRINT_ERROR("In writingCubeFile: the (Hexahedron*) phex variable ");
            fprintf(stderr,"of the structure pointed by pMesh is pointing to ");
            fprintf(stderr,"the %p adress.\n",(void*)pMesh->phex);
            closeTheFile(&cubeFile);
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }

        if (pParameters->opt_mode>0)
        {
            PRINT_ERROR("In writingCubeFile: although the opt_mode ");
            fprintf(stderr,"(=%d) variable of the ",pParameters->opt_mode);
            fprintf(stderr,"structure pointed by pParameters is positive, ");
            fprintf(stderr,"thus imposing that pMesh is storing a ");
            fprintf(stderr,"tetrahedral mesh, a positive number of ");
            fprintf(stderr,"hexahedra (=%d) is stored in the ",pMesh->nhex);
            fprintf(stderr,"structure pointed by pMesh, which should not ");
            fprintf(stderr,"the case in such a situation.\n");
            closeTheFile(&cubeFile);
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }
        if (pMesh->nhex!=iMax)
        {
            PRINT_ERROR("In writingCubeFile: we were expecting ");
            fprintf(stderr,"%d hexahedra instead of %d in ",iMax,pMesh->nhex);
            fprintf(stderr,"the structure pointed by pMesh.\n");
            closeTheFile(&cubeFile);
            free(fileLocation);
            fileLocation=NULL;
            return 0;
        }
    }

    for (i=0; i<iMax; i++)
    {
        if (!(i%6))
        {
            fprintf(cubeFile,"\n");
        }
        if (!pMesh->nhex)
        {
            fprintf(cubeFile,"    %lf",0.);
        }
        else
        {
            // Exterior domain: (+/-)2; Interior domain: (+/-)3
            if (abs(pMesh->phex[i].label)==2)
            {
                fprintf(cubeFile,"    %lf",0.);
            }
            else if (abs(pMesh->phex[i].label)==3)
            {
                fprintf(cubeFile,"    %lf",1.);
            }
            else
            {
                fprintf(cubeFile,"    %lf",0.);
            }
        }
    }

    // Closing the *.cube file: fclose function returns zero if the input FILE*
    // variable is successfully closed, otherwise EOF (end-of-file) is returned
    if (fclose(cubeFile))
    {
        PRINT_ERROR("In writingCubeFile: the ");
        fprintf(stderr,"%s file has not been closed properly.\n",fileLocation);
        cubeFile=NULL;
        free(fileLocation);
        fileLocation=NULL;
        return 0;
    }
    cubeFile=NULL;
    fprintf(stdout,"Closing file.\n");

    // Free the memory dynamically allocated for fileLocation
    free(fileLocation);
    fileLocation=NULL;

    return 1;
}

