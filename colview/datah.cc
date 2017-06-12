
#include "visu.hh"
#include "points.hh"
extern vector <string> tpi, ntpi;

#define MAXVAL ( std::numeric_limits<GLfloat>::max() )
//#define MINVAL ( std::numeric_limits<GLfloat>::min() )



bool thecmp(const Point& a, const Point& b) {
  //cout << "cmp " << a.f[1] << " " << b.f[1] << endl;
  return a.f[1] < b.f[1];
}



void SortData(DataObject * d) {
  cout << "SortData " << d->r.size(); //<< " " << d->r.max_size();
  double T1 = myclock();
  sort(d->r.begin(), d->r.end(), thecmp );
  cout << " (" << myclock()-T1 << " sec)" << endl;
}



void SwapData(DataObject * d, string tsw) {

  if( tsw == string("sxy") ) {
    for(unsigned long i=0; i< d->NumDat; i++) {
      float t   = d->r[i].x;
      d->r[i].x = d->r[i].y;
      d->r[i].y = t;
    }
  }
  else if( tsw == string("sxz") ) {
    for(unsigned long i=0; i< d->NumDat; i++) {
      float   t = d->r[i].x;
      d->r[i].x = d->r[i].z;
      d->r[i].z = t;
    }
  }
  else if( tsw == string("syz") ) {
    for(unsigned long i=0; i< d->NumDat; i++) {
      float t = d->r[i].y;
      d->r[i].y = d->r[i].z;
      d->r[i].z = t;
    }
  }
  else if( tsw == string("sxx") ) {
    for(unsigned long i=0; i< d->NumDat; i++)
      d->r[i].x = -d->r[i].x;
  }
  else if( tsw == string("syy") ) {
    for(unsigned long i=0; i< d->NumDat; i++)
      d->r[i].y = -d->r[i].y;
  }
  else if( tsw == string("szz") ) {
    for(unsigned long i=0; i< d->NumDat; i++)
      d->r[i].z = -d->r[i].z;
  }
  
  ComputeFrame(d);
  CenterData(d);
  ComputeFrame(d);

  d->NeedUpdatePV = true;
}



// void SwapDataXY(DataObject * d) {

//   for(unsigned long i=0; i< d->NumDat; i++) {
//     float t = d->r[i].x;
//     d->r[i].x = d->r[i].y;
//     d->r[i].y = t;
//   }

//   ComputeFrame(d);
//   CenterData(d);
//   ComputeFrame(d);
//   d->NeedUpdatePV = true;
// }



// void SwapDataXZ(DataObject * d) {

//   for(unsigned long i=0; i< d->NumDat; i++) {
//     float t = d->r[i].x;
//     d->r[i].x = d->r[i].z;
//     d->r[i].z = t;
//   }

//   ComputeFrame(d);
//   CenterData(d);
//   ComputeFrame(d);
//   d->NeedUpdatePV = true;
// }



// void SwapDataYZ(DataObject * d) {

//   for(unsigned long i=0; i< d->NumDat; i++) {
//     float t = d->r[i].y;
//     d->r[i].y = d->r[i].z;
//     d->r[i].z = t;
//   }

//   ComputeFrame(d);
//   CenterData(d);
//   ComputeFrame(d);
//   d->NeedUpdatePV = true;
// }



// void SwapDataZZ(DataObject * d) {

//   for(unsigned long i=0; i< d->NumDat; i++) {
//     //float t = d->r[i].y;
//     d->r[i].z = -d->r[i].z;
//   }

//   ComputeFrame(d);
//   CenterData(d);
//   ComputeFrame(d);
//   d->NeedUpdatePV = true;
// }

// void SwapDataYY(DataObject * d) {

//   for(unsigned long i=0; i< d->NumDat; i++) {
//     //float t = d->r[i].y;
//     d->r[i].y = -d->r[i].y;
//   }

//   ComputeFrame(d);
//   CenterData(d);
//   ComputeFrame(d);
//   d->NeedUpdatePV = true;
// }




void BestSpatialUnit(DataObject * d) {

  //string unit[16] = {"m", "cm", "mm", "μm",     "nm", "Angstrom" };
  string unit[16] = {"m", "cm", "mm", "micron", "nm", "Angstrom" };

  float   fac[16] = {  1, 1e-2, 1e-3, 1e-6, 1e-9,    1e-10   };

  string newunit = "         ";
  float uf = 1;

  while( d->rMax < 1.0) {
    bool needChange = false;

    for(int i=0; i<6; i++) {
      if(d->SpatialUnits == unit[i]) {
	newunit = unit[i+1];
	uf = fac[i]/fac[i+1];
	needChange = true;
      }
    }

    if(needChange) {
      cout << "BestSpatialUnit1: " << d->SpatialUnits << " "  << d->SpatialFactor <<endl;
      ScaleData(d, uf, uf, uf);
      ComputeFrame(d);
      d->SpatialUnits = newunit;      
      d->SpatialFactor *= uf;
      cout << "BestSpatialUnit2: " << d->SpatialUnits << " "  << d->SpatialFactor <<endl;
    }
  }
}



