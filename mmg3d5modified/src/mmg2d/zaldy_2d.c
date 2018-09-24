/* =============================================================================
**  This file is part of the mmg software package for the tetrahedral
**  mesh modification.
**  Copyright (c) Bx INP/Inria/UBordeaux/UPMC, 2004- .
**
**  mmg is free software: you can redistribute it and/or modify it
**  under the terms of the GNU Lesser General Public License as published
**  by the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  mmg is distributed in the hope that it will be useful, but WITHOUT
**  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
**  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
**  License for more details.
**
**  You should have received a copy of the GNU Lesser General Public
**  License and of the GNU General Public License along with mmg (in
**  files COPYING.LESSER and COPYING). If not, see
**  <http://www.gnu.org/licenses/>. Please read their terms carefully and
**  use this copy of the mmg distribution only if you accept them.
** =============================================================================
*/
/**
 * \file mmg2d/zaldy_2d.c
 * \brief Memory management.
 * \author Charles Dapogny (UPMC)
 * \author Cécile Dobrzynski (Bx INP/Inria/UBordeaux)
 * \author Pascal Frey (UPMC)
 * \author Algiane Froehly (Inria/UBordeaux)
 * \version 5
 * \copyright GNU Lesser General Public License.
 * \todo Doxygen documentation
 */
#include "mmg2d.h"


/* Create a new vertex in the mesh, and return its number */
int _MMG2D_newPt(MMG5_pMesh mesh,double c[2],int16_t tag) {
  MMG5_pPoint  ppt;
  int     curpt;

  if ( !mesh->npnil )  return(0);

  curpt = mesh->npnil;
  if ( mesh->npnil > mesh->np )  mesh->np = mesh->npnil;
  ppt   = &mesh->point[curpt];
  memcpy(ppt->c,c,2*sizeof(double));
  ppt->tag   &= ~MG_NUL;
  mesh->npnil = ppt->tmp;
  ppt->tmp    = 0;
  ppt->xp     = 0;
  ppt->tag = tag;

  return(curpt);
}

/* Delete a point in the mesh and update the garbage collector accordingly */
void _MMG2D_delPt(MMG5_pMesh mesh,int ip) {
  MMG5_pPoint   ppt;
  MMG5_pxPoint  pxp;

  ppt = &mesh->point[ip];
  if ( ppt->xp ) {
    pxp = &mesh->xpoint[ppt->xp];
    memset(pxp,0,sizeof(MMG5_xPoint));
  }

  memset(ppt,0,sizeof(MMG5_Point));
  ppt->tag    = MG_NUL;
  ppt->tmp    = mesh->npnil;

  mesh->npnil = ip;
  if ( ip == mesh->np )  mesh->np--;
}

/* Create a new edge in the mesh and return its address */
int _MMG5_newEdge(MMG5_pMesh mesh) {
  int     curiel;

  if ( !mesh->nanil ) {
    return(0);
  }
  curiel = mesh->nanil;
  if ( mesh->nanil > mesh->na )  mesh->na = mesh->nanil;
  mesh->nanil = mesh->edge[curiel].b;
  mesh->edge[curiel].b = 0;

  return(curiel);
}


void _MMG5_delEdge(MMG5_pMesh mesh,int iel) {
  MMG5_pEdge    pt;

  pt = &mesh->edge[iel];
  if ( !pt->a ) {
    fprintf(stdout,"  ## INVALID EDGE.\n");
    return;
  }
  memset(pt,0,sizeof(MMG5_Edge));
  pt->b = mesh->nanil;
  mesh->nanil = iel;
  if ( iel == mesh->na )  mesh->na--;
}

/* Create a new triangle in the mesh and return its address */
int _MMG2D_newElt(MMG5_pMesh mesh) {
  int     curiel;

  if ( !mesh->nenil ) {
    return(0);
  }
  curiel = mesh->nenil;
  if ( mesh->nenil > mesh->nt )  mesh->nt = mesh->nenil;
  mesh->nenil = mesh->tria[curiel].v[2];
  mesh->tria[curiel].v[2] = 0;
  mesh->tria[curiel].ref = 0;
  mesh->tria[curiel].base = 0;
  mesh->tria[curiel].edg[0] = 0;
  mesh->tria[curiel].edg[1] = 0;
  mesh->tria[curiel].edg[2] = 0;

  return(curiel);
}

