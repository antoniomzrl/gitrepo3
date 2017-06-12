//#include "visu.hh"
#include <GL/gl.h>
#include <cstdlib>
#include<iostream>
using namespace std;

#include <tiffio.h>


void WindowDump(char *, int, int);


void SaveImage(int x, int y, int w, int h) {
  static int i = 1;
  char fn[128], cmd[128];
  sprintf(fn, "img%02d.ppm", i);
  cout << fn << " " << w << "x" << h << endl;

  WindowDump(fn, w, h);

  sprintf(cmd, "convert img%02d.ppm img%02d.png", i, i);
  if( system(cmd) < 0) cout << "error executing: " << cmd << endl;
  
  i++;
}



void WindowDump(char * fname, int width, int height) {
   unsigned char * image;

   /* Allocate our buffer for the image */
   if ((image = (unsigned char *) malloc(3*width*height)) == NULL) {
      fprintf(stderr,"Failed to allocate memory for image\n");
      return;
   }

   FILE * fi= fopen(fname,"w");
   if (fi == NULL) {
      fprintf(stderr,"Failed to open file for window dump\n");
      return;
   }

   //xxx glPixelStorei(GL_PACK_ALIGNMENT,1);

   /* Copy the image into our buffer */
   //xxx glReadBuffer(GL_BACK_LEFT);
   //xxx glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE,image);

   /* Write the raw file */
   fprintf(fi,"P6\n%d %d\n255\n",width,height);

   //fwrite(image,width*height*3,1,fi); // order
   for (int j=height-1;j>=0;j--) {
      for (int i=0;i<width;i++) {
         fputc(image[3*j*width+3*i+0],fi);
         fputc(image[3*j*width+3*i+1],fi);
         fputc(image[3*j*width+3*i+2],fi);
      }
   }
   fclose(fi);

   free(image);
   return;
}