void ComputeFrame(DataObject * d) {

  //static bool FirstCall = true;
 
  //if(FirstCall && d->NumDat > 0)
    {
      //cout << "ComputeFrame FirstCall ------------------ " << FirstCall << endl;
      //FirstCall = false;

    d->m.f[3] = d->r[0].f[3]; d->M.f[3] = d->r[0].f[3];
    d->m.f[4] = d->r[0].f[4]; d->M.f[4] = d->r[0].f[4];
    d->m.f[5] = d->r[0].f[5]; d->M.f[5] = d->r[0].f[5];
    d->m.f[6] = d->r[0].f[6]; d->M.f[6] = d->r[0].f[6];
    d->m.f[0] = d->r[0].f[0]; d->M.f[0] = d->r[0].f[0];
    d->m.f[1] = d->r[0].f[1]; d->M.f[1] = d->r[0].f[1];
    d->m.f[2] = d->r[0].f[2]; d->M.f[2] = d->r[0].f[2];

    for(unsigned long i=0; i< d->NumDat; i++) {
      if( d->r[i].f[0] < d->m.f[0]) d->m.f[0] = d->r[i].f[0];
      if( d->r[i].f[0] > d->M.f[0]) d->M.f[0] = d->r[i].f[0];
      if( d->r[i].f[1] < d->m.f[1]) d->m.f[1] = d->r[i].f[1];
      if( d->r[i].f[1] > d->M.f[1]) d->M.f[1] = d->r[i].f[1];
      if( d->r[i].f[2] < d->m.f[2]) d->m.f[2] = d->r[i].f[2];
      if( d->r[i].f[2] > d->M.f[2]) d->M.f[2] = d->r[i].f[2];
      if( d->r[i].f[3] < d->m.f[3]) d->m.f[3] = d->r[i].f[3];
      if( d->r[i].f[3] > d->M.f[3]) d->M.f[3] = d->r[i].f[3];
      if( d->r[i].f[4] < d->m.f[4]) d->m.f[4] = d->r[i].f[4];
      if( d->r[i].f[4] > d->M.f[4]) d->M.f[4] = d->r[i].f[4];
      if( d->r[i].f[5] < d->m.f[5]) d->m.f[5] = d->r[i].f[5];
      if( d->r[i].f[5] > d->M.f[5]) d->M.f[5] = d->r[i].f[5];
      if( d->r[i].f[6] < d->m.f[6]) d->m.f[6] = d->r[i].f[6];
      if( d->r[i].f[6] > d->M.f[6]) d->M.f[6] = d->r[i].f[6];
    }
  }

  d->m.x = MAXVAL; d->M.x = -MAXVAL;
  d->m.y = MAXVAL; d->M.y = -MAXVAL;
  d->m.z = MAXVAL; d->M.z = -MAXVAL;

  for(unsigned long i=0; i< d->NumDat; i++) {
    if( d->r[i].x < d->m.x) d->m.x = d->r[i].x;
    if( d->r[i].x > d->M.x) d->M.x = d->r[i].x;
    if( d->r[i].y < d->m.y) d->m.y = d->r[i].y;
    if( d->r[i].y > d->M.y) d->M.y = d->r[i].y;
    if( d->r[i].z < d->m.z) d->m.z = d->r[i].z;
    if( d->r[i].z > d->M.z) d->M.z = d->r[i].z;
  }

  //rM = sqrt(rM);
  d->rMax = sqrt(d->M.x*d->M.x + d->M.y*d->M.y + d->M.z*d->M.z);
  d->Rv = d->rMax*1.1;

  d->xc = (d->m.x + d->M.x)*0.5;
  d->yc = (d->m.y + d->M.y)*0.5;
  d->zc = (d->m.z + d->M.z)*0.5;
  d->xn = (d->M.x - d->m.x)*0.5;
  d->yn = (d->M.y - d->m.y)*0.5;
  d->zn = (d->M.z - d->m.z)*0.5;
  d->wn = std::max(d->xn, d->yn);
  d->wn = std::max(d->wn, d->zn);
  d->Diameter = sqrt( pow((d->M.x -d->m.x),2) + pow((d->M.y -d->m.y),2) +
		      pow((d->M.z -d->m.z),2) );
  d->Radius = d->Diameter*0.5;

  // d->c.x = d->m.x; d->C.x = d->M.x;
  // d->c.y = d->m.y; d->C.y = d->M.y;
  // d->c.z = d->m.z; d->C.z = d->M.z;
  // d->c.f[3] = d->m.f[3]; d->C.z = d->M.z;
  // d->ctm = d->tm; d->ctM = d->tM;
  // d->cem = d->em; d->ceM = d->eM;
  // d->clm = d->lm; d->clM = d->lM;
  // d->cpm = d->pm; d->cpM = d->pM;
  d->c = d->m;  //init clip = min
  d->C = d->M;   //     Clip = Max

  // d->LightPos.x = d->rMax*5;
  // d->LightPos.y = -d->rMax*5;
  // d->LightPos.z = d->rMax*5;
  d->Volume = (d->M.x - d->m.x) * (d->M.y - d->m.y) * (d->M.z - d->m.z);

  if( d->verboseOut > 0) 
    cout << "ComputeFrame:" << endl
	 << "\tNumDat " << d->NumDat
	 << "\tVol: " << d->Volume
	 << "\tr |" << d->rMax << "|" << endl
	 << "\tx (" << d->m.x << "," << d->M.x << ")"
	 << "\ty (" << d->m.y << "," << d->M.y << ")"
	 << "\tz (" << d->m.z << "," << d->M.z << ")" << endl
	 << "\tf[3] (" << d->m.f[3] << "," << d->M.f[3] << ")"
	 << "\tf[4] (" << d->m.f[4] << "," << d->M.f[4] << ")"
	 << "\tf[5] (" << d->m.f[5] << "," << d->M.f[5] << ")"
	 << "\tf[6] (" << d->m.f[6] << "," << d->M.f[6] << ")" << endl
	 << "\tf[0] (" << d->m.f[0] << "," << d->M.f[0] << ")"
	 << "\tf[1] (" << d->m.f[1] << "," << d->M.f[1] << ")"
	 << "\tf[2] (" << d->m.f[2] << "," << d->M.f[2] << ")" << endl
      //<< "\tcolor (" << d->tm << "," << d->tM << ")\n"
      // << "\tE     (" << d->em  << "," << d->eM << ")\n"
      // << "\tEloss (" << d->lm  << "," << d->lM << ")\n"
      // << "\tPar Ty(" << d->pm  << "," << d->pM << ")\n"
	 << endl;
}


