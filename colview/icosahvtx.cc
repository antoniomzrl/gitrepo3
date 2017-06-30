#include "colv.hh"

vector <vec3> vertices, normals;

void AddTriangle(vec3& a, vec3& b, vec3& c) {
  vec3 n = (a +b +c) /(3.0f);
  
  vertices.push_back(a);
  vertices.push_back(b);
  vertices.push_back(c);

  normals.push_back(n);
  normals.push_back(n);
  normals.push_back(n);
}


// Project the mid-point of a triangle edge on the sphere
vec3 midpointOnSphere(vec3& a, vec3& b) {
  vec3 midpoint = a + b; //(a + b) * 0.5f;
  //return midpoint /midpoint.length();
  return normalize(midpoint);
}


/* Draw triangle OR subdivide */
void AddIcosahedron(int MaxLevel, int MyLevel , vec3& a, vec3& b, vec3& c) {

  if( MyLevel < MaxLevel) {
    vec3 ab = midpointOnSphere(a, b);
    vec3 bc = midpointOnSphere(b, c);
    vec3 ca = midpointOnSphere(c, a);
    MyLevel++;
    AddIcosahedron(MaxLevel, MyLevel, a, ab, ca);
    AddIcosahedron(MaxLevel, MyLevel, ab,  b, bc);
    AddIcosahedron(MaxLevel, MyLevel, ca, bc,  c);
    AddIcosahedron(MaxLevel, MyLevel, ab, bc, ca);
  }
  else {
    AddTriangle(a, b, c);
  }
}



void ComputeIcosahedronVertex(int Level) {
  double T1 = myclock();

  normals.clear();
  vertices.clear();

  if(Level == 0) {     //point
    normals.clear();
    vertices.clear();
    vertices.push_back( vec3(0,0,0) );
    normals.push_back( vec3(0,0,1) );
    return;
  }
  
  //int Nov = 20*pow(4,Level)*9; //20 (init faces) * 4^n (subdiv) * 9 (vertex)
  
  double phi = (1.0 + sqrt(5.0))/2.0;             //Edge length = golden ratio
  double cRadius = sqrt(10 + 2*sqrt(5)) /(4*phi); // Circumscribed Radius
  double a = (1.0 /cRadius) /2;                  //constants for vertices
  double b = (1.0 /cRadius) /(2 *phi);

  //Init platonic solid Icosahedron
  //Vert: 12 Edges: 30 Faces: 20 Edges per face: 3 Edges shared per vertex: 5
  vec3 v1 = vec3( 0,  b, -a);
  vec3 v2 = vec3( b,  a,  0);
  vec3 v3 = vec3(-b,  a,  0);
  vec3 v4 = vec3( 0,  b,  a);
  vec3 v5 = vec3( 0, -b,  a);
  vec3 v6 = vec3(-a,  0,  b);
  vec3 v7 = vec3( 0, -b, -a);
  vec3 v8 = vec3( a,  0, -b);
  vec3 v9 = vec3( a,  0,  b);
  vec3 vA = vec3(-a,  0, -b);
  vec3 vB = vec3( b, -a,  0);
  vec3 vC = vec3(-b, -a,  0);

  AddIcosahedron(Level, 1, v1, v2, v3);
  AddIcosahedron(Level, 1, v4, v3, v2);
  AddIcosahedron(Level, 1, v4, v5, v6);
  AddIcosahedron(Level, 1, v4, v9, v5);
  AddIcosahedron(Level, 1, v1, v7, v8);
  AddIcosahedron(Level, 1, v1, vA, v7);
  AddIcosahedron(Level, 1, v5, vB, vC);
  AddIcosahedron(Level, 1, v7, vC, vB);
  AddIcosahedron(Level, 1, v3, v6, vA);
  AddIcosahedron(Level, 1, vC, vA, v6);
  AddIcosahedron(Level, 1, v2, v8, v9);
  AddIcosahedron(Level, 1, vB, v9, v8);
  AddIcosahedron(Level, 1, v4, v6, v3);
  AddIcosahedron(Level, 1, v4, v2, v9);
  AddIcosahedron(Level, 1, v1, v3, vA);
  AddIcosahedron(Level, 1, v1, v8, v2);
  AddIcosahedron(Level, 1, v7, vA, vC);
  AddIcosahedron(Level, 1, v7, vB, v8);
  AddIcosahedron(Level, 1, v5, vC, v6);
  AddIcosahedron(Level, 1, v5, v9, vB);

  double Time = myclock()-T1;
  printDbg(3, "ComputeIcosahedronVertex (", Time, ")\n", "PURPLE");
}


//tetrahedron
// void ComputeIcosahedronVertex1(int Level) {

//   Normal.clear();
//   vertex.clear();
//   int Nov = pow(4,Level+1)*9; //4 (init faces) * 4^n (subdiv) * 9 (vertex)
  
//   //init tetrahedron
//   vec3 v1 = vec3( 1,  1,  1) /sqrt(3.0);
//   vec3 v2 = vec3( 1, -1, -1) /sqrt(3.0);
//   vec3 v3 = vec3(-1,  1, -1) /sqrt(3.0);
//   vec3 v4 = vec3(-1, -1,  1) /sqrt(3.0);

//   AddIcosahedron(Level, 1, v1, v2, v3);
//   AddIcosahedron(Level, 1, v1, v2, v4);
//   AddIcosahedron(Level, 1, v1, v3, v4);
//   AddIcosahedron(Level, 1, v2, v3, v4);
// }
