#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>  //cout
using namespace std;

/* Make it not a power of two to avoid cache thrashing on the chip */
#define CS	60
#define PI      3.14159265358979323846


void MygluSphere(GLdouble radius, GLint slices, GLint stacks) {
  GLfloat s1a[CS];
  GLfloat c1a[CS];
  GLfloat s2a[CS];
  GLfloat c2a[CS];
  GLfloat s1b[CS];
  GLfloat c1b[CS];
  GLfloat s2b[CS];
  GLfloat c2b[CS];

  float ss2[CS][CS];
  float cs2[CS][CS];
  float ss1[CS][CS];
  float cs1[CS][CS];

  if(slices >= CS) slices = CS-1;
  if(stacks >= CS) stacks = CS-1;
  if(slices < 2 || stacks < 1 || radius < 0.0) {
    cout << "gluQuadricError xxx";
    return;
  }

  /* Cache1 is the vertex locations cache */
  /* Cache2 is the various normals at the vertices themselves */
  /* Cache3 is the various normals for the faces */


  for(int i = 0; i < slices; i++) {
    float ang = 2 * PI * i / slices;
    s1a[i] = sin(ang);
    c1a[i] = cos(ang);
    s2a[i] = s1a[i];
    c2a[i] = c1a[i];
  }

  for(int j = 0; j <= stacks; j++) {
    float ang = PI * j / stacks;
    s2b[j] = -sin(ang);
    c2b[j] = -cos(ang);
    s1b[j] = radius * sin(ang);
    c1b[j] = radius * cos(ang);
  }

  /* Make sure it comes to a point */
  s1b[0] = 0;
  s1b[stacks] = 0;

  s1a[slices] = s1a[0];
  c1a[slices] = c1a[0];
  s2a[slices] = s2a[0];
  c2a[slices] = c2a[0];



  for(int j = 0; j <= stacks; j++) {
    for(int i = 0; i <= slices; i++) {
      ss2[i][j] = s2a[i] * s2b[j];
      cs2[i][j] = c2a[i] * s2b[j];
      ss1[i][j] = s1a[i] * s1b[j];
      cs1[i][j] = c1a[i] * s1b[j];
    }
  }

 
  // Low end first (j == 0 iteration)
  glNormal3f(ss2[0][0], cs2[0][0], c2b[0]);
 
  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(0.0, 0.0, radius);
  for(int i = 0; i <= slices; i++) {
    glNormal3f(ss2[i][1], cs2[i][1], c2b[1]);
    glVertex3f(ss1[i][1], cs1[i][1], c1b[1]);
  }  
  glEnd();


  // High end next (j == stacks-1 iteration)
  glNormal3f(ss2[stacks][stacks], cs2[stacks][stacks], c2b[stacks]);

  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(0.0, 0.0, -radius);
  for(int i = slices; i >= 0; i--) {
    glNormal3f(ss2[i][stacks-1], cs2[i][stacks-1], c2b[stacks-1]);
    glVertex3f(ss1[i][stacks-1], cs1[i][stacks-1], c1b[stacks-1]);
  }
  glEnd();


  for(int j = 1; j < (stacks-1); j++) {
    glBegin(GL_QUAD_STRIP);
    for(int i = 0; i <= slices; i++) {
      glNormal3f(ss2[i][j],   cs2[i][j],   c2b[j]);
      glVertex3f(ss1[i][j],   cs1[i][j],   c1b[j]);
      glNormal3f(ss2[i][j+1], cs2[i][j+1], c2b[j+1]);
      glVertex3f(ss1[i][j+1], cs1[i][j+1], c1b[j+1]);
    }
    glEnd();
  }

}