void ComputeHistogram(DataObject *d) {
  cout << "ComputeHistogram" << endl;
  
  // clear hist
  d->EdepTotal = 0;
  for(int i=0; i<100; i++) {
    d->Hist[i] = 0;
    d->Hist2[i] = 0.0;
  }

  for(unsigned long i=0; i< d->NumDat; i++) {
    unsigned long j = d->r[i].f[1];
    if( j>=0 && j<tpi.size()) {
      d->Hist[j]++;
      d->Hist2[j] += d->r[i].f[5];
    }
    d->EdepTotal += d->r[i].f[5];
    //if(d->NumDat < 50) cout << i << " " << d->r[i].d << " " << d->EdepTotal << endl;
  }


  //cout << "Histogram:" << endl;
  //for(int i=0; i<100; i++) {
  //  if( d->Hist[i] || d->Hist[i-1] || d->Hist[i+1] )
  //    cout << "\t"<< i-1 << " " << tpi[i] << " " << d->Hist[i]
  //	   << " (" << (float)d->Hist[i]/(float)d->NumDat << ")" <<endl;
  //}
  //cout << "Total: " << d->NumDat << endl;
}



void CenterData(DataObject * d) {
  //#ifdef DEBUG
  cout << "visuogl CenterData" << endl;
  //#endif

  for(unsigned long i=0; i< d->NumDat; i++) {
    d->r[i].x -= (d->m.x + d->M.x)*0.5;
    d->r[i].y -= (d->m.y + d->M.y)*0.5;
    d->r[i].z -= (d->m.z + d->M.z)*0.5;
  }
}




void CornerData(DataObject * d) {
#ifdef DEBUG
  cout << "visuogl CornerData" << endl;
#endif

  for(unsigned long i=0; i< d->NumDat; i++) {
    d->r[i].x -= d->m.x;
    d->r[i].y -= d->m.y;
    //d->r[i].z -= d->m.z;
    d->r[i].z -= (d->m.z + d->M.z)*0.5;
  }
}



void ShiftData(DataObject * d, float x, float y, float z) {
  cout << "ShiftData " << x << " " << y << " " << z << endl;
  for(unsigned long i=0; i< d->NumDat; i++) {
    d->r[i].x -= x;
    d->r[i].y -= y;
    d->r[i].z -= z;
  }
}



void ScaleData(DataObject * d, float sx, float sy, float sz) {
#ifdef DEBUG
  cout << "visuogl ScaleData" << endl;
#endif

  for(unsigned long i=0; i< d->NumDat; i++) {
    d->r[i].x *= sx;
    d->r[i].y *= sy;
    d->r[i].z *= sz;
  }
}



// Puntos proximos
void Interpolate(DataObject * d, int np) {
  static bool FirstCall = true;
  int * idx = new int[np+1];
  double * dis = new double[np+1];
  Point * b;
 
  if(FirstCall) {
    cout << "Interpolate FirstCall ------------------ " << FirstCall << endl;
    FirstCall = false;

    b = new Point[d->NumDat];
    for(unsigned long i=0; i<d->NumDat; i++)
      b[i] = d->r[i];
  }

  for(unsigned long i=0; i<d->NumDat; i++) {
    for(int k=0;k<np; k++) {
      dis[k] = MAXVAL;
      idx[k] = 0;
    }
    // for(unsigned long j=0; j<d->NumDat, j!=i; j++) {
      //float D = pow(d->r[i].x-b[j].x,2) +pow(d->r[i].y-b[j].y,2) +pow(d->r[i].z-b[j].z,2);
      //for(int k=0;k<np; k++) {

      //}
    //}
  }

}
