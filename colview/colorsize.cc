#include "visu.hh"

extern DataObject * d;

vector <vec4> posval;
vector <vec4> plt;

float Interpolate(int x, int x1, int x2, float y1, float y2) {
  return( y1 + (y2-y1)*(x-x1)/(x2-x1) );
}

void InitPalette() {
  //  plt.push_back( vec4(0.3, 0,  0.5,0 ) ); // Indigo 
  plt.push_back( vec4(0.5, 0,   1  ,0 ) ); // Violet 
  plt.push_back( vec4(0,   0,   1,  0 ) ); // B
  plt.push_back( vec4(0,   1,   1,  0 ) ); // Cyan
  plt.push_back( vec4(0,   1,   0,  0 ) ); // G
  plt.push_back( vec4(1,   1,   0,  0 ) ); // Yellow
  plt.push_back( vec4(1,   0.5, 0,  0 ) ); // Orange
  plt.push_back( vec4(1,   0,   0,  0 ) ); // R

  if( plt.size() > 10) {
    cout << "WARNING, plt size > 10!!! change plt[10] in shader" << endl;
    exit(-1);
  }
  
  // plt.push_back( vec4( 1.0  ,0.5  ,0.5  , 0 ) );
  // plt.push_back( vec4( 1.0  ,0.75 ,0.5  , 0 ) );
  // plt.push_back( vec4( 1.0  ,1.0  ,0.5  , 0 ) );
  // plt.push_back( vec4( 0.75 ,1.0  ,0.5  , 0 ) );
  // plt.push_back( vec4( 0.5  ,1.0  ,0.5  , 0 ) );
  // plt.push_back( vec4( 0.5  ,1.0  ,0.75 , 0 ) );
  // plt.push_back( vec4( 0.5  ,1.0  ,1.0  , 0 ) );
  // plt.push_back( vec4( 0.5  ,0.75 ,1.0  , 0 ) );
  // plt.push_back( vec4( 0.5  ,0.5  ,1.0  , 0 ) );
  // plt.push_back( vec4( 0.75 ,0.5  ,1.0  , 0 ) );
  // plt.push_back( vec4( 1.0  ,0.5  ,1.0  , 0 ) );
  // plt.push_back( vec4( 1.0  ,0.5  ,0.75 , 1 ) );
  
  // plt.push_back( vec4(0.5,    0  ,    1  , 0)); // Violet 
  // plt.push_back( vec4(0.3,    0  ,    0.5, 0)); // Indigo 
  // plt.push_back( vec4(0  ,    0  ,    1  , 0)); // B   
  // plt.push_back( vec4(0  ,    1  ,    0  , 0)); // G  
  // plt.push_back( vec4(1  ,    1  ,    0  , 0)); // Yellow 
  // plt.push_back( vec4(1  ,    0.5,    0  , 0)); // Orange 
  // plt.push_back( vec4(1  ,    0  ,    0  , 0)); // R
}



void UpdatePalette() {
  static bool first = true;
  if( first) {
    first = false;
    InitPalette();
  }

  int N = plt.size();
  for(int i=0; i<N; i++) {
    if(i<N/2) plt[i].w = Interpolate(i,   0, N/2, d->CanvasPoint[0], d->CanvasPoint[1]);
    else      plt[i].w = Interpolate(i, N/2, N-1, d->CanvasPoint[1], d->CanvasPoint[2]);
  }

  if( d->DiscretizePalette > 15)
    d->DiscretizePalette = 1000;
  
  // Normalize r g b
  // for(unsigned int i=0; i< plt.size(); i++) {
  //   //float len = sqrt( pow(plt[i].x,2) + pow(plt[i].y,2) + pow(plt[i].z,2) );
  //   float len = plt[i].x + plt[i].y + plt[i].z;
  //   vec3  col = plt[i].xyz() / len;
  //   float w   = plt[i].w;
  //   plt[i] = vec4( col, w);
  // }

  cout << "UpdatePalette" << endl;
  for(unsigned int i=0; i<plt.size(); i++) {
    cout << "palette " << i << " (";
    printglm(plt[i]);
  }
}



// For colorbar must match glsl shader
// globals / uniforms : d->BallFactor d->DiscretizePalette & plt
// vec3 ComputeColor(float v) {
//   float pltDtz = d->DiscretizePalette;
//   int   pltSz  = plt.size();
  
//   float x = clamp( round(pltDtz *v) / pltDtz, plt[0].w, plt[pltSz-1].w );
  
//   vec3 cl;

//   for( int j=0; j< pltSz-1; j++) {
//     if( x <= plt[j+1].w  && x >= plt[j].w ) {
//       cl = plt[j].xyz() + (plt[j+1].xyz() - plt[j].xyz())
//   	* (x - plt[j].w) / (plt[j+1].w - plt[j].w);
//     }
//   }

//   // Nearest:  
//   // float dmin = pltSz; int lprox = 0;
//   // for( unsigned int j=0; j< pltSz; j++) {
//   //   if( abs(x - plt[j].w) < dmin ) {
//   //     lprox = j;
//   //     dmin = abs(x - plt[j].w);
//   //   }
//   // }
//   // cl = plt[lprox].xyz();

