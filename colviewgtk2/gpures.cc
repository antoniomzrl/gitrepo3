
#include "visu.hh"
extern DataObject * d;
extern vector <vec4> posval;
extern vector <vec3> vertices;

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049
string ShowGPUResources() {
  GLint TotalKB = 0, CurAvailKB = 0;
  glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX,   &TotalKB );
  glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &CurAvailKB );
  GLint used = TotalKB -CurAvailKB;
  long int NoTri = posval.size()*vertices.size()/3;

  ostringstream ss;
  ss << "VBO GPU MODE IcoLvl " << d->IcosahedronLevel << " Tri/Ico "
     << vertices.size()/3 << " Npos " << posval.size();
  
  if( NoTri > 1e6) ss << " MTri " << NoTri*1e-6;
  else             ss << " NoTri " << NoTri;
  ss << endl;  
  ss << " mem(MB): " << used/1024 << " / " << TotalKB/1024
     << " used " << endl;

  return ss.str();
}