/* Delete a triangle in the mesh and update the garbage collector accordingly */
int _MMG2D_delElt(MMG5_pMesh mesh,int iel) {
  MMG5_pTria    pt;
  int      iadr;

  pt = &mesh->tria[iel];
  if ( !MG_EOK(pt) ) {
    fprintf(stdout,"  ## INVALID ELEMENT.\n");
    return 0;
  }
  memset(pt,0,sizeof(MMG5_Tria));
  pt->v[2] = mesh->nenil;
  pt->qual = 0.0;
  iadr = (iel-1)*3 + 1;
  if ( mesh->adja )
    memset(&mesh->adja[iadr],0,3*sizeof(int));

  mesh->nenil = iel;
  if ( iel == mesh->nt )  mesh->nt--;
  return 1;
}


/* check if n elets available */
int _MMG5_getnElt(MMG5_pMesh mesh,int n) {
  int     curiel;

  if ( !mesh->nenil )  return(0);
  curiel = mesh->nenil;
  do {
    curiel = mesh->tria[curiel].v[2];
  }
  while (--n);

  return(n == 0);
}

/** memory repartition for the -m option */
int _MMG2D_memOption(MMG5_pMesh mesh) {
  long long  million = 1048576L,memtmp;
  int        ctri,npask,bytes;

  mesh->memMax = _MMG5_memSize();

  mesh->npmax = MG_MAX(1.5*mesh->np,_MMG2D_NPMAX);
  mesh->ntmax = MG_MAX(1.5*mesh->nt,_MMG2D_NEMAX);
  mesh->namax = mesh->na;
  mesh->xpmax  = M_MAX(0.1*mesh->xp,0.1*_MMG2D_NPMAX);

  if ( mesh->info.mem <= 0 ) {
    if ( mesh->memMax && (mesh->memMax >2000*million))
      /* maximal memory = 2Go */
      mesh->memMax = 2000*million;
    else {
      /* default value = 800 MB */
      printf(" ## Maximum memory set to default value: %d MB.\n",_MMG5_MEMMAX);
      mesh->memMax = _MMG5_MEMMAX*million;
    }
  }
  else {
    /* memory asked by user if possible, otherwise total physical memory */
    if ( (long long)(mesh->info.mem)*million > mesh->memMax && mesh->memMax ) {
      fprintf(stderr,"\n  ## Warning: %s: asking for %d MB of memory ",
              __func__,mesh->info.mem);
      fprintf(stderr,"when only %lld available.\n",mesh->memMax/million);
    }
    else {
      mesh->memMax= (long long)(mesh->info.mem)*million;
    }

    /* if asked memory is lower than default _MMG2D_NPMAX/_MMG2D_NTMAX we take lower values */
    ctri = 2;

    /* Euler-poincare: ne = 6*np; nt = 2*np; na = np/5 *
     * point+tria+edges+adjt+sol */
    bytes = sizeof(MMG5_Point) +  0.1*sizeof(MMG5_xPoint) +
      2*sizeof(MMG5_Tria) + 3*2*sizeof(int)
      + sizeof(MMG5_Sol) /*+ sizeof(Displ)*/
      + 0.2*sizeof(MMG5_Edge);

    /* init allocation need _MMG5_MEMMIN B (for mesh->namein/out and
     * sol->namein/out */
    npask = (int)((double)(mesh->info.mem*million-_MMG5_MEMMIN)/(double)bytes);

    mesh->npmax = MG_MIN(npask,mesh->npmax);
    mesh->ntmax = MG_MIN(ctri*npask,mesh->ntmax);
    mesh->namax = MG_MIN(ctri*npask,mesh->namax);
    mesh->xpmax = MG_MIN(0.1*npask,0.1*mesh->xp);

    /*check if the memory asked is enough to load the mesh*/
    if(mesh->np &&
       (mesh->npmax < mesh->np || mesh->ntmax < mesh->nt || mesh->namax < mesh->na) ){
      memtmp = (long long)mesh->np * bytes + _MMG5_MEMMIN;
      memtmp = MG_MAX(memtmp, ((long long)mesh->nt * bytes /ctri)+_MMG5_MEMMIN);
      memtmp = MG_MAX(memtmp, ((long long)mesh->na * bytes /ctri)+_MMG5_MEMMIN);
      mesh->memMax = memtmp+1;
      fprintf(stderr,"  ## Error: %s: asking for %d MB of memory ",
              __func__,mesh->info.mem);
      fprintf(stderr,"is not enough to load mesh. You need to ask %lld MB minimum\n",
              memtmp/million+1);
      return 0;
    }
    if(mesh->info.mem < _MMG5_MEMMIN + 1) {
      mesh->memMax =  _MMG5_MEMMIN + 1;
      fprintf(stderr,"\n  ## Error: %s: asking for %d MB of memory ",
              __func__,mesh->info.mem);
      fprintf(stderr,"is not enough to load mesh. You need to ask %d B minimum\n",
              _MMG5_MEMMIN + 1);
      return 0;
    }
  }

  if ( abs(mesh->info.imprim) > 4 || mesh->info.ddebug ) {
    fprintf(stdout,"  MAXIMUM MEMORY AUTHORIZED (MB)    %lld\n",
            mesh->memMax/million);
  }

  if ( abs(mesh->info.imprim) > 5 || mesh->info.ddebug ) {
    fprintf(stdout,"  _MMG2D_NPMAX    %d\n",mesh->npmax);
    fprintf(stdout,"  _MMG2D_NTMAX    %d\n",mesh->ntmax);
  }

  return 1;
}

