
#include "LeptsDistribution.hh"


LeptsDistribution::LeptsDistribution()
{
  bFileFound = false;
}

// old version
void LeptsDistribution::ReadFile(G4String fileName) {

  G4int eB, out, out2;
  float  float_data1,float_data2;
  G4double sum, esum;
  FILE * fp;

  for (eB=0;eB<10000;eB++){
    E[eB]=0.0;
    f[eB]=0.0;
    F[eB]=0.0;
    eF[eB]=0.0;
  }

  if ((fp=fopen(fileName.c_str(), "r"))==NULL){
    //G4cout << "Error reading " << fileName << G4endl;
    NoBins = 0;
    bFileFound = false;
    return;
  }
  else{
    bFileFound = true;
    //    G4cout << "Read Distro (" << fileName << ") " << G4endl;
    out=1;
    eB=1;
    while (out==1){
      out  = fscanf(fp,"%f \n",&float_data1);
      out2 = fscanf(fp,"%f \n",&float_data2);
      if (out==1 && out2==1){
	E[eB]=(G4double)float_data1;
	f[eB]=(G4double)float_data2;
	eB++;
      }
    }

    fclose(fp);
  }
  
  E[0] = E[1] - (E[2]-E[1])*0.01;
  E[eB] = E[eB-1] + (E[eB-1]-E[eB-2])*0.01;
  NoBins=eB;  //=1272+1 or 9607+1;

  if( NoBins >= NMAX )
    printf("ERROR !!!!  Eloss NoBins= %d \n", NoBins);

  sum=0.0;
  esum=0.0;
  for (eB=0;eB<=NoBins;eB++) {
    if( f[eB] > 0) {
      sum+=f[eB];
      esum+=E[eB]*f[eB];
    }
    F[eB]=sum;
    eF[eB]=esum;
  }

  //  if( verboseLevel >= 1 ) G4cout << "Norm: " << F[NoBins] << " NoBins: "<< NoBins << G4endl;
  Fnorm = F[NoBins];
  
  for (eB=0;eB<=NoBins;eB++) {
    eF[eB] = eF[eB]/F[eB];
    F[eB] = F[eB]/Fnorm;
    f[eB] = f[eB]/Fnorm;
  }
  //for (eB=0;eB<=NoBins;eB++)
  //G4cout << "eff " << E[eB] << " " << f[eB] << " " << F[eB] << "\n";
}



G4bool LeptsDistribution::ReadFile( FILE* fp, G4int nData ) 
{
  // for (i=0;i<10000;i++){
  //   E[i]=0.0;
  //   f[i]=0.0;
  //   F[i]=0.0;
  //   eF[i]=0.0;
  // }

  bFileFound = true;
  float d1, d2;
  int b=1;

  for(int id = 0; id < nData; id++ ){	  
    if (fscanf(fp,"%f %f", &d1, &d2) == 2) {
      E[b] = (double)d1;
      f[b] = (double)d2;
      b++;
    }
    else return 1;
  }
  
  f[0] = 0; f[b] = 0;
  E[0] = E[1]; E[b] = E[b-1];
  F[0] = 0;
  NoBins = b;

  if( NoBins >= NMAX ) {
    printf("ERROR !!!!  Eloss NoBins= %d \n", NoBins);
    exit( -1);
  }
  
  for( int i=0;i<NoBins;i++) {
    if(E[i] > E[i+1]) {
      cout << "Sanity error in distribution line " << i << " Ei > Ei+1 (" << E[i]
	   << " " << E[i+1] << ")" << endl;
      exit( -1);
    }
    if(f[i] < 0) {
      cout << "Sanity error in distribution line " << i << " fi < 0 (" << f[i] << ")" << endl;
      exit( -1);
    }
  }

  double sum=0, esum=0;
  for( int i=1;i<=NoBins;i++) {
    double hi = (E[i] -E[i-1]);
    sum += hi*(f[i]+f[i-1])/2.0;
    esum += hi*E[i]*f[i];
    F[i] = sum;
    eF[i] = esum;
  }

  G4cout << " Distr NoDat " << nData << " mean " << esum/sum << G4endl;
  
  //if( verboseLevel >= 1 ) G4cout << "Norm: " << F[NoBins] << " NoBins: "<< NoBins << G4endl;
  
  Fnorm = F[NoBins];
  for(int i=0;i<=NoBins;i++) {
    //cout << "ixfF " << i << " " << E[i] << " " << f[i] << " " << F[i] << endl;
    eF[i] = eF[i]/F[i];
    F[i] = F[i]/Fnorm;
    f[i] = f[i]/Fnorm;
  }
  
  // cout << "i xi fi Fi" << endl;
  // for(int i=0; i<=NoBins; i++)
  //   cout << i << "\t" << E[i] << "\t" << f[i]*Fnorm << "\t" << F[i] << endl;

  return 0;
}



G4double LeptsDistribution::Sample( G4double eMin, G4double eMax) {
  // Sample Energy from Cumulative distr. G4interval [eMin, eMax]

  if( eMin > eMax || eMax < E[0]) return 0.0;
    
  G4int i,j,k=0, iMin=0, iMax=0;

  i=0; j=NoBins;
  while ((j-i)>1) {
    k=(i+j)/2;
    if( E[k] < eMin ) i=k;
    else              j=k;
  }
  iMin = max(0,i-1);

  i=0; j=NoBins;
  while ((j-i)>1) {
    k=(i+j)/2;
    if( E[k] < eMax ) i=k;
    else              j=k;
  }
  iMax = j;
  

  double u = G4UniformRand();
  double r = F[iMin] + (F[iMax] - F[iMin]) * u;

  int a=0, b=NoBins;
  while ((b-a)>1) {
    k=(b+a)/2;
    if( F[k] < r ) a=k;
    else           b=k;
  }

  double x;

  // Delta
  //x = E[b];
  
  double dx = E[b] - E[a];
  double dF = F[b] - F[a];

  //double s = G4UniformRand(); // 2nd rand
  double s = (r - F[a]) / dF;   // from remainder & scaled

  //linear F, step f
  //x = E[a] + s*dx;

  // linear f, sqr F
  double df = f[b] - f[a];
  double sf = f[b] + f[a];
  if( df == 0) x = E[a] + s*dx;
  else         x = E[a] + ( sqrt(f[a]*f[a] + sf*df*s) -f[a]) *dx/df;
  
  if( x > eMax) {
    //cout << "xem " << x << " " << eMin << " " << eMax << endl;
    x = eMax;
  }
  
  return( x );
}




G4double LeptsDistribution::SampleMeanEnergyLost( G4double e) {
  // cout << "smett" << endl;
  // for(int i=0; i<=NoBins; i++)
  //   cout << i << "\t" << E[i] << "\t" << f[i]*Fnorm << "\t" << F[i] << endl;

  if(e < E[0]) return (0.0);
  
  G4int i,j,k=0;

  i=0; j=NoBins;
  while ((j-i)>1) {
    k=(i+j)/2;
    if( e < E[k] ) j=k;
    else           i=k;
  }
  //i = max(0,i-1);
  
  G4double vitp = ( f[i]*(E[i+1]-e) + f[i+1]*(e-E[i]) ) / (E[i+1]-E[i]);
  return ( vitp * Fnorm );

  // cout << "smet " << e << " " << i << " " << E[i] << " " << f[i]*Fnorm
  //      << " " << vitp * Fnorm << endl;
}