//   return (cl);
// }



// float Interpolate(float x1, float x2, float y1, float y2, float x, int Nsteps) {
//   float y = y1 + (y2-y1)*(x-x1)/(x2-x1);
//   return( nearbyintf(y*Nsteps)/Nsteps );
// }


// float Interpolate(float x1, float x2, float y1, float y2, float x) {
//   return( y1 + (y2-y1)*(x-x1)/(x2-x1) );
// }

// // For colorbar must match glsl shader
// // globals / uniforms : d->BallFactor & d->CanvasVector[5]
// vec4 ComputeColorSizeOLD(float v) {
//   static float R[6] = { 0, 0, 0, 1, 1, 1};
//   static float G[6] = { 0, 1, 1, 1, 0, 0};
//   static float B[6] = { 1, 1, 0, 0, 0, 1};

//   float x = clamp(v, 0.001f, 1.0f);

//   int i=0;
//   for( int j=0; j<(5-1); j++)
//     if( x > d->CanvasVector[j] && x <= d->CanvasVector[j+1] )
//       i=j+1;

//   float r = Interpolate(d->CanvasVector[i-1], d->CanvasVector[i], R[i-1], R[i], x);
//   float g = Interpolate(d->CanvasVector[i-1], d->CanvasVector[i], G[i-1], G[i], x);
//   float b = Interpolate(d->CanvasVector[i-1], d->CanvasVector[i], B[i-1], B[i], x);
//   float sz = d->BallFactor + (1 - d->BallFactor) * x;

//   return vec4(r, g, b, sz);
// }



void UpdatePV(DataObject *d) {
  double T1 = myclock();

  posval.clear();
  vec3 c(d->xc, d->yc, d->zc);

  float vm = d->m.f[d->vType];
  float vM = d->M.f[d->vType];
  if(vm == vM) {
    vm *= 0.999;
    vM *= 1.001;
  }
  
  cout << "strides " << d->Stride << " " << d->SmartStride << endl;

  // if( d->SmartStride >1) {
  //   float fac = 1.0/d->SmartStride;
  //   for(unsigned long i=d->MinDat; i<d->MaxDat; i++) {
  //     int nd = d->Hist[(int)d->r[i].f[1]];
  //     float rn = 0;
  //     if( nd > 100 ) rn = static_cast <float>(rand()) / static_cast <float>(RAND_MAX);
  //     if( rn < fac ) {
  // 	vec3  p = vec3(d->r[i].x, d->r[i].y, d->r[i].z);
  // 	float v = (d->r[i].f[d->vType]-vm)/(vM-vm); // in (0,1)
  // 	posval.push_back( vec4((p-c)/d->wn, v) );

  //     }
  //   }
  // }

  if( d->SmartStride >1) {
    float fac = 1.0/d->SmartStride;
    for(unsigned long i=d->MinDat; i<d->MaxDat; i++) {
      int Nd = d->Hist[(int)d->r[i].f[1]];
      float rn = static_cast <float>(rand()) / static_cast <float>(RAND_MAX);
      if( rn < fac || Nd < 1000 ) {
   	vec3  p = vec3(d->r[i].x, d->r[i].y, d->r[i].z);
   	float v = (d->r[i].f[d->vType]-vm)/(vM-vm); // in (0,1)
   	posval.push_back( vec4((p-c)/d->wn, v) );
      }
    }
  }
  else {
    for(unsigned long i=d->MinDat; i<d->MaxDat; i+=d->Stride) {
      vec3  p = vec3(d->r[i].x, d->r[i].y, d->r[i].z);
      float v = (d->r[i].f[d->vType]-vm)/(vM-vm); // in (0,1)
      posval.push_back( vec4((p-c)/d->wn, v) );
    }
  }

  double Time = myclock()-T1;
  
  printDbg(3, "UpdatePV(", Time, ")\n", "PURPLE");
}


// void UpdateP(DataObject *d) {
//   double T1 = myclock();

//   UpdateCanvasVector();
    
//   posval.clear();
//   vec3 c(d->xc, d->yc, d->zc);

//   for(unsigned long i=d->MinDat; i<d->MaxDat; i+=d->Stride) {
//     vec3 p = vec3(d->r[i].x, d->r[i].y, d->r[i].z);
//     posval.push_back( vec4((p-c)/d->wn, 1.0) );
//   }

//   double Time = myclock()-T1;
  
//   printDbg(3, "UpdateP(", Time, ")\n", "PURPLE");
// }

// // Colors & Sizes
// void UpdateCS(DataObject *d) {
//   double T1 = myclock();

//   UpdateCanvasVector();
  
//   colors.clear();

//   float vm = d->m.f[d->vType];
//   float vM = d->M.f[d->vType];
    
//   for(unsigned long i=d->MinDat; i<d->MaxDat; i+=d->Stride) {
//     float v  = d->r[i].f[d->vType];
//     vec4 clsz = ComputeColorSize( (v-vm)/(vM-vm) );
//     colors.push_back( clsz );
//   }

//   double Time = myclock()-T1;
//   printDbg(3, "UpdateCS   (", Time, ")\n", "PURPLE");
// }