/**
* \file adaptMesh.c
* \brief It contains all the functions used to properly adapt the mesh according
*        to the molecular orbitals' metric and the one associated with the
*        level-set function of an initial starting guess for the MPD domain.
*
* \author Jeremy DALPHIN
* \version 2.0
* \date September 1st, 2018
*
* The main function of this file is called \ref adaptMesh and many other
* functions should be static but have been defined as non-static for performing
* unit-tests on them.
*/

#include "loadMesh.h"
#include "adaptMesh.h"

/* ************************************************************************** */
// The function initialDomainInMeshExists checks if the pMesh variable is
// containing an internal domain (i.e. faces are labelled 10 in the mesh, or
// if pParameters->opt_mode<=0 hexahedra are labelled 2 or 3). It has the
// Parameters* and Mesh* variables (both defined in main.h) as input arguments
// and it returns one (respectively two, minus one) if it is the case (resp. if
// it is the case but with no face labelled 10, if it is not the case),
// otherwise zero is returned if an error is encountered during the process
/* ************************************************************************** */
int initialDomainInMeshExists(Parameters* pParameters, Mesh* pMesh)
{
    int i=0, iMax=0, counter=0, counterDomain=0, returnValue=0;

    // Check if the input variables are pointing to NULL
    if (pParameters==NULL || pMesh==NULL)
    {
        PRINT_ERROR("In initialDomainInMeshExists: at least one of the ");
        fprintf(stderr,"input variable pParameters=%p or ",(void*)pParameters);
        fprintf(stderr,"pMesh=%p does not point to a valid ",(void*)pMesh);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Check and set up the iMax variable for the tetrahedral/hexahedral mesh
    if (pParameters->opt_mode>0)
    {
        // Check the total number of triangles and its associated pointer
        iMax=pMesh->ntri;
        if (iMax<1)
        {
            PRINT_ERROR("In initialDomainInMeshExists: the total number of ");
            fprintf(stderr,"triangles (=%d) of the structure pointed by ",iMax);
            fprintf(stderr,"pMesh should be a positive integer.\n");
            return 0;
        }

        if (pMesh->ptri==NULL)
        {
            PRINT_ERROR("In initialDomainInMeshExists: the variable ");
            fprintf(stderr,"pMesh->ptri=%p does not point ",(void*)pMesh->ptri);
            fprintf(stderr,"to a valid address.\n");
            return 0;
        }
    }
    else
    {
        // Check the total number of quadrilaterals and its associated pointer
        iMax=pMesh->nqua;
        if (iMax<1)
        {
            PRINT_ERROR("In initialDomainInMeshExists: the total number of ");
            fprintf(stderr,"quadrilaterals (=%d) in the structure ",iMax);
            fprintf(stderr,"pointed by pMesh should be a positive integer.\n");
            return 0;
        }

        if (pMesh->pqua==NULL)
        {
            PRINT_ERROR("In initialDomainInMeshExists: the variable ");
            fprintf(stderr,"pMesh->pqua=%p does not point ",(void*)pMesh->pqua);
            fprintf(stderr,"to a valid address.\n");
            return 0;
        }
    }

    // Search for an internal domain in the tetrahedral/hexahedral mesh
    if (pParameters->opt_mode>0)
    {
        // Return 1 if one of the triangle labels is set to 10
        for (i=0; i<iMax; i++)
        {
            if (pMesh->ptri[i].label==10)
            {
                returnValue=1;
                break;
            }
        }
    }
    else
    {
        // Return 1 if one of the quadrilateral labels is set to 10
        for (i=0; i<iMax; i++)
        {
            if (pMesh->pqua[i].label==10)
            {
                returnValue=1;
                break;
            }
        }

        // Maybe the boundary quadrilaterals were not set up but domain is there
        if (!returnValue)
        {
            // Check the total number of hexahedra and its associated pointer
            iMax=pMesh->nhex;
            if (iMax<1)
            {
                PRINT_ERROR("In initialDomainInMeshExists: the total number ");
                fprintf(stderr,"of hexahedra (=%d) in the structure ",iMax);
                fprintf(stderr,"pointed by pMesh should be a positive ");
                fprintf(stderr,"integer.\n");
                return 0;
            }

            if (pMesh->phex==NULL)
            {
                PRINT_ERROR("In initialDomainInMeshExists: the variable ");
                fprintf(stderr,"pMesh->phex=%p does not ",(void*)pMesh->phex);
                fprintf(stderr,"point to a valid address.\n");
                return 0;
            }

            // Return 2 if at least one of the hexahedron labels is set to 3
            // and the others are set to 2 or 3
            counter=0;
            counterDomain=0;
            for (i=0; i<iMax; i++)
            {
                switch (pMesh->phex[i].label)
                {
                    case 3:
                        counterDomain++;
                        counter++;
                        break;

                    case 2:
                        counter++;
                        break;

                    default:
                        break;
                }
            }
            if (counterDomain>0 && counter==iMax)
            {
                returnValue=2;
            }
        }
    }

    // Return -1 if no internal domain has been found in the mesh
    if (!returnValue)
    {
        returnValue=-1;
    }

    return returnValue;
}

/* ************************************************************************** */
// The function initializeAdjacency sets up the Adjacency structures associated
// with the quadrilaterals that are labelled 10 in structure pointed by pMesh
// (it is only used for hexahedral meshes i.e. if the pParameters->opt_mode
// variable is not positive). It has the Parameters* and Mesh* variables (both
// defined in main.h) as input arguments and it returns 1 on sucess, otherwise 0
/* ************************************************************************** */
int initializeAdjacency(Parameters* pParameters, Mesh* pMesh)
{
    size_t lengthAdjacency=0;
    int i=0, j=0, k=0, l=0, lMax=0, lPoint=0, lLabel=0, label=0, counter=0;
    int nX=0, nY=0, nZ=0, initialNumberOfQuadrilaterals=0;

    // Check that the input variables are not pointing to NULL
    if (pParameters==NULL || pMesh==NULL)
    {
        PRINT_ERROR("In initializeAdjacency: at least one of the input ");
        fprintf(stderr,"variable pParameters=%p or ",(void*)pParameters);
        fprintf(stderr,"pMesh=%p does not point to a valid ",(void*)pMesh);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Check that the mesh is made of hexahedra
    if (pParameters->opt_mode>0)
    {
        PRINT_ERROR("In initializeAdjacency: the opt_mode variable ");
        fprintf(stderr,"(=%d) of the structure pointed ",pParameters->opt_mode);
        fprintf(stderr,"by pParameters should not be a positive integer.\n");
        return 0;
    }

    // Evaluate the initial number of quadrilaterals
    nX=pParameters->n_x;
    nY=pParameters->n_y;
    nZ=pParameters->n_z;
    if (nX<3 || nY<3 || nZ<3)
    {
        PRINT_ERROR("In initializeAdjacency: the integer n_x ");
        fprintf(stderr,"(=%d), n_y (=%d), and n_z (=%d) variables ",nX,nY,nZ);
        fprintf(stderr,"of the structure pointed by pParameters should ");
        fprintf(stderr,"remain (strictly) greater than two (in order to have ");
        fprintf(stderr,"at least one normal/tangent vector on each face of ");
        fprintf(stderr,"the cube associated with the default initial ");
        fprintf(stderr,"computational box).\n");
        return 0;
    }
    initialNumberOfQuadrilaterals=2*((nX-1)*(nY-1)+(nX-1)*(nZ-1)+(nY-1)*(nZ-1));

    // Check the number of hexahedra and its associated pointer
    lMax=pMesh->nhex;
    if (lMax<1)
    {
        PRINT_ERROR("In initializeAdjacency: the total number of hexahedra ");
        fprintf(stderr,"(=%d) in the structure pointed by pMesh should ",lMax);
        fprintf(stderr,"be a positive integer.\n");
        return 0;
    }

    if (pMesh->phex==NULL)
    {
        PRINT_ERROR("In initializeAdjacency: the variable ");
        fprintf(stderr,"pMesh->phex=%p does not point to ",(void*)pMesh->phex);
        fprintf(stderr,"a valid address.\n");
        return 0;
    }

    // Count the number of boundary quadrilaterals corresponding to the initial
    // internal domain inside the computational box and save it in pMesh->nadj
    lMax=pMesh->nqua;
    if (lMax<=initialNumberOfQuadrilaterals)
    {
        PRINT_ERROR("In initializeAdjacency: expecting at least ");
        fprintf(stderr,"%d boundary ",initialNumberOfQuadrilaterals);
        fprintf(stderr,"quadrilaterals instead of %d in the structure ",lMax);
        fprintf(stderr,"pointed by pMesh.\n");
        return 0;
    }

    counter=0;
    for (l=0; l<lMax; l++)
    {
        if (pMesh->pqua[l].label==10)
        {
            counter++;
        }
    }
    pMesh->nadj=counter;

    // Check the number of quadrilaterals concerned by the internal domain
    if (initialNumberOfQuadrilaterals!=pMesh->nqua-pMesh->nadj)
    {
        PRINT_ERROR("In initializeAdjacency: expecting ");
        fprintf(stderr,"%d initial quadrilaterals instead of ",pMesh->nadj);
        fprintf(stderr,"%d ",pMesh->nqua-initialNumberOfQuadrilaterals);
        fprintf(stderr,"boundary elements enclosing the internal domain ");
        fprintf(stderr,"detected in the structure pointed by pMesh.\n");
        return 0;
    }

    // Allocate memory for the Adjacency structures
    // calloc function returns a pointer to allocated memory, otherwise NULL
    lengthAdjacency=pMesh->nadj;
    pMesh->padj=(Adjacency*)calloc(lengthAdjacency,sizeof(Adjacency));
    if (pMesh->padj==NULL)
    {
        PRINT_ERROR("In initializeAdjacency: could not allocate memory for ");
        fprintf(stderr,"the Adjacency structures pointed by pMesh->padj.\n");
        return 0;
    }

    // Set Adjacency of boundary quadrilaterals that are not faces of the cube
    counter=0;
    lMax=pMesh->nhex;
    for (l=0; l<lMax; l++)
    {
        label=pMesh->phex[l].label;
        k=l%(nZ-1);
        lPoint=l/(nZ-1);
        j=lPoint%(nY-1);
        i=lPoint/(nY-1);

        // In the hexahedron corresponding to each cube cell, the vertices are
        // saved as (a*nY+b)*nZ+c a=0...nX-1, b=0...nY-1, and c=0...nZ-1
        // Point reference (a,b,c) in each cube cell: 1=(i-1,j-1,k-1)
        // 2=(i,j-1,k-1) 3=(i,j,k-1) 4=(i-1,j,k-1) 5=(i-1,j-1,k) 6=(i,j-1,k)
        // 7=(i,j,k) 8=(i-1,j,k) i=1...nX-1; j=1...nY-1; k=1...nZ-1
        if (i>0)
        {
            lPoint=((i-1)*(nY-1)+j)*(nZ-1)+k;
            lLabel=pMesh->phex[lPoint].label;

            // Test made not to count two times the quadrilaterals
            if (label==2 && lLabel==3)
            {
                pMesh->padj[counter].quad=initialNumberOfQuadrilaterals+counter;
                pMesh->padj[counter].hexout=l+1;
                pMesh->padj[counter].hexin=lPoint+1;
                counter++;
            }
        }
        if (i<nX-2)
        {
            lPoint=((i+1)*(nY-1)+j)*(nZ-1)+k;
            lLabel=pMesh->phex[lPoint].label;
            if (label==2 && lLabel==3)
            {
                pMesh->padj[counter].quad=initialNumberOfQuadrilaterals+counter;
                pMesh->padj[counter].hexout=l+1;
                pMesh->padj[counter].hexin=lPoint+1;
                counter++;
            }
        }
        if (j>0)
        {
            lPoint=(i*(nY-1)+(j-1))*(nZ-1)+k;
            lLabel=pMesh->phex[lPoint].label;
            if (label==2 && lLabel==3)
            {
                pMesh->padj[counter].quad=initialNumberOfQuadrilaterals+counter;
                pMesh->padj[counter].hexout=l+1;
                pMesh->padj[counter].hexin=lPoint+1;
                counter++;
            }
        }
        if (j<nY-2)
        {
            lPoint=(i*(nY-1)+(j+1))*(nZ-1)+k;
            lLabel=pMesh->phex[lPoint].label;
            if (label==2 && lLabel==3)
            {
                pMesh->padj[counter].quad=initialNumberOfQuadrilaterals+counter;
                pMesh->padj[counter].hexout=l+1;
                pMesh->padj[counter].hexin=lPoint+1;
                counter++;
            }
        }
        if (k>0)
        {
            lPoint=(i*(nY-1)+j)*(nZ-1)+(k-1);
            lLabel=pMesh->phex[lPoint].label;
            if (label==2 && lLabel==3)
            {
                pMesh->padj[counter].quad=initialNumberOfQuadrilaterals+counter;
                pMesh->padj[counter].hexout=l+1;
                pMesh->padj[counter].hexin=lPoint+1;
                counter++;
            }
        }
        if (k<nZ-2)
        {
            lPoint=(i*(nY-1)+j)*(nZ-1)+(k+1);
            lLabel=pMesh->phex[lPoint].label;
            if (label==2 && lLabel==3)
            {
                pMesh->padj[counter].quad=initialNumberOfQuadrilaterals+counter;
                pMesh->padj[counter].hexout=l+1;
                pMesh->padj[counter].hexin=lPoint+1;
                counter++;
            }
        }
    }

    // Check the number of quadrilaterals related to the Adjacency structures
    if (pMesh->nadj!=counter)
    {
        PRINT_ERROR("In initializeAdjacency: expecting ");
        fprintf(stderr,"%d initial quadrilaterals enclosing the ",pMesh->nadj);
        fprintf(stderr,"internal domain detected in the structure pointed by ");
        fprintf(stderr,"pMesh instead of the %d boundary elements ",counter);
        fprintf(stderr,"obtained while looking at the hexahedron labels.\n");
        return 0;
    }

    return 1;
}

/* ************************************************************************** */
// The function initializeLevelSetFunction generates either the initial 
// level-set function asssociated with the initial domain (sphere/cube) at the
// vertices of the tetrahedral structure pointed by pMesh, or directly the
// initial internal domain by labelling 3 the elements of the hexahedral
// structure that belongs to the domain otherwise 2. The pParameters->ls_* 
// variables characterizes the initial domain as a cube or a sphere. It has the
// Parameters* and Mesh* variables (both defined in main.h) as input arguments
// and it returns one on sucess, otherwise zero is returned in case of error
/* ************************************************************************** */
int initializeLevelSetFunction(Parameters* pParameters, Mesh* pMesh)
{
    int l=0, lMax=0, lPoint=0;
    double value=0., dx=0., dy=0., dz=0., xMin=0., xMax=0., yMin=0.;
    double yMax=0., zMin=0., zMax=0., r0=0., x0=0., y0=0., z0=0.;
    Point *pPoint=NULL;
    Hexahedron *pHexahedron=NULL;

    // Check that the input variables are not pointing to NULL
    if (pParameters==NULL || pMesh==NULL)
    {
        PRINT_ERROR("In initializeLevelSetFunction: at least one of the ");
        fprintf(stderr,"input variable pParameters=%p or ",(void*)pParameters);
        fprintf(stderr,"pMesh=%p does not point to a valid ",(void*)pMesh);
        fprintf(stderr,"address.\n");
        return 0;
    }

    // Set up the local parameters for the initial sphere/cube
    x0=pParameters->ls_x;
    y0=pParameters->ls_y;
    z0=pParameters->ls_z;
    r0=pParameters->ls_r;
    if (r0<=0.)
    {
        PRINT_ERROR("In initializeLevelSetFunction: the prescribed radius ");
        fprintf(stderr,"(=%lf) for the initial sphere should be ",r0);
        fprintf(stderr,"positive.\n");
        return 0;
    }

    // Check and set correctly the lMax variable for tetrahedral/hexahedral mesh
    lMax=pMesh->nver;
    if (lMax<1)
    {
        PRINT_ERROR("In initializeLevelSetFunction: the total number of ");
        fprintf(stderr,"vertices (=%d) in the structure pointed by ",lMax);
        fprintf(stderr,"pMesh should be a positive integer.\n");
        return 0;
    }

    if (pMesh->pver==NULL)
    {
        PRINT_ERROR("In initializeLevelSetFunction: the variable ");
        fprintf(stderr,"pMesh->pver=%p of the structure ",(void*)pMesh->pver);
        fprintf(stderr,"pointed by pMesh does not point to a valid address.\n");
        return 0;
    }

    if (pParameters->opt_mode<=0)
    {
        // Check number of hexahedra and its associated pointer
        lMax=pMesh->nhex;
        if (lMax<1)
        {
            PRINT_ERROR("In initializeLevelSetFunction: the total number of ");
            fprintf(stderr,"hexahedra (=%d) in the structure pointed by ",lMax);
            fprintf(stderr,"pMesh should be a positive integer.\n");
            return 0;
        }

        if (pMesh->phex==NULL)
        {
            PRINT_ERROR("In initializeLevelSetFunction: the variable ");
            fprintf(stderr,"pMesh->phex=%p of the ",(void*)pMesh->phex);
            fprintf(stderr,"structure pointed by pMesh does not point to a ");
            fprintf(stderr,"valid address.\n");
            return 0;
        }
    }

    // Distinguish the case where the initial domain is a sphere or a cube
    if (pParameters->ls_type)
    {
        fprintf(stdout,"\nInitializing the signed distance function ");
        if (pParameters->verbose)
        {
            fprintf(stdout,"of the sphere of radius %lf and center ",r0);
            fprintf(stdout,"(%lf,%lf,%lf).\n",x0,y0,z0);
        }
        else
        {
            fprintf(stdout,"of the initial domain (sphere).\n");
        }

        if (pParameters->opt_mode>0)
        {
            // Save the initial level-set function in pMesh->pver[l].value
            for (l=0; l<lMax; l++)
            {
                pPoint=&pMesh->pver[l];
                dx=pPoint->x-x0;
                dy=pPoint->y-y0;
                dz=pPoint->z-z0;
                value=dx*dx+dy*dy+dz*dz;
                pPoint->value=sqrt(value)-r0;
            }
        }
        else
        {
            // Labelling the exterior hexahedra by 2 (positive level-set
            // function) while the interior ones are labelled 3 (negative value)
            for (l=0; l<lMax; l++)
            {
                pHexahedron=&pMesh->phex[l];
                lPoint=pHexahedron->p1;

                // Warning here: points are stored thanks to the .mesh file so
                // do not forget to remove one from these int when calling a
                // point in the structure pointed by pMesh
                pPoint=&pMesh->pver[lPoint-1];

                dx=pPoint->x+.5*pParameters->delta_x-x0;
                dy=pPoint->y+.5*pParameters->delta_y-y0;
                dz=pPoint->z+.5*pParameters->delta_z-z0;

                value=dx*dx+dy*dy+dz*dz;
                value=sqrt(value)-r0;
                if (value>0.)
                {
                    pHexahedron->label=2;
                }
                else
                {
                    pHexahedron->label=3;
                }
            }
        }
    }
    else
    {
        fprintf(stdout,"\nInitializing the signed distance function ");
        if (pParameters->verbose)
        {
            fprintf(stdout,"of the cube [%lf,%lf]x",xMin,xMax);
            fprintf(stdout,"[%lf,%lf]x[%lf,%lf] of ",yMin,yMax,zMin,zMax);
            fprintf(stdout,"length %lf and center ",r0);
            fprintf(stdout,"(%lf,%lf,%lf).\n",x0,y0,z0);
        }
        else
        {
            fprintf(stdout,"of the initial domain (cube).\n");
        }

        xMin=x0-.5*r0;
        yMin=y0-.5*r0;
        zMin=z0-.5*r0;

        xMax=x0+.5*r0;
        yMax=y0+.5*r0;
        zMax=z0+.5*r0;

        if (pParameters->opt_mode>0)
        {
            // Save the initial level-set function in pMesh->pver[l].value
            lMax=pMesh->nver;
            for (l=0; l<lMax; l++)
            {
                pPoint=&pMesh->pver[l];
                dx=pPoint->x-xMax;
                value=dx;
                if (value<=0.)
                {
                    dx=xMin-pPoint->x;
                    if (dx>0.)
                    {
                        value=dx;
                    }
                    else
                    {
                        if (dx>value)
                        {
                            value=dx;
                        }
                    }
                }
                dx=value;

                dy=pPoint->y-yMax;
                value=dy;
                if (value<=0.)
                {
                    dy=yMin-pPoint->y;
                    if (dy>0.)
                    {
                        value=dy;
                    }
                    else
                    {
                        if (dy>value)
                        {
                            value=dy;
                        }
                    }
                }
                dy=value;

                dz=pPoint->z-zMax;
                value=dz;
                if (value<=0.)
                {
                    dz=zMin-pPoint->z;
                    if (dz>0.)
                    {
                        value=dz;
                    }
                    else
                    {
                        if (dz>value)
                        {
                            value=dz;
                        }
                    }
                }
                dz=value;

                if (dx<=0.)
                {
                    if (dy<=0.)
                    {
                        if (dz<=0.)
                        {
                            value=DEF_MAX(dx,dy);
                            value=DEF_MAX(value,dz);
                        }
                        else
                        {
                            value=dz;
                        }
                    }
                    else
                    {
                        if (dz<=0.)
                        {
                            value=dy;
                        }
                        else
                        {
                            value=sqrt(dy*dy+dz*dz);
                        }
                    }
                }
                else
                {
                    if (dy<=0.)
                    {
                        if (dz<=0.)
                        {
                            value=dx;
                        }
                        else
                        {
                            value=sqrt(dx*dx+dz*dz);
                        }
                    }
                    else
                    {
                        if (dz<=0.)
                        {
                            value=sqrt(dx*dx+dy*dy);
                        }
                        else
                        {
                            value=sqrt(dx*dx+dy*dy+dz*dz);
                        }
                    }
                }
                pPoint->value=value;
            }
        }
        else
        {
            // Labelling the exterior hexahedra by 2 (positive level-set
            // function) while the interior ones are labelled 3 (negative value)
            for (l=0; l<lMax; l++)
            {
                pHexahedron=&pMesh->phex[l];
                lPoint=pHexahedron->p1;

                // Warning here: points are stored thanks to the .mesh file so
                // do not forget to remove one from these int when calling a
                // point in the structure pointed by pMesh
                pPoint=&pMesh->pver[lPoint-1];
                x0=pPoint->x+.5*pParameters->delta_x;
                y0=pPoint->y+.5*pParameters->delta_y;
                z0=pPoint->z+.5*pParameters->delta_z;

                dx=x0-xMax;
                value=dx;
                if (value<=0.)
                {
                    dx=xMin-x0;
                    if (dx>0.)
                    {
                        value=dx;
                    }
                    else
                    {
                        if (dx>value)
                        {
                            value=dx;
                        }
                    }
                }
                dx=value;

                dy=y0-yMax;
                value=dy;
                if (value<=0.)
                {
                    dy=yMin-y0;
                    if (dy>0.)
                    {
                        value=dy;
                    }
                    else
                    {
                        if (dy>value)
                        {
                            value=dy;
                        }
                    }
                }
                dy=value;

                dz=z0-zMax;
                value=dz;
                if (value<=0.)
                {
                    dz=zMin-z0;
                    if (dz>0.)
                    {
                        value=dz;
                    }
                    else
                    {
                        if (dz>value)
                        {
                            value=dz;
                        }
                    }
                }
                dz=value;

                if (dx<=0. && dy<=0. && dz<=0.)
                {
                      pHexahedron->label=3;
                }
                else
                {
                    pHexahedron->label=2;
                }
            }
        }
    }

    return 1;
}

/* ************************************************************************** */
// The function getLevelSetQuadrilaterals modifies the pMesh structure so that
// it contains the quadrilaterals corresponding to the boundary of the internal
// MPD domain (and update the Adjacency structures accordingly). It has the
// Parameters* and Mesh* variables (defined in main.h) as input arguments and it
// returns one on sucess, otherwise zero is returned if an error is encountered
/* ************************************************************************** */
int getLevelSetQuadrilaterals(Parameters* pParameters, Mesh* pMesh)
{
    size_t length=0;
    int i=0, j=0, k=0, l=0, lMax=0, p1=0, p2=0, p3=0, p4=0, p5=0, p6=0;
    int p7=0, p8=0, label=0, lLabel=0, counter=0, lPoint=0;
    int nX=0, nY=0, nZ=0, initialNumberOfQuadrilaterals=0;
    Quadrilateral *pQuadrilateral=NULL;
    Hexahedron *pHexahedron=NULL;

    // Check that the input variables are not pointing to NULL
    if (pParameters==NULL || pMesh==NULL)
    {
        PRINT_ERROR("In getLevelSetQuadrilaterals: one of the input ");
        fprintf(stderr,"variable pParameters=%p or ",(void*)pParameters);
        fprintf(stderr,"pMesh=%p is not a valid address.\n",(void*)pMesh);
        return 0;
    }

    if (pParameters->opt_mode>0)
    {
        PRINT_ERROR("In getLevelSetQuadrilaterals: the opt_mode variable ");
        fprintf(stderr,"(=%d) of the structure pointed ",pParameters->opt_mode);
        fprintf(stderr,"by pParameters should not be a positive integer.\n");
        return 0;
    }

    // Evaluate the initial number of quadrilaterals
    nX=pParameters->n_x;
    nY=pParameters->n_y;
    nZ=pParameters->n_z;
    if (nX<3 || nY<3 || nZ<3)
    {
        PRINT_ERROR("In getLevelSetQuadrilaterals: expecting that the n_x ");
        fprintf(stderr,"(=%d), n_y (=%d), and n_z (=%d) variables ",nX,nY,nZ);
        fprintf(stderr,"of the structure pointed by pParameters must remain ");
        fprintf(stderr,"(strictly) greater than two (in order to have at ");
        fprintf(stderr,"least one normal/tangent vector on each face of the ");
        fprintf(stderr,"cube associated with the default initial ");
        fprintf(stderr,"computational box).\n");
        return 0;
    }
    initialNumberOfQuadrilaterals=2*((nX-1)*(nY-1)+(nX-1)*(nZ-1)+(nY-1)*(nZ-1));

    // Counting the number of quadrilaterals corresponding to the MPD domain
    lMax=pMesh->nhex;
    if (lMax<1)
    {
        PRINT_ERROR("In getLevelSetQuadrilaterals: expecting a ");
        fprintf(stderr,"positive number of hexahedra instead of ");
        fprintf(stderr,"%d in the structure pointed by pMesh.\n",lMax);
        return 0;
    }
    if (pMesh->phex==NULL)
    {
        PRINT_ERROR("In getLevelSetQuadrilaterals: the phex variable of the ");
        fprintf(stderr,"the structure pointed by pMesh is pointing to the ");
        fprintf(stderr,"%p address.\n",(void*)pMesh->phex);
        return 0;
    }

    counter=0;
    for (l=0; l<lMax; l++)
    {
        label=pMesh->phex[l].label;
        k=l%(nZ-1);
        lPoint=l/(nZ-1);
        j=lPoint%(nY-1);
        i=lPoint/(nY-1);
        if (i>0)
        {
            lPoint=((i-1)*(nY-1)+j)*(nZ-1)+k;
            lLabel=pMesh->phex[lPoint].label;

            // This ensures no to count two times the quadrilaterals.
            if (label==2 && lLabel==3)
            {
                counter++;
            }
        }
        if (i<nX-2)
        {
            lPoint=((i+1)*(nY-1)+j)*(nZ-1)+k;
            lLabel=pMesh->phex[lPoint].label;
            if (label==2 && lLabel==3)
            {
                counter++;
            }
        }
        if (j>0)
        {
            lPoint=(i*(nY-1)+(j-1))*(nZ-1)+k;
            lLabel=pMesh->phex[lPoint].label;
            if (label==2 && lLabel==3)
            {
                counter++;
            }
        }
        if (j<nY-2)
        {
            lPoint=(i*(nY-1)+(j+1))*(nZ-1)+k;
            lLabel=pMesh->phex[lPoint].label;
            if (label==2 && lLabel==3)
            {
                counter++;
            }
        }
        if (k>0)
        {
            lPoint=(i*(nY-1)+j)*(nZ-1)+(k-1);
            lLabel=pMesh->phex[lPoint].label;
            if (label==2 && lLabel==3)
            {
                counter++;
            }
        }
        if (k<nZ-2)
        {
            lPoint=(i*(nY-1)+j)*(nZ-1)+(k+1);
            lLabel=pMesh->phex[lPoint].label;
            if (label==2 && lLabel==3)
            {
                counter++;
            }
        }
    }
    pMesh->nadj=counter;

    // Reallocate memory for the quadrilaterals and check if it worked
    // realloc function returns a pointer to allocated memory, otherwise NULL
    length=initialNumberOfQuadrilaterals+counter;
    pQuadrilateral=(Quadrilateral*)realloc(pMesh->pqua,
                                                  length*sizeof(Quadrilateral));
    if (pQuadrilateral==NULL)
    {
        PRINT_ERROR("In getLevelSetQuadrilaterals: could not reallocate ");
        fprintf(stderr,"memory for additional quadrilaterals associated with ");
        fprintf(stderr,"the internal domain of the structure pointed by ");
        fprintf(stderr,"pMesh.\n");
        return 0;
    }
    pMesh->pqua=(Quadrilateral*)pQuadrilateral;

    // Allocate memory for the Adjacency structure pointed by pMesh.
    // calloc function returns a pointer to allocated memory, otherwise NULL
    length=counter;
    pMesh->padj=(Adjacency*)calloc(length,sizeof(Adjacency));
    if (pMesh->padj==NULL)
    {
        PRINT_ERROR("In getLevelSetQuadrilaterals: could not allocate memory ");
        fprintf(stderr,"for the Adjacency structures pointed by pMesh.\n");
        return 0;
    }

    // Save boundary quadrilaterals that are not faces of the computational box
    lMax=pMesh->nhex;
    counter=initialNumberOfQuadrilaterals;
    for (l=0; l<lMax; l++)
    {
        pHexahedron=&pMesh->phex[l];
        p1=pHexahedron->p1;
        p2=pHexahedron->p2;
        p3=pHexahedron->p3;
        p4=pHexahedron->p4;
        p5=pHexahedron->p5;
        p6=pHexahedron->p6;
        p7=pHexahedron->p7;
        p8=pHexahedron->p8;
        label=pHexahedron->label;

        k=l%(nZ-1);
        lPoint=l/(nZ-1);
        j=lPoint%(nY-1);
        i=lPoint/(nY-1);

        // In the hexahedron corresponding to each cube cell, the vertices are
        // saved as (a*nY+b)*nZ+c a=0...nX-1, b=0...nY-1, c=0...nZ-1
        // Point reference (a,b,c) in a cube cell: 1=(i-1,j-1,k-1) 2=(i,j-1,k-1)
        // 3=(i,j,k-1) 4=(i-1,j,k-1) 5=(i-1,j-1,k) 6=(i,j-1,k) 7=(i,j,k)
        // 8=(i-1,j,k) i=1...nX-1; j=1...nY-1; k=1...nZ-1
        if (i>0)
        {
            lPoint=((i-1)*(nY-1)+j)*(nZ-1)+k;
            lLabel=pMesh->phex[lPoint].label;

            // This ensures not to count two times the quadrilaterals. The mesh
            // labels can be negative here (see pParameters->trick_matrix)
            if (label==2 && lLabel==3)
            {
                // Square 1584 (do not forget to save the quadrilaterals with
                // a trigonometric orientation when looking from the exterior)
                pQuadrilateral=&pMesh->pqua[counter];
                pQuadrilateral->p1=p1;
                pQuadrilateral->p2=p5;
                pQuadrilateral->p3=p8;
                pQuadrilateral->p4=p4;
                pQuadrilateral->label=10;
                pMesh->padj[counter-initialNumberOfQuadrilaterals].quad=
                                                                      counter+1;
                pMesh->padj[counter-initialNumberOfQuadrilaterals].hexout=l+1;
                pMesh->padj[counter-initialNumberOfQuadrilaterals].hexin=
                                                                       lPoint+1;
                counter++;
            }
        }
        if (i<nX-2)
        {
            lPoint=((i+1)*(nY-1)+j)*(nZ-1)+k;
            lLabel=pMesh->phex[lPoint].label;
            if (label==2 && lLabel==3)
            {
                // Square 2376
                pQuadrilateral=&pMesh->pqua[counter];
                pQuadrilateral->p1=p2;
                pQuadrilateral->p2=p3;
                pQuadrilateral->p3=p7;
                pQuadrilateral->p4=p6;
                pQuadrilateral->label=10;
                pMesh->padj[counter-initialNumberOfQuadrilaterals].quad=
                                                                      counter+1;
                pMesh->padj[counter-initialNumberOfQuadrilaterals].hexout=l+1;
                pMesh->padj[counter-initialNumberOfQuadrilaterals].hexin=
                                                                       lPoint+1;
                counter++;
            }
        }
        if (j>0)
        {
            lPoint=(i*(nY-1)+(j-1))*(nZ-1)+k;
            lLabel=pMesh->phex[lPoint].label;
            if (label==2 && lLabel==3)
            {
                // Square 1265
                pQuadrilateral=&pMesh->pqua[counter];
                pQuadrilateral->p1=p1;
                pQuadrilateral->p2=p2;
                pQuadrilateral->p3=p6;
                pQuadrilateral->p4=p5;
                pQuadrilateral->label=10;
                pMesh->padj[counter-initialNumberOfQuadrilaterals].quad=
                                                                      counter+1;
                pMesh->padj[counter-initialNumberOfQuadrilaterals].hexout=l+1;
                pMesh->padj[counter-initialNumberOfQuadrilaterals].hexin=
                                                                       lPoint+1;
                counter++;
            }
        }
        if (j<nY-2)
        {
            lPoint=(i*(nY-1)+(j+1))*(nZ-1)+k;
            lLabel=pMesh->phex[lPoint].label;
            if (label==2 && lLabel==3)
            {
                // Square 3487
                pQuadrilateral=&pMesh->pqua[counter];
                pQuadrilateral->p1=p3;
                pQuadrilateral->p2=p4;
                pQuadrilateral->p3=p8;
                pQuadrilateral->p4=p7;
                pQuadrilateral->label=10;
                pMesh->padj[counter-initialNumberOfQuadrilaterals].quad=
                                                                      counter+1;
                pMesh->padj[counter-initialNumberOfQuadrilaterals].hexout=l+1;
                pMesh->padj[counter-initialNumberOfQuadrilaterals].hexin=
                                                                       lPoint+1;
                counter++;
            }
        }
        if (k>0)
        {
            lPoint=(i*(nY-1)+j)*(nZ-1)+(k-1);
            lLabel=pMesh->phex[lPoint].label;
            if (label==2 && lLabel==3)
            {
                // Square 1432
                pQuadrilateral=&pMesh->pqua[counter];
                pQuadrilateral->p1=p1;
                pQuadrilateral->p2=p4;
                pQuadrilateral->p3=p3;
                pQuadrilateral->p4=p2;
                pQuadrilateral->label=10;
                pMesh->padj[counter-initialNumberOfQuadrilaterals].quad=
                                                                      counter+1;
                pMesh->padj[counter-initialNumberOfQuadrilaterals].hexout=l+1;
                pMesh->padj[counter-initialNumberOfQuadrilaterals].hexin=
                                                                       lPoint+1;
                counter++;
            }
        }
        if (k<nZ-2)
        {
            lPoint=(i*(nY-1)+j)*(nZ-1)+(k+1);
            lLabel=pMesh->phex[lPoint].label;
            if (label==2 && lLabel==3)
            {
                // Square 5678
                pQuadrilateral=&pMesh->pqua[counter];
                pQuadrilateral->p1=p5;
                pQuadrilateral->p2=p6;
                pQuadrilateral->p3=p7;
                pQuadrilateral->p4=p8;
                pQuadrilateral->label=10;
                pMesh->padj[counter-initialNumberOfQuadrilaterals].quad=
                                                                      counter+1;
                pMesh->padj[counter-initialNumberOfQuadrilaterals].hexout=l+1;
                pMesh->padj[counter-initialNumberOfQuadrilaterals].hexin=
                                                                       lPoint+1;
                counter++;
            }
        }
    }

    // Updating the number of quadrilaterals
    pMesh->nqua=initialNumberOfQuadrilaterals+pMesh->nadj;
    if (pMesh->nqua!=counter)
    {
        PRINT_ERROR("In getLevelSetQuadrilaterals: expecting ");
        fprintf(stderr,"%d quadrilaterals instead of %d ",pMesh->nadj,counter);
        fprintf(stderr,"for the boundary faces of the internal domain.\n");
        return 0;
    }

    return 1;
}

/* ************************************************************************** */
// The function evaluatingHessianAtVertices evaluates a (Gaussian function)
// primitive of pMolecularOrbital described in pChemicalSystem at pPoint, and
// it also calculates its gradient and hessian matrix. It has the
// ChemicalSystem*, MolecularOrbital*, Point* variables (both defined in
// main.h), and the integer characterizing the Gaussian as input arguments.
// It returns the value of the gaussian primitive, fills gradient[3] with its
// exact first-order derivatives, and hessian[6] (stored as 0->xx 1->yy 2->zz
// 3->xy,yx 4->xz,zx 5->yz,zy) with its exact second-order ones
/* ************************************************************************** */
double evaluatingHessianAtVertices(ChemicalSystem* pChemicalSystem,
                                   MolecularOrbital* pMolecularOrbital,
                                   Point* pPoint, double gradient[3],
                                               double hessian[6], int primitive)
{
    // First, if the coefficient is zero, return zero to avoid waist of time
    double coefficient=pMolecularOrbital->coeff[primitive];
    if (coefficient==0.)
    {
        return 0.;
    }

    double exponent=pMolecularOrbital->exp[primitive];
    int type=pMolecularOrbital->type[primitive];

    // Warning: we stored Nuclei reference as in *.chem file so do not forget
    // to remove one from these int when calling pChemicalSystem->pnucl
    int iNucleus=pMolecularOrbital->nucl[primitive];
    Nucleus *pCenter=&pChemicalSystem->pnucl[iNucleus-1];

    double dx=(pPoint->x)-(pCenter->x);
    double dy=(pPoint->y)-(pCenter->y);
    double dz=(pPoint->z)-(pCenter->z);

    double distanceSquared=dx*dx+dy*dy+dz*dz;
    double exponent2=2.*exponent;

    double dxx=exponent2*dx*dx;
    double dyy=exponent2*dy*dy;
    double dzz=exponent2*dz*dz;

    double dxy=exponent2*dx*dy;
    double dxz=exponent2*dx*dz;
    double dyz=exponent2*dy*dz;

    double value=coefficient*exp(-exponent*distanceSquared);
    double function=value;

    // Evaluate function, gradient and hessian according of the type of the
    // Gaussian primitive function (ORB_* are defined in main.h). Warning here:
    // gradient and hessian must have been correctly initialized to zero before)
    switch (type)
    {
        default:
            fprintf(stdout,"\nWarning in evaluatingHessianAtVertices ");
            fprintf(stdout,"function: the local type (=%d) variable ",type);
            fprintf(stdout,"should be a (positive) integer not (strictly) ");
            fprintf(stdout,"greater than twenty here. We have treated it as ");
            fprintf(stdout,"if it was equal to one (1s-type orbital).\n");

        // These preprocessor constants are defined in main.h (hence common to
        // all *.h files) because it is also used in optimization.c functions
        case ORB_1S:
            value*=exponent2;
            gradient[0]-=value*dx;
            gradient[1]-=value*dy;
            gradient[2]-=value*dz;
            hessian[0]+=value*(dxx-1.);
            hessian[1]+=value*(dyy-1.);
            hessian[2]+=value*(dzz-1.);
            hessian[3]+=value*dxy;
            hessian[4]+=value*dxz;
            hessian[5]+=value*dyz;
            break;

        case ORB_2PX:
            function*=(dxx-1.);
            gradient[0]-=function;
            function*=exponent2;
            hessian[3]+=function*dy;
            hessian[4]+=function*dz;
            value*=dx;
            function=value;
            value*=exponent2;
            gradient[1]-=value*dy;
            gradient[2]-=value*dz;
            hessian[0]+=value*(dxx-3.);
            hessian[1]+=value*(dyy-1.);
            hessian[2]+=value*(dzz-1.);
            hessian[5]+=value*dyz;
            break;

        case ORB_2PY:
            function*=(dyy-1.);
            gradient[1]-=function;
            function*=exponent2;
            hessian[3]+=function*dx;
            hessian[5]+=function*dz;
            value*=dy;
            function=value;
            value*=exponent2;
            gradient[0]-=value*dx;
            gradient[2]-=value*dz;
            hessian[0]+=value*(dxx-1.);
            hessian[1]+=value*(dyy-3.);
            hessian[2]+=value*(dzz-1.);
            hessian[4]+=value*dxz;
            break;

        case ORB_2PZ:
            function*=(dzz-1.);
            gradient[2]-=function;
            function*=exponent2;
            hessian[4]+=function*dx;
            hessian[5]+=function*dy;
            value*=dz;
            function=value;
            value*=exponent2;
            gradient[0]-=value*dx;
            gradient[1]-=value*dy;
            hessian[0]+=value*(dxx-1.);
            hessian[1]+=value*(dyy-1.);
            hessian[2]+=value*(dzz-3.);
            hessian[3]+=value*dxy;
            break;

        case ORB_3DXX:
            hessian[0]+=value*(2.+dxx*(dxx-5.));
            function*=dx*(dxx-2.);
            gradient[0]-=function;
            function*=exponent2;
            hessian[3]+=function*dy;
            hessian[4]+=function*dz;
            value*=dx*dx;
            function=value;
            value*=exponent2;
            gradient[1]-=value*dy;
            gradient[2]-=value*dz;
            hessian[1]+=value*(dyy-1.);
            hessian[2]+=value*(dzz-1.);
            hessian[5]+=value*dyz;
            break;

        case ORB_3DYY:
            hessian[1]+=value*(2.+dyy*(dyy-5.));
            function*=dy*(dyy-2.);
            gradient[1]-=function;
            function*=exponent2;
            hessian[3]+=function*dx;
            hessian[5]+=function*dz;
            value*=dy*dy;
            function=value;
            value*=exponent2;
            gradient[0]-=value*dx;
            gradient[2]-=value*dz;
            hessian[0]+=value*(dxx-1.);
            hessian[2]+=value*(dzz-1.);
            hessian[4]+=value*dxz;
            break;

        case ORB_3DZZ:
            hessian[2]+=value*(2.+dzz*(dzz-5.));
            function*=dz*(dzz-2.);
            gradient[2]-=function;
            function*=exponent2;
            hessian[4]+=function*dx;
            hessian[5]+=function*dy;
            value*=dz*dz;
            function=value;
            value*=exponent2;
            gradient[0]-=value*dx;
            gradient[1]-=value*dy;
            hessian[0]+=value*(dxx-1.);
            hessian[1]+=value*(dyy-1.);
            hessian[3]+=value*dxy;
            break;

        case ORB_3DXY:
            hessian[3]+=value*(dyy-1.)*(dxx-1.);
            function*=dy*(dxx-1.);
            gradient[0]-=function;
            hessian[4]+=function*exponent2*dz;
            value*=dx;
            function=value*(dyy-1.);
            gradient[1]-=function;
            hessian[5]+=function*exponent2*dz;
            value*=dy;
            function=value;
            value*=exponent2;
            gradient[2]-=value*dz;
            hessian[0]+=value*(dxx-3.);
            hessian[1]+=value*(dyy-3.);
            hessian[2]+=value*(dzz-1.);
            break;

        case ORB_3DXZ:
            hessian[4]+=value*(dxx-1.)*(dzz-1.);
            function*=dz*(dxx-1.);
            gradient[0]-=function;
            hessian[3]+=function*exponent2*dy;
            value*=dx;
            function=value*(dzz-1.);
            gradient[2]-=function;
            hessian[5]+=function*exponent2*dy;
            value*=dz;
            function=value;
            value*=exponent2;
            gradient[1]-=value*dy;
            hessian[0]+=value*(dxx-3.);
            hessian[1]+=value*(dyy-1.);
            hessian[2]+=value*(dzz-3.);
            break;

        case ORB_3DYZ:
            hessian[5]+=value*(dyy-1.)*(dzz-1.);
            function*=dz*(dyy-1.);
            gradient[1]-=function;
            hessian[3]+=function*exponent2*dx;
            value*=dy;
            function=value*(dzz-1.);
            gradient[2]-=function;
            hessian[4]+=function*exponent2*dx;
            value*=dz;
            function=value;
            value*=exponent2;
            gradient[0]-=value*dx;
            hessian[0]+=value*(dxx-1.);
            hessian[1]+=value*(dyy-3.);
            hessian[2]+=value*(dzz-3.);
            break;

        case ORB_4FXXX:
            hessian[0]+=value*dx*(6.+dxx*(dxx-7.));
            value*=dx*dx;
            function=value*(dxx-3.);
            gradient[0]-=function;
            hessian[3]+=function*exponent2*dy;
            hessian[4]+=function*exponent2*dz;
            value*=dx;
            function=value;
            value*=exponent2;
            gradient[1]-=value*dy;
            gradient[2]-=value*dz;
            hessian[1]+=value*(dyy-1.);
            hessian[2]+=value*(dzz-1.);
            hessian[5]+=value*dyz;
            break;

        case ORB_4FYYY:
            hessian[1]+=value*dy*(6.+dyy*(dyy-7.));
            value*=dy*dy;
            function=value*(dyy-3.);
            gradient[1]-=function;
            hessian[3]+=function*exponent2*dx;
            hessian[5]+=function*exponent2*dz;
            value*=dy;
            function=value;
            value*=exponent2;
            gradient[0]-=value*dx;
            gradient[2]-=value*dz;
            hessian[0]+=value*(dxx-1.);
            hessian[2]+=value*(dzz-1.);
            hessian[4]+=value*dxz;
            break;

        case ORB_4FZZZ:
            hessian[2]+=value*dz*(6.+dzz*(dzz-7.));
            value*=dz*dz;
            function=value*(dzz-3.);
            gradient[2]-=function;
            hessian[4]+=function*exponent2*dx;
            hessian[5]+=function*exponent2*dy;
            value*=dz;
            function=value;
            value*=exponent2;
            gradient[0]-=value*dx;
            gradient[1]-=value*dy;
            hessian[0]+=value*(dxx-1.);
            hessian[1]+=value*(dyy-1.);
            hessian[3]+=value*dxy;
            break;

        case ORB_4FXXY:
            hessian[0]+=value*dy*(2.+dxx*(dxx-5.));
            function*=dx*(dxx-2.);
            hessian[3]+=function*(dyy-1.);
            function*=dy;
            gradient[0]-=function;
            hessian[4]+=function*exponent2*dz;
            value*=dx*dx;
            function=value*(dyy-1.);
            gradient[1]-=function;
            hessian[5]+=function*exponent2*dz;
            value*=dy;
            function=value;
            value*=exponent2;
            gradient[2]-=value*dz;
            hessian[1]+=value*(dyy-3.);
            hessian[2]+=value*(dzz-1.);
            break;

        case ORB_4FXXZ:
            hessian[0]+=value*dz*(2.+dxx*(dxx-5.));
            function*=dx*(dxx-2.);
            hessian[4]+=function*(dzz-1.);
            function*=dz;
            gradient[0]-=function;
            hessian[3]+=function*exponent2*dy;
            value*=dx*dx;
            function=value*(dzz-1.);
            gradient[2]-=function;
            hessian[5]+=function*exponent2*dy;
            value*=dz;
            function=value;
            value*=exponent2;
            gradient[1]-=value*dy;
            hessian[1]+=value*(dyy-1.);
            hessian[2]+=value*(dzz-3.);
            break;

        case ORB_4FYYZ:
            hessian[1]+=value*dz*(2.+dyy*(dyy-5.));
            function*=dy*(dyy-2.);
            hessian[5]+=function*(2.*exponent*dz*dz-1.);
            function*=dz;
            gradient[1]-=function;
            hessian[3]+=function*exponent2*dx;
            value*=dy*dy;
            function=value*(dzz-1.);
            gradient[2]-=function;
            hessian[4]+=function*exponent2*dx;
            value*=dz;
            function=value;
            value*=exponent2;
            gradient[0]-=value*dx;
            hessian[0]+=value*(dxx-1.);
            hessian[2]+=value*(dzz-3.);
            break;

        case ORB_4FXYY:
            hessian[1]+=value*dx*(2.+dyy*(dyy-5.));
            function*=dy*(dyy-2.);
            hessian[3]+=function*(dxx-1.);
            function*=dx;
            gradient[1]-=function;
            hessian[5]+=function*exponent2*dz;
            value*=dy*dy;
            function=value*(dxx-1.);
            gradient[0]-=function;
            hessian[4]+=function*exponent2*dz;
            value*=dx;
            function=value;
            value*=exponent2;
            gradient[2]-=value*dz;
            hessian[0]+=value*(dxx-3.);
            hessian[2]+=value*(dzz-1.);
            break;

        case ORB_4FXZZ:
            hessian[2]+=value*dx*(2.+dzz*(dzz-5.));
            function*=dz*(dzz-2.);
            hessian[4]+=function*(dxx-1.);
            function*=dx;
            gradient[2]-=function;
            hessian[5]+=function*exponent2*dy;
            value*=dz*dz;
            function=value*(dxx-1.);
            gradient[0]-=function;
            hessian[3]+=function*exponent2*dy;
            value*=dx;
            function=value;
            value*=exponent2;
            gradient[1]-=value*dy;
            hessian[0]+=value*(dxx-3.);
            hessian[1]+=value*(dyy-1.);
            break;

        case ORB_4FYZZ:
            hessian[2]+=value*dy*(2.+dzz*(dzz-5.));
            function*=dz*(dzz-2.);
            hessian[5]+=function*(dyy-1.);
            function*=dy;
            gradient[2]-=function;
            hessian[4]+=function*exponent2*dx;
            value*=dz*dz;
            function=value*(dyy-1.);
            gradient[1]-=function;
            hessian[3]+=function*exponent2*dx;
            value*=dy;
            function=value;
            value*=exponent2;
            gradient[0]-=value*dx;
            hessian[0]+=value*(dxx-1.);
            hessian[1]+=value*(dyy-3.);
            break;

        case ORB_4FXYZ:
            function*=dx*dy*dz;
            gradient[0]+=value*dy*dz*(1.-dxx);
            gradient[1]+=value*dx*dz*(1.-dyy);
            gradient[2]+=value*dx*dy*(1.-dzz);
            hessian[3]+=value*dz*(dxx-1.)*(dyy-1.);
            hessian[4]+=value*dy*(dxx-1.)*(dzz-1.);
            hessian[5]+=value*dx*(dyy-1.)*(dzz-1.);
            value*=exponent2*dx*dy*dz;
            hessian[0]+=value*(dxx-3.);
            hessian[1]+=value*(dyy-3.);
            hessian[2]+=value*(dzz-3.);
            break;
    }

    return function;
}

/* ************************************************************************** */
// The function evaluatingMetricOnMesh calculates at the vertices of the
// structure pointed by pMesh the metric associated to every product of
// orbitals given in the pChemicalSystem structure. It has the Parameters*, the
// Mesh* and the ChemicalSystem* variables (both defined in main.h) as input
// arguments and it returns one on sucess, otherwise zero is returned
/* ************************************************************************** */
int evaluatingMetricOnMesh(Parameters* pParameters, Mesh* pMesh,
                                                ChemicalSystem* pChemicalSystem)
{
    int i=0, j=0, k=0, l=0, nVer=0, nGauss=0, nMorb=0, info=0;
    double value=0., lambda=0., hessian[9]={0.}, eigenvalues[3]={0.};
    double *pHessian=&(hessian[0]), *pEigenvalues=&(eigenvalues[0]);
    double functionI=0., functionJ=0., gradientI[3]={0.}, gradientJ[3]={0.};
    double hessianI[6]={0.}, hessianJ[6]={0.};
    MolecularOrbital *pMolecularOrbitalI=NULL, *pMolecularOrbitalJ=NULL;
    Point *pPoint=NULL;

    // Check that the input variables are not pointing to NULL
    if (pParameters==NULL || pMesh==NULL || pChemicalSystem==NULL)
    {
        PRINT_ERROR("In evaluatingMetricOnMesh: one of the input ");
        fprintf(stderr,"variable pParameters=%p, ",(void*)pParameters);
        fprintf(stderr,"pMesh=%p, or ",(void*)pMesh);
        fprintf(stderr,"pChemicalSystem=%p ",(void*)pChemicalSystem);
        fprintf(stderr,"does not have a valid address.\n");
        return 0;
    }

    // The mesh must be made of tetraahedra in order to compute the metric
    if (pParameters->opt_mode<=0)
    {
        PRINT_ERROR("In evaluatingMetricOnMesh: the opt_mode variable ");
        fprintf(stderr,"(=%d) of the structure pointed ",pParameters->opt_mode);
        fprintf(stderr,"by pParameters should be a positive integer.\n");
        return 0;
    }

    // Set the number of vertices, molecular orbitals and Gaussian primitives
    nVer=pMesh->nver;
    nMorb=pChemicalSystem->nmorb;
    nGauss=pChemicalSystem->ngauss;
    if (nVer<1 || nMorb<1 || nGauss<1)
    {
        PRINT_ERROR("In evaluatingMetricOnMesh: the variables ");
        fprintf(stderr,"pMesh->nver=%d, pChemicalSystem->nmorb=%d ",nVer,nMorb);
        fprintf(stderr,"or pChemicalSystem->ngauss=%d must be ",nGauss);
        fprintf(stderr,"positive integers.\n");
        return 0;
    }
    if (pMesh->pver==NULL || pChemicalSystem->pmorb==NULL)
    {
        PRINT_ERROR("In evaluatingMetricOnMesh: one of the input ");
        fprintf(stderr,"variable pMesh->pver=%p, or",(void*)pMesh->pver);
        fprintf(stderr,"pChemicalSystem->pmorb=");
        fprintf(stderr,"%p does not have a ",(void*)pChemicalSystem->pmorb);
        fprintf(stderr,"valid address.\n");
        return 0;
    }

    if (pParameters->opt_mode!=1)
    {
        fprintf(stdout,"\nEvaluating the metric of molecular orbitals ");
        fprintf(stdout,"on mesh.\n");
        if (pParameters->verbose)
        {
            fprintf(stdout,"met_err=%lf ",pParameters->met_err);
            fprintf(stdout,"met_min=%lf ",pParameters->met_min);
            fprintf(stdout,"met_max=%lf\n",pParameters->met_max);
        }
    }
    else
    {
        if (pParameters->verbose)
        {
            fprintf(stdout,"\nEvaluating the metric of molecular orbitals ");
            fprintf(stdout,"on mesh.\n"); 
        }
        else
        {
            fprintf(stdout,"45 %% done.\n");
        }
    }

    // Initializing the metric to pParameters->met_max at each mesh vertex
    for (i=0; i<nVer; i++)
    {
        pMesh->pver[i].value=pParameters->met_max;
    }

    for (i=0; i<nMorb; i++)
    {
        pMolecularOrbitalI=&pChemicalSystem->pmorb[i];
        for (j=0; j<i; j++)
        {
            pMolecularOrbitalJ=&pChemicalSystem->pmorb[j];
            if (pParameters->orb_rhf)
            {
                info=(pMolecularOrbitalI->spin==1 &&
                                                   pMolecularOrbitalJ->spin==1);
            }
            else
            {
                info=(pMolecularOrbitalI->spin==pMolecularOrbitalJ->spin);
            }

            // Case where Orbitals i and j have different spin (and take into
            // account the case of restricted Hartree-Fock spin repetitions)
            if (!info)
            {
                continue;
            }

            // Point evaluation of the Hessian matrix of Orbital(i)*Orbital(j)
            // (stored as 0->xx 1->yy 2->zz 3->xy,yx 4->xz,zx 5->yz,zy)
            for (k=0; k<nVer; k++)
            {
                pPoint=&pMesh->pver[k];

                // Re-initialization of the function, gradient and Hessian
                functionI=0.;
                functionJ=0.;
                for (l=0; l<3; l++)
                {
                    gradientI[l]=0.;
                    gradientJ[l]=0.;
                    hessianI[l]=0.;
                    hessianJ[l]=0.;
                    hessian[l]=0.;
                    eigenvalues[l]=0.;
                }
                for (l=3; l<6; l++)
                {
                    hessianI[l]=0.;
                    hessianJ[l]=0.;
                    hessian[l]=0.;
                }
                for (l=6; l<9; l++)
                {
                    hessian[l]=0.;
                }

                // Evaluating function, gradient and Hessian matrix for
                // Orbital i and j depending of the type of primitives
                for (l=0; l<nGauss; l++)
                {
                    functionI+=evaluatingHessianAtVertices(pChemicalSystem,
                                                           pMolecularOrbitalI,
                                                           pPoint,gradientI,
                                                           hessianI,l);
                    functionJ+=evaluatingHessianAtVertices(pChemicalSystem,
                                                           pMolecularOrbitalJ,
                                                           pPoint,gradientJ,
                                                           hessianJ,l);
                }
                // Evaluating Hessian of Orb(i)*Orb(j) for computing metric
                hessian[0]=hessianI[0]*functionJ+2.*gradientI[0]*gradientJ[0]
                                                         +functionI*hessianJ[0];
                hessian[4]=hessianI[1]*functionJ+2.*gradientI[1]*gradientJ[1]
                                                         +functionI*hessianJ[1];
                hessian[8]=hessianI[2]*functionJ+2.*gradientI[2]*gradientJ[2]
                                                         +functionI*hessianJ[2];
                hessian[1]=hessianI[3]*functionJ+gradientI[0]*gradientJ[1]
                               +gradientI[1]*gradientJ[0]+functionI*hessianJ[3];
                hessian[2]=hessianI[4]*functionJ+gradientI[0]*gradientJ[2]
                               +gradientI[2]*gradientJ[0]+functionI*hessianJ[4];
                hessian[5]=hessianI[5]*functionJ+gradientI[1]*gradientJ[2]
                               +gradientI[2]*gradientJ[1]+functionI*hessianJ[5];

                // Diagonalize using lapacke library (lapack interface for c)
                // LAPACKE_dsyev returns zero if successfull; we refer to
                // diagonalizeOverlapMatrix in optimization.c file for details
                info=LAPACKE_dsyev(LAPACK_COL_MAJOR,'N','L',3,pHessian,3,
                                                                  pEigenvalues);
                if (info)
                {
                    PRINT_ERROR("In evaluatingMetricOnMesh: dsyev function ");
                    fprintf(stderr,"in Lapacke library returned %d ",info);
                    fprintf(stderr,"instead of zero.\n");
                    return 0;
                }

                // Get the normalized highest eigenvalues
                lambda=1./(pParameters->met_max*pParameters->met_max);
                for (l=0; l<3; l++)
                {
                    value=MET_CST*DEF_ABS(eigenvalues[l])/pParameters->met_err;
                    if (value>lambda)
                    {
                        lambda=value;
                    }
                }

                // Computing metric (note that initial values were set to
                // pParameters->met_max; upper bound was set for initial lamba)
                value=DEF_MAX(1./sqrt(lambda),pParameters->met_min);
                if (value<pPoint->value)
                {
                    pPoint->value=value;
                }
            }
        }

        // We now treat the case where i=j
        for (k=0; k<nVer; k++)
        {
            pPoint=&pMesh->pver[k];

            // Re-initialization of the function, gradient and Hessian matrix
            functionI=0.;
            for (l=0; l<3; l++)
            {
                gradientI[l]=0.;
                hessianI[l]=0.;
                hessian[l]=0.;
                eigenvalues[l]=0.;
            }
            for (l=3; l<6; l++)
            {
                hessianI[l]=0.;
                hessian[l]=0.;
            }
            for (l=6; l<9; l++)
            {
                hessian[l]=0.;
            }

            for (l=0; l<nGauss; l++)
            {
                // Evaluating function, gradient and Hessian for Orbital i
                // (stored as 0->xx 1->yy 2->zz 3->xy,yx 4->xz,zx 5->yz,zy)
                functionI+=evaluatingHessianAtVertices(pChemicalSystem,
                                                       pMolecularOrbitalI,
                                                       pPoint,gradientI,
                                                       hessianI,l);
            }

            // Evaluating Hessian of Orb(i)^2 for computing metric
            hessian[0]=2.*(hessianI[0]*functionI+gradientI[0]*gradientI[0]);
            hessian[4]=2.*(hessianI[1]*functionI+gradientI[1]*gradientI[1]);
            hessian[8]=2.*(hessianI[2]*functionI+gradientI[2]*gradientI[2]);
            hessian[1]=2.*(hessianI[3]*functionI+gradientI[0]*gradientI[1]);
            hessian[2]=2.*(hessianI[4]*functionI+gradientI[0]*gradientI[2]);
            hessian[5]=2.*(hessianI[5]*functionI+gradientI[1]*gradientI[2]);

            // Diagonalize using lapacke library (lapack interface for c)
            // LAPACKE_dsyev returns zero if successfull; we refer to
            // diagonalizeOverlapMatrix in optimization.c file for details
            info=LAPACKE_dsyev(LAPACK_COL_MAJOR,'N','L',3,pHessian,3,
                                                                  pEigenvalues);
            if (info)
            {
                PRINT_ERROR("In evaluatingMetricOnMesh: dsyev function ");
                fprintf(stderr,"in Lapacke library returned %d ",info);
                fprintf(stderr,"instead of zero.\n");
                return 0;
            }

            // Get the normalized highest eigenvalues
            lambda=1./(pParameters->met_max*pParameters->met_max);
            for (l=0; l<3; l++)
            {
                value=MET_CST*DEF_ABS(eigenvalues[l])/pParameters->met_err;
                if (value>lambda)
                {
                    lambda=value;
                }
            }

            // Computing the metric (note that the initial values were set to
            // pParameters->met_max; upper bound was set for the initial lamba)
            value=DEF_MAX(1./sqrt(lambda),pParameters->met_min);
            if (value<(pPoint->value))
            {
                pPoint->value=value;
            }
        }
    }

    return 1;
}

/* ************************************************************************** */
// The function writingSolFile writes the values stored in pMesh->pver[i].value,
// into a *.sol file whose name is the one stored in pParameters->name_mesh but
// where the *.mesh extension has been replaced by the *.sol one. It has the
// Parameters* and Mesh* variables (both defined in the main.h) as input
// arguments and it returns zero if an error occurred, otherwise one on success
/* ************************************************************************** */
int writingSolFile(Parameters* pParameters, Mesh* pMesh)
{
    size_t lengthName=0;
    char *fileLocation=NULL;
    int i=0, iMax=0;
    FILE *solFile=NULL;

    // Check that the input variables are not pointing to NULL
    if (pParameters==NULL || pMesh==NULL)
    {
        PRINT_ERROR("In writingSolFile: one of the input variables ");
        fprintf(stderr,"pParameters=%p or ",(void*)pParameters);
        fprintf(stderr,"pMesh=%p is not a valid address.\n",(void*)pMesh);
        return 0;
    }

    if (getMeshFormat(pParameters->name_mesh,pParameters->name_length)!=1)
    {
        PRINT_ERROR("In writingSolFile: getMeshFormat function did not ");
        fprintf(stderr,"return one, which was the expected value here.\n");
        return 0;
    }

    // Allocate memory for fileLocation and checked if it worked
    // calloc function returns a pointer to allocated memory, otherwise NULL
    lengthName=pParameters->name_length;
    fileLocation=(char*)calloc(lengthName,sizeof(char));
    if (fileLocation==NULL)
    {
        PRINT_ERROR("In writingSolFile: could not allocate memory for the ");
        fprintf(stderr,"(local) char* fileLocation variable.\n");
        return 0;
    }

    // strncpy returns a pointer to the string (not used here)
    // strlen function returns the length of the string not including the '\0'
    strncpy(fileLocation,pParameters->name_mesh,lengthName);
    lengthName=strlen(fileLocation);
    fileLocation[lengthName-5]='.';
    fileLocation[lengthName-4]='s';
    fileLocation[lengthName-3]='o';
    fileLocation[lengthName-2]='l';
    fileLocation[lengthName-1]='\0';

    // Opening *.sol file (warning: reset and overwrite file if already exists)
    // fopen returns a FILE pointer on success, otherwise NULL is returned
    if (pParameters->opt_mode!=1 || pParameters->verbose)
    {
        fprintf(stdout,"\nOpening %s file. ",fileLocation);
    }
    solFile=fopen(fileLocation,"w+");
    if (solFile==NULL)
    {
        PRINT_ERROR("In writingSolFile: we were not able to open and write ");
        fprintf(stderr,"into the %s file",fileLocation);
        free(fileLocation);
        fileLocation=NULL;
        return 0;
    }
    if (pParameters->opt_mode!=1 || pParameters->verbose)
    {
        fprintf(stdout,"Start writing solution. ");
    }

    // Writing according to the *.sol format: MeshVersionFormated (1=single,
    // 2=double precision), Dimension (2 or 3), SolAtVertices numberOfSolution
    // typeOfSolution (1=scalar, 2=vector, 3=symmetric tensor)
    iMax=pMesh->nver;
    if (iMax<1)
    {
        PRINT_ERROR("In writingMeshFile: the number of mesh vertices ");
        fprintf(stderr,"(=%d) should be a positive integer.\n",iMax);
        closeTheFile(&solFile);
        free(fileLocation);
        fileLocation=NULL;
        return 0;
    }
    fprintf(solFile,"MeshVersionFormatted 2\n\nDimension 3\n\n");
    fprintf(solFile,"SolAtVertices\n%d\n1 1 \n\n",pMesh->nver);

    if (pMesh->pver==NULL)
    {
        PRINT_ERROR("In writingMeshFile: the pMesh->pver variable should not ");
        fprintf(stderr,"be pointing to the %p address.\n",(void*)pMesh->pver);
        closeTheFile(&solFile);
        free(fileLocation);
        fileLocation=NULL;
        return 0;
    }
    for (i=0; i<iMax; i++)
    {
        fprintf(solFile,"%.8le \n",pMesh->pver[i].value);
    }
    fprintf(solFile,"\nEnd");

    // Closing the *.sol file: fclose function returns zero if the input FILE*
    // variable is successfully closed, otherwise EOF (end-of-file) is returned
    if (fclose(solFile))
    {
        PRINT_ERROR("In writingSolFile: the ");
        fprintf(stderr,"%s file has not been closed properly.\n",fileLocation);
        solFile=NULL;
        free(fileLocation);
        fileLocation=NULL;
        return 0;
    }
    solFile=NULL;
    if (pParameters->opt_mode!=1 || pParameters->verbose)
    {
        fprintf(stdout,"Closing file.\n");
    }

    // Free the memory allocated for fileLocation
    free(fileLocation);
    fileLocation=NULL;

    return 1;
}

/* ************************************************************************** */
// The function computeMetric evaluates the molecular orbitals' metric on the
// mesh and save it in a metric.sol file. It has the Parameters*, Mesh* and
// ChemicalSystem* (both defined in main.h) as input arguments and it returns
// zero if an error occurred, otherwise one is returned in case of success
/* ************************************************************************** */
int computeMetric(Parameters* pParameters, Mesh* pMesh,
                        ChemicalSystem* pChemicalSystem, int iterationInTheLoop)
{
    size_t lengthName=0;
    char* fileLocation=NULL;

    // Evaluate the orbitals' metric on mesh (NULL pointers are checked here)
    if (!evaluatingMetricOnMesh(pParameters,pMesh,pChemicalSystem))
    {
        PRINT_ERROR("In computeMetric: evaluatingMetricOnMesh function ");
        fprintf(stderr,"returned zero instead of one.\n");
        return 0;
    }

    // Check the iterationInTheLoopVariable variable
    if (iterationInTheLoop<0 || iterationInTheLoop>pParameters->iter_max)
    {
        PRINT_ERROR("In computeMetric: the input iterationInTheLoop variable ");
        fprintf(stderr,"(=%d) should be a non-negative ",iterationInTheLoop);
        fprintf(stderr,"integer not (strictly) greater than the maximal ");
        fprintf(stderr,"number of allowed iterations ");
        fprintf(stderr,"(=%d).\n",pParameters->iter_max);
        return 0;
    }

    // Check the name_mesh and name_length variables
    if (getMeshFormat(pParameters->name_mesh,pParameters->name_length)!=1)
    {
        PRINT_ERROR("In computeMetric: getMeshFormat function did not return ");
        fprintf(stderr,"one, which was the expected value here.\n");
        return 0;
    }

    // Check that the metric.mesh file does not exist. Warning: metric.mesh
    // and metric.sol cannot be used as names in the MPD program
    switch (initialFileExists("metric.mesh",15))
    {
        case -1:
            break;

        case 1:
            PRINT_ERROR("In computeMetric: 'metric.mesh' file cannot refer ");
            fprintf(stderr,"to a mesh name in the MPD program.\nPlease ");
            fprintf(stderr,"modify the name associated with the name_mesh ");
            fprintf(stderr,"keyword or if no such line exists in your ");
            fprintf(stderr,"(input) *.info file, check that it is not ");
            fprintf(stderr,"entitled 'metric.info'.\n");
            return 0;
            break;

        default:
            PRINT_ERROR("In computeMetric: initialFileExists function ");
            fprintf(stderr,"returned zero instead of (+/-)one.\n");
            return 0;
            break;
    }

    // Allocate memory for the fileLocation variables and check if it worked
    // calloc function returns a pointer to allocated memory, otherwise NULL
    lengthName=pParameters->name_length;
    fileLocation=(char*)calloc(lengthName,sizeof(char));
    if (fileLocation==NULL)
    {
        PRINT_ERROR("In computeMetric: could not allocate memory for the ");
        fprintf(stderr,"local char* fileLocation variable.\n");
        return 0;
    }

    // Store the name of the *.mesh file in fileLocation
    // strncpy function returns a pointer to the string (not used here)
    strncpy(fileLocation,pParameters->name_mesh,lengthName);

    // Temporary rename the *.mesh by metric.mesh
    if (!renameFileLocation(pParameters->name_mesh,pParameters->name_length,
                                                                 "metric.mesh"))
    {
        PRINT_ERROR("In computeMetric: renameFileLocation function returned ");
        fprintf(stderr,"zero instead of one.\n");
        free(fileLocation);
        fileLocation=NULL;
        return 0;
    }
    strncpy(pParameters->name_mesh,"metric.mesh",pParameters->name_length);

    // Save the metric values in a file entitled metric.sol
    if (!writingSolFile(pParameters,pMesh))
    {
        PRINT_ERROR("In computeMetric: writingSolFile function returned zero ");
        fprintf(stderr,"instead of one.\n");
        free(fileLocation);
        fileLocation=NULL;
        return 0;
    }

    // Vizualize the metric of the orbitals on the mesh
    if (pParameters->opt_mode!=1 && iterationInTheLoop)
    {
        if (pParameters->save_print>0)
        {
            if ((iterationInTheLoop-1)%pParameters->save_print==0 &&
                                                     pParameters->save_where==1)
            {
                if (!plotMeshWithMeditSoftware(pParameters))
                {
                    PRINT_ERROR("In computeMetric: plotMeshWithMeditSoftware ");
                    fprintf(stderr,"function returned zero instead of one.\n");
                    free(fileLocation);
                    fileLocation=NULL;
                    return 0;
                }
            }
        }
    }

    // Rename the *.mesh file as it was
    if (!renameFileLocation("metric.mesh",pParameters->name_length,
                                                                  fileLocation))
    {
        PRINT_ERROR("In computeMetric: renameFileLocation function returned ");
        fprintf(stderr,"zero instead of one.\n");
        free(fileLocation);
        fileLocation=NULL;
        return 0;
    }
    strncpy(pParameters->name_mesh,fileLocation,pParameters->name_length);

    // Free the memory allocated for fileLocation
    free(fileLocation);
    fileLocation=NULL;

    return 1;
}

/* ************************************************************************** */
// The function performLevelSetAdaptation computes the orbitals' metric and
// perform a mesh adaptation according to the level-set function (that must
// have been previously computed in a *.sol file associated with the .mesh) but
// also respecting the orbitals' metric. This is done by using the modified 
// version of the external mmg3d software, that must have been previously
// installed. It has the Parameters*, Mesh*, and ChemicalSystem* (both defined
// in main.h) and the int iterationInTheLoop variables as input arguments and
// it returns one on success otherwise zero is returned if an error is found
/* ************************************************************************** */
int performLevelSetAdaptation(Parameters* pParameters, Mesh* pMesh,
                              ChemicalSystem* pChemicalSystem,
                                                         int iterationInTheLoop)
{
    int sizeMemory=0;

    // Check that the input variables are not pointing to NULL
    if (pParameters==NULL || pMesh==NULL || pChemicalSystem==NULL)
    {
        PRINT_ERROR("In performLevelSetAdaptation: one of the input variable");
        fprintf(stderr,"pParameters=%p, ",(void*)pParameters);
        fprintf(stderr,"pMesh=%p, or ",(void*)pMesh);
        fprintf(stderr,"pChemicalSystem=%p ",(void*)pChemicalSystem);
        fprintf(stderr,"does not point to a valid address.\n");
        return 0;
    }

    if (pParameters->opt_mode==2 && iterationInTheLoop)
    {
        fprintf(stdout,"\nSTEP 4: COMPUTE THE METRIC RELATED TO THE ");
        fprintf(stdout,"CHEMISTRY OF THE MOLECULAR ORBITALS.\n");
    }
    else if (pParameters->opt_mode==4 && iterationInTheLoop)
    {
        fprintf(stdout,"\nSTEP 2: COMPUTE THE METRIC RELATED TO THE ");
        fprintf(stdout,"CHEMISTRY OF THE MOLECULAR ORBITALS.\n");
    }

    // Evaluate the metric associated with the molecular orbitals
    if (!computeMetric(pParameters,pMesh,pChemicalSystem,iterationInTheLoop))
    {
        PRINT_ERROR("In performLevelSetAdaptation: computeMetric function ");
        fprintf(stderr,"returned zero instead of one.\n");
        return 0;
    }

    if (pParameters->opt_mode==2 && iterationInTheLoop)
    {
        fprintf(stdout,"\nSTEP 5: GET NEW DOMAIN BY ADAPTING MESH TO BOTH ");
        fprintf(stdout,"ADVECTED LEVEL-SET AND METRIC.\n");
    }
    else if (pParameters->opt_mode==4 && iterationInTheLoop)
    {
        fprintf(stdout,"\nSTEP 3: GET NEW DOMAIN BY ADAPTING MESH TO BOTH ");
        fprintf(stdout,"ADVECTED LEVEL-SET AND METRIC.\n");
    }

    // Free the memory allocated for the mesh
    if (pParameters->opt_mode!=1 || pParameters->verbose)
    {
        sizeMemory=sizeof(Mesh)+(pMesh->nver)*sizeof(Point);
        sizeMemory+=(pMesh->nnorm+pMesh->ntan)*sizeof(Vector);
        sizeMemory+=(pMesh->nedg)*sizeof(Edge);
        sizeMemory+=(pMesh->ntri)*sizeof(Triangle);
        sizeMemory+=(pMesh->ntet)*sizeof(Tetrahedron);
        fprintf(stdout,"\nCleaning the allocated memory ");
        fprintf(stdout,"(%d,%d Mo) for ",sizeMemory/1000000,sizeMemory%1000000);
        fprintf(stdout,"a mesh adaptation according to both the level-set ");
        fprintf(stdout,"interface geometry and molecular orbitals ");
        fprintf(stdout,"chemistry.\n");
    }
    freeMeshMemory(pMesh);

    // Perform a mesh adaptation according to both level set and metric
    // Warning: mmg3d software must have been previously installed
    if (!adaptMeshWithMmg3dSoftware(pParameters,"ls"))
    {
        PRINT_ERROR("In performLevelSetAdaptation: ");
        fprintf(stderr,"adaptMeshWithMmg3dSoftware function returned zero ");
        fprintf(stderr,"instead of one.\n");;
        return 0;
    }

    // Read the new *.mesh file
    if (!readMeshFileAndAllocateMesh(pParameters,pMesh))
    {
        PRINT_ERROR("In performLevelSetAdaptation: ");
        fprintf(stderr,"readMeshFileAndAllocateMesh function returned zero ");
        fprintf(stderr,"instead of one.\n");
        return 0;
    }

    // Update the parameters related to the computational box
    if (!updateDiscretizationParameters(pParameters,pMesh))
    {
        PRINT_ERROR("In performLevelSetAdaptation: ");
        fprintf(stderr,"updateDiscretizationParameters function returned ");
        fprintf(stderr,"zero instead of one.\n");
        return 0;
    }

    // Vizualize the new mesh
    if (pParameters->opt_mode!=1 && iterationInTheLoop)
    {
        if (pParameters->save_print>0)
        {
            if ((iterationInTheLoop-1)%pParameters->save_print==0 &&
                                                     pParameters->save_where==7)
            {
                if (!plotMeshWithMeditSoftware(pParameters))
                {
                    PRINT_ERROR("In performLevelSetAdaptation: ");
                    fprintf(stderr,"plotMeshWithMeditSoftware function ");
                    fprintf(stderr,"returned zero instead of one.\n");
                    return 0;
                }
            }
        }
    }

    return 1;
}

/* ************************************************************************** */
// The function adaptMesh prepares the mesh for the optimization loop. If the
// mesh already contains an initial guess for the MPD domain (internal faces
// labelled 10, elements labelled 3 for those belonging to the interior of the
// domain otherwise labelled 2), nothing is done by the adaptMesh function
// (apart from setting up the Adjacency structures for hexahedral meshes). In
// the case where no internal domain is found inside de mesh of the
// computational box, an initial sphere or cube is built by using the ls_*
// parameters. In the case of tetrahedral meshes, the mesh is adapted to the
// chemistry of the molecular orbitals and also to the geometry of the level-set
// function interface associated with the initial domain. It has the
// Parameters*, Mesh* and ChemicalSystem* (both defined in main.h) as input
// arguments and it returns zero if an error occurred, otherwise one is
// returned in case of success
/* ************************************************************************** */
int adaptMesh(Parameters* pParameters, Mesh* pMesh,
                                                ChemicalSystem* pChemicalSystem)
{
    int sizeMemory=0, optMode=0;

    switch (initialDomainInMeshExists(pParameters,pMesh))
    {
        // If a level-set structure is already encoded in a hexahedral mesh,
        // create the adjacency table for the faces of the internal domain
        // For tetrahedral meshes, nothing has to be done
        case 2:
            if (pParameters->opt_mode<=0)
            {
                if (pParameters->verbose)
                {
                    fprintf(stdout,"\nInternal domain detected in the mesh. ");
                    fprintf(stdout,"Adding boundary quadrilaterals and build ");
                    fprintf(stdout,"the Adjacency structures. ");
                }
                if (!getLevelSetQuadrilaterals(pParameters,pMesh))
                {
                    PRINT_ERROR("In adaptMesh: getLevelSetQuadrilaterals ");
                    fprintf(stderr,"function returned zero instead of one.\n");
                    return 0;
                }
                if (pParameters->verbose)
                {
                    fprintf(stdout,"Procedure end normally.\n");
                }
            }
            break;

        case 1:
            if (pParameters->opt_mode<=0)
            {
                if (pParameters->verbose)
                {
                    fprintf(stdout,"\nInternal domain detected in the mesh. ");
                    fprintf(stdout,"Setting up the Adjacency structures for ");
                    fprintf(stdout,"the boundary quadrilaterals. ");
                }
                if (!initializeAdjacency(pParameters,pMesh))
                {
                    PRINT_ERROR("In adaptMesh: initializeAdjacency function ");
                    fprintf(stderr,"returned zero instead of one.\n");
                    return 0;
                }
                if (pParameters->verbose)
                {
                    fprintf(stdout,"Procedure ended normally.\n");
                }
            }
            break;

        case -1:
            if (pParameters->opt_mode<=0)
            {
                // Evaluate the initial level-set function on mesh
                if (!initializeLevelSetFunction(pParameters,pMesh))
                {
                    PRINT_ERROR("In adaptMesh: initializeLevelSetFunction ");
                    fprintf(stderr,"function returned zero instead of one.\n");
                    return 0;
                }

                // Modify the pMesh structure so that it contains the
                // quadrilaterals corresponding to the level-set interface
                if (!getLevelSetQuadrilaterals(pParameters,pMesh))
                {
                    PRINT_ERROR("In adaptMesh: getLevelSetQuadrilaterals ");
                    fprintf(stderr,"function returned zero instead of one.\n");
                    return 0;
                }

                // Since we have added some quadrilaterals, save the new mesh
                if (!writingMeshFile(pParameters,pMesh))
                {
                    PRINT_ERROR("In adaptMesh: writingMeshFile function ");
                    fprintf(stderr,"returned zero instead of one.\n");
                    return 0;
                }
            }
            else
            {
                // This trick is used not to print the content of mmg3d output
                // (see adaptMeshWithMmg3dSoftware function in main.c file)
                if (pParameters->opt_mode==1)
                {
                    optMode=1;
                    pParameters->opt_mode=3;
                }

                // Evaluate the metric associated with the orbitals
                if (!evaluatingMetricOnMesh(pParameters,pMesh,pChemicalSystem))
                {
                    PRINT_ERROR("In adaptMesh: evaluatingMetricOnMesh ");
                    fprintf(stderr,"function returned zero instead of one.\n");
                    return 0;
                }

                // Save metric values in a *.sol file associated with *.mesh one
                if (!writingSolFile(pParameters,pMesh))
                {
                    PRINT_ERROR("In adaptMesh: writingSolFile function ");
                    fprintf(stderr,"returned zero instead of one.\n");
                    return 0;
                }

                // Free the memory allocated for the mesh
                sizeMemory=sizeof(Mesh)+(pMesh->nver)*sizeof(Point);
                sizeMemory+=(pMesh->nnorm+pMesh->ntan)*sizeof(Vector);
                sizeMemory+=(pMesh->nedg)*sizeof(Edge);
                sizeMemory+=(pMesh->ntri)*sizeof(Triangle);
                sizeMemory+=(pMesh->ntet)*sizeof(Tetrahedron);
                fprintf(stdout,"Cleaning the allocated memory ");
                fprintf(stdout,"(%d,%d ",sizeMemory/1000000,sizeMemory%1000000);
                fprintf(stdout,"Mo) for a first mesh adaptation according to ");
                fprintf(stdout,"the orbitals' metric.\n");
                freeMeshMemory(pMesh);

                // Perform a first adaptation according to the orbitals' metric
                // Warning: mmg3d software must have been previously installed
                if (!adaptMeshWithMmg3dSoftware(pParameters,"met"))
                {
                    PRINT_ERROR("In adaptMesh: adaptMeshWithMmg3dSoftware ");
                    fprintf(stderr,"function returned zero instead of one.\n");;
                    return 0;
                }

                // Read the new *.mesh file
                if (!readMeshFileAndAllocateMesh(pParameters,pMesh))
                {
                    PRINT_ERROR("In performLevelSetAdaptation: ");
                    fprintf(stderr,"readMeshFileAndAllocateMesh function ");
                    fprintf(stderr,"returned zero instead of one.\n");
                    return 0;
                }

                // Update the parameters related to the computational box
                if (!updateDiscretizationParameters(pParameters,pMesh))
                {
                    PRINT_ERROR("In performLevelSetAdaptation: ");
                    fprintf(stderr,"updateDiscretizationParameters function ");
                    fprintf(stderr,"returned zero instead of one.\n");
                    return 0;
                }

                // Evaluate the initial level-set function on mesh
                if (!initializeLevelSetFunction(pParameters,pMesh))
                {
                    PRINT_ERROR("In adaptMesh: initializeLevelSetFunction ");
                    fprintf(stderr,"function returned zero instead of one.\n");
                    return 0;
                }

                // Save the level-set in a *.sol file associated with *.mesh one
                if (!writingSolFile(pParameters,pMesh))
                {
                    PRINT_ERROR("In adaptMesh: writingSolFile function ");
                    fprintf(stderr,"returned zero instead of one.\n");
                    return 0;
                }

                // Perform a 2nd adaptation according to the level-set function
                // Warning: mmg3d software must have been previously installed
                if (!performLevelSetAdaptation(pParameters,pMesh,
                                                             pChemicalSystem,0))
                {
                    PRINT_ERROR("In adaptMesh: performLevelSetAdaptation ");
                    fprintf(stderr,"function returned zero instead of one.\n");;
                    return 0;
                }

                // This trick is used not to print the content of mmg3d output
                // (see adaptMeshWithMmg3dSoftware function in main.c file)
                if (pParameters->opt_mode==3 && optMode==1)
                {
                    pParameters->opt_mode=1;
                }
            }

            // If pParameters->save_print==0, graphic and prompt mode is off
            if (pParameters->save_print)
            {
                // Vizualize the mesh of the initial domain with medit (Warning:
                // the medit software must have been previously installed)
                if (!plotMeshWithMeditSoftware(pParameters))
                {
                    PRINT_ERROR("In adaptMesh: plotMeshWithMeditSoftware ");
                    fprintf(stderr,"function returned zero instead of one.\n");
                    return 0;
                }
            }
            break;

        default:
        PRINT_ERROR("In adaptMesh: initialDomainInMeshExists function ");
        fprintf(stderr,"returned zero instead of (+/-)one.\n");
        return 0;
        break;
    }

    return 1;
}

