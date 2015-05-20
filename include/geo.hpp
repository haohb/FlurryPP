/*!
 * \file geo.hpp
 * \brief Header file for geometry class
 *
 * \author - Jacob Crabill
 *           Aerospace Computing Laboratory (ACL)
 *           Aero/Astro Department. Stanford University
 *
 * \version 0.0.1
 *
 * Flux Reconstruction in C++ (Flurry++) Code
 * Copyright (C) 2014 Jacob Crabill.
 *
 */
#pragma once

#include <string>
#include <vector>

#include "global.hpp"

#include "ele.hpp"
#include "input.hpp"
#include "solver.hpp"
#include "face.hpp"

class geo
{
public:
  geo();

  /* === Primay setup routines === */

  //! Setup the geomery using input parameters
  void setup(input* params);

  //! Take the basic connectivity data and generate the rest
  void processConnectivity();

  //! Create the elements and faces needed for the simulation
  void setupElesFaces(vector<ele> &eles, vector<face*> &faces);

  /* === Helper Routines === */

  //! Read essential connectivity from a Gmsh mesh file
  void readGmsh(string fileName);

  //! Create a simple Cartesian mesh from input parameters
  void createMesh();

  void createQuadMesh(); // or lump into createMesh()
  void createTriMesh();

  //! Get the reference-domain location of the solution points for the given element & polynomial order
  vector<point> getLocSpts(int eType, int order);

  //! Get the reference-domain location of the flux points for the given element & polynomial order
  vector<point> getLocFpts(int eType, int order);

  //! Get the point locations of the requested type (i.e. Gauss, Lobatto) for the given order
  vector<double> getPts1D(string ptsType, int order);

  int nDims, nFields;
  int nEles, nVerts, nEdges, nFaces, nBndFaces, nMpiFaces;
  int nBounds;  //! Number of boundaries

  void partitionMesh(void);

private:

  input *params;

  // Basic [essential] Connectivity Data
  matrix<int> c2v;
  vector<point> xv;

  // Additional Connectivity Data
  matrix<int> c2e, c2b, e2c, e2v, v2e, v2v, v2c;
  vector<int> v2nv, v2nc, c2nv, c2ne, ctype;
  vector<int> intEdges, bndEdges, mpiEdges;
  vector<int> bcList;            //! List of boundary conditions for each boundary
  vector<int> bcType;            //! Boundary condition for each boundary edge
  matrix<int> bndPts;            //! List of node IDs on each boundary
  vector<int> nBndPts;           //! Number of points on each boudary
  vector<matrix<int> > bndFaces; //! List of nodes on each face (edge) on each boundary
  vector<int> nFacesPerBnd;         //! List of # of faces on each boundary
  matrix<int> mpiFaces;          //! List of nodes on each MPI face on processor
  vector<int> procR;             //! What processor lies to the 'right' of this face
  //map<string,int> bcNum;         //! Maps a boundary-condition string to its integer enum
  vector<bool> isBnd; // might want to change this to "int" and have it store WHICH boundary the face is on (-1 for internal)

  /* --- MPI-Related Varialbes (global vs. local data) --- */
  matrix<int> c2v_g;
  vector<point> xv_g;
  vector<int> ic2icg;  //! Local cell to global cell index
  vector<int> iv2ivg;  //! Local vertex to global vertex index
  vector<int> ctype_g, c2ne_g, c2nv_g;
  matrix<int> bndPts_g;
  vector<int> nBndPts_g;
  int nEles_g, nVerts_g;

  //! Match up pairs of periodic boundary faces
  void processPeriodicBoundaries(void);

  //! Check if two given periodic edges match up
  bool checkPeriodicFaces(int *edge1, int *edge2);
};