/* allocate main structure */
int MMG2D_zaldy(MMG5_pMesh mesh) {
  int     k;

  if ( !_MMG2D_memOption(mesh) )  return 0;

  _MMG5_ADD_MEM(mesh,(mesh->npmax+1)*sizeof(MMG5_Point),"initial vertices",
                printf("  Exit program.\n");
                return 0);
  _MMG5_SAFE_CALLOC(mesh->point,mesh->npmax+1,MMG5_Point,0);

  if ( mesh->xp ) {
    _MMG5_ADD_MEM(mesh,(mesh->xpmax+1)*sizeof(MMG5_xPoint),"initial xpoint",return(0));
    _MMG5_SAFE_CALLOC(mesh->xpoint,mesh->xpmax+1,MMG5_xPoint,0);
    memset(&mesh->xpoint[0],0,sizeof(MMG5_xPoint));
  }
  _MMG5_ADD_MEM(mesh,(mesh->ntmax+1)*sizeof(MMG5_Tria),"initial triangles",return(0));
  _MMG5_SAFE_CALLOC(mesh->tria,mesh->ntmax+1,MMG5_Tria,0);
  memset(&mesh->tria[0],0,sizeof(MMG5_Tria));

  if ( mesh->na ) {
    _MMG5_ADD_MEM(mesh,(mesh->namax+1)*sizeof(MMG5_Edge),"initial edges",return(0));
    _MMG5_SAFE_CALLOC(mesh->edge,(mesh->namax+1),MMG5_Edge,0);
  }

  /* keep track of empty links */
  mesh->npnil = mesh->np + 1;
  mesh->nenil = mesh->nt + 1;


  for (k=mesh->npnil; k<mesh->npmax-1; k++) {
    /* Set tangent field of point to 0 */
    mesh->point[k].n[0] = 0;
    mesh->point[k].n[1] = 0;
    mesh->point[k].n[2] = 0;
    /* link */
    mesh->point[k].tmp  = k+1;
  }

  for (k=mesh->nenil; k<mesh->ntmax-1; k++)
    mesh->tria[k].v[2] = k+1;

  return(1);
}
