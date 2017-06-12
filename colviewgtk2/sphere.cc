#include "visu.hh"

extern DataObject * d;

/* Make it not a power of two to avoid cache thrashing on the chip */
#define CS	60
float cb[CS], cb2[CS];
float ss2[CS][CS];
float cs2[CS][CS];
float ss1[CS][CS];
float cs1[CS][CS];
int slices, stacks;
float radius;

void ComputeSphereVertex(double Rd, int Sl, int St) {

  radius = Rd;
  slices = Sl;
  stacks = St;

  if(slices >= CS) slices = CS-1;
  if(stacks >= CS) stacks = CS-1;
  if(slices < 2 || stacks < 1 || radius < 0.0) {
    cout << "Error ComputeSphereVertex" << endl;;
    return;
  }

  // Cache1 is vertex locations
  // Cache2 is various normals at the vertices
  // Cache3 is various normals for the faces

  float PI = 3.14159265358979323846;
  float dy = PI/stacks, dx = 2*PI/slices;

  for(int j = 0; j <= stacks; j++) {
    cb[j] = -cos(dy*j);
    cb2[j] = radius * cos(dy*j);
    float sb = -sin(dy*j);

    for(int i = 0; i <= slices; i++) {
      float sa = sin(dx*i);
      float ca = cos(dx*i);
      ss1[i][j] = -radius * sb * sa;
      cs1[i][j] = -radius * sb * ca;
      ss2[i][j] =  sa * sb;
      cs2[i][j] =  ca * sb;
    }
  }
}


// void PlotSphere(int j) {
//   glPushMatrix();
//   glTranslated(d->r[j].x, d->r[j].y, d->r[j].z);

//   // Low end first (j == 0)
//   glNormal3f(ss2[0][0], cs2[0][0], cb[0]);
 
//   glBegin(GL_TRIANGLE_FAN);
//   glVertex3f(0.0, 0.0, radius);
//   for(int i = 0; i <= slices; i++) {
//     glNormal3f(ss2[i][1], cs2[i][1], cb[1]);
//     glVertex3f(ss1[i][1], cs1[i][1], cb2[1]);
//   }  
//   glEnd();


//   // High end next (j == stacks-1)
//   glNormal3f(ss2[stacks][stacks], cs2[stacks][stacks], cb[stacks]);

//   glBegin(GL_TRIANGLE_FAN);
//   glVertex3f(0.0, 0.0, -radius);
//   for(int i = slices; i >= 0; i--) {
//     glNormal3f(ss2[i][stacks-1], cs2[i][stacks-1], cb[stacks-1]);
//     glVertex3f(ss1[i][stacks-1], cs1[i][stacks-1], cb2[stacks-1]);
//   }
//   glEnd();


//   for(int j = 1; j < (stacks-1); j++) {
//     glBegin(GL_QUAD_STRIP);
//     for(int i = 0; i <= slices; i++) {
//       glNormal3f(ss2[i][j],   cs2[i][j],   cb[j]);
//       glVertex3f(ss1[i][j],   cs1[i][j],   cb2[j]);
//       glNormal3f(ss2[i][j+1], cs2[i][j+1], cb[j+1]);
//       glVertex3f(ss1[i][j+1], cs1[i][j+1], cb2[j+1]);
//     }
//     glEnd();
//   }
  
//   glPopMatrix();
// }


	       
void PlotSphere(int j) {
  glPushMatrix();
  glTranslated(d->r[j].x, d->r[j].y, d->r[j].z);
 
  glBegin(GL_TRIANGLE_FAN);

  glNormal3f(ss2[0][0], cs2[0][0], cb[0]);   // Low end (j == 0)
  glVertex3f(0.0, 0.0, radius);
  for(int i = 0; i <= slices; i++) {
    glNormal3f(ss2[i][1], cs2[i][1], cb[1]);
    glVertex3f(ss1[i][1], cs1[i][1], cb2[1]);
  }  
 
  glNormal3f(ss2[stacks][stacks], cs2[stacks][stacks], cb[stacks]); // High end (j == stacks-1)
  glVertex3f(0.0, 0.0, -radius);
  for(int i = slices; i >= 0; i--) {
    glNormal3f(ss2[i][stacks-1], cs2[i][stacks-1], cb[stacks-1]);
    glVertex3f(ss1[i][stacks-1], cs1[i][stacks-1], cb2[stacks-1]);
  }
  glEnd();

  glBegin(GL_QUAD_STRIP);
  for(int j = 1; j < (stacks-1); j++) {
    for(int i = 0; i <= slices; i++) {
      glNormal3f(ss2[i][j],   cs2[i][j],   cb[j]);
      glVertex3f(ss1[i][j],   cs1[i][j],   cb2[j]);
      glNormal3f(ss2[i][j+1], cs2[i][j+1], cb[j+1]);
      glVertex3f(ss1[i][j+1], cs1[i][j+1], cb2[j+1]);
    }
  }
  glEnd();
  
  glPopMatrix();
}
