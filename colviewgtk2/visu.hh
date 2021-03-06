
#ifndef _VISU_HH_
#define _VISU_HH_

	 
#include <string.h>


#include <limits>    //Max datatypes
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib> // for exit function

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <cstdlib>
#include <sstream>
#include <stdlib.h> 

#define NPOS std::string::npos

//#include <iostream.h>
//#include <iomanip.h>

#ifdef WIN32
#include <windows.h>
#include <sys/types.h>
#define GLEW_STATIC
#endif

//#include <GL/glew.h>
#include <epoxy/gl.h>


#include <gtk/gtk.h>
#include <gtk/gtkgl.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <gdk/gdkkeysyms.h>

//#include <gl\gl.h>
//#include <GL/glut.h>
//#include <FL/glut.h> // fltk

#include <fcntl.h>
#include <zlib.h>


#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/norm.hpp>
#include <glm/gtc/constants.hpp>
using namespace glm;
using namespace std;


#include "points.hh"


#include <vector>
#include <algorithm> //sort


typedef struct DataObject {
  string FileName, FileType;
  unsigned long NumDat, MinDat, MaxDat;
  double EdepTotal;
  //GLfloat xm, ym, zm, tm, em, lm, pm;
  //GLfloat xM, yM, zM, tM, eM, lM, pM;        // Max
  //GLfloat cxm, cym, czm, ctm, cem, clm, cpm; // Clip
  //GLfloat cxM, cyM, czM, ctM, ceM, clM, cpM;
  Point m, M, c, C; // min, max, clip min, clip max
  GLfloat xc, yc, zc, xn, yn, zn, wn, Radius, Diameter;
  GLfloat xLeft, xRight, yBottom, yTop, zNear, zFar, Fv, rMax, Rv, Volume;
  GLfloat BallSize, BallFactor, BallScale, Background, DiscretizePalette;
  GLfloat CanvasPoint[3];
  vec4 Light; //Ambient, Diffuse, Specular, Intensity
  vec3 LightPos;
  float greenAttenuation;
  int vAxis, vFrame, vFrameLabels, vColorBar, 
    vFontList, vSelection, vEconomic, vColorScale,
    vLorLines, vBalls, vTracks,
    vElastic, vType, vAutoRotate, vID;
  string Label[10];
  
  int IcosahedronLevel, Stride, SmartStride;
  int FontSize;
  float LineWidth;
  GLfloat ScaleX, ScaleY, ScaleZ;
  GLfloat xBegin, yBegin, xEnd, yEnd; // Mouse
  float rcx, rcy, rcz; //rot. cent
  float Wsize;
  float  SpatialFactor;
  string SpatialUnits;
  int verboseOut;

  bool NeedUpdatePV, NeedRepaint, NeedUpdatePalette;
  
  unsigned long Hist[100]; //type of interaction
  double Hist2[100];       //edep per interaction

  //Point * r;
  vector<Point> r;

  DataObject(); // constructor
  DataObject(int);
} DataObject;



typedef struct ShaderID {
  GLuint programID,
    mvpID, imvID,  sfID, bfID, // uniforms in shaders
    pltID, pltSzID, pltDtzID,
    LightPosID, LightID, LightColorID, greenAttenID, LineColorID,
    vtxID,  nmlID,   pvlID,                   //layout in shader
    vtxGPU, nmlGPU,  pvlGPU;                  //GPU buffers
} ShaderID;


// Prototypes
void PlotFrame(vec3, vec3, vec4); 
void PlotAxis(DataObject *);
void PlotSelection(DataObject *);
void PlotOrthoLines();
void PlotColorBarLines();
void UpdateLines();
void PlotColorBarText();

void PlotLorLines(DataObject *);
void PlotTracks(DataObject *);
void PlotPoints(DataObject *);
void PlotBalls(DataObject *);

void InitialPosition();
void RotateScene(int, int, int);
void ScaleScene(int );
void TranslateScene(int, int);
void InitGLEW();

void InitShadersIcosahedrons();
void InitShadersLines();
void InitShadersText();
ShaderID * GetShaderIDs( const char *);
void SetAttrib(GLint, GLint, GLint, GLuint);
void UpdateGpuBuffer(GLuint, unsigned int, void *);
void UpdateTransformMatrices();
string AxisToMe();

// update gpu-buffer
void Draw(void);
void DrawInfo(void);
void Reshape(int, int);
void ReshapeInfo(int, int);

#include <gtk/gtk.h>
gboolean AutoRotate( gpointer );

void ZoomView(DataObject *);
void SwapData(DataObject *, string);
// void SwapDataXY(DataObject *);
// void SwapDataXZ(DataObject *);
// void SwapDataYZ(DataObject *);
// void SwapDataYY(DataObject *);
// void SwapDataZZ(DataObject *);
bool InClip(Point);
void ClipData(DataObject *);
void ClipDataAndCenter(DataObject *);
void ClipDataAndCorner(DataObject *);
void CommitRotation();
void CommitRotationHalfPi();
void BestSpatialUnit(DataObject *);
void ComputeFrame(DataObject *);
void ScaleBestUnit(DataObject *);
void SortData(DataObject *);
void ComputeHistogram(DataObject *);
void ComputePositions(DataObject *);
void UpdatePCS(DataObject *);
void UpdateP(DataObject *);
void UpdatePV(DataObject *);
void UpdatePalette();
void UpdateCS(DataObject *);
void UpdateSizes(DataObject *);

string GLInfo();

void DisplayInfo(DataObject *);
void DisplayNearData(double, double, DataObject *);
void DisplayGLInfo();
void DisplayFontList();
void PlotFontList();
void CenterData(DataObject *);
void ShiftData(DataObject *, float, float, float);
void CornerData(DataObject *);
void ScaleData(DataObject *, float, float, float);

vec3 ComputeColor(float);

float Interpolate(float, float, float, float, float);
void LightsOff(void);
void Lights(void);

float BestNear(float);
void PlotLegend(DataObject *);
void PlotFrameLabels(DataObject *);
void PlotColorBar(float, float, float, int, string);
void PlotColorBar(float, float, float, float, float, float, string);
void PlotColorBars(DataObject *);
void PlotColorBar(DataObject *);
void PlotText(float, float, float, char *, void *);
void PlotText(float, float, float, char *);
void invertMatrix(const GLdouble *, GLdouble *);
void ComputeSphereVertex(double, int, int);
void PlotSphere(int);

void ComputeIcosahedronVertex(int);
void PlotIcosahedrons();
GLuint LoadShaders(const char *,const char *);
GLuint LoadShaders(const char *);
string ShowGPUResources();
void RenderText(string, float, float, float, vec3);

unsigned long GetFileSize(char *);
unsigned long GetBinaryFileSize(const char *);
//void WriteASCIIFile(vector<Point>, int, char *);
void WriteASCIIFile(DataObject *);
void WriteBinaryFile(DataObject *, char *);
void WriteBinaryFile(DataObject *);
void WriteBinaryCompressFile(DataObject *, char *);
void WriteBinaryCompressFile(DataObject *);
void SaveImage(int, int, int, int);
void SaveParamFile(DataObject *);
void ReadParamFile(char *, DataObject *);
DataObject * ReadPetFile(char *);
DataObject * ReadInputFile(char *);
DataObject * ReadBinaryFile(string, string);
double myclock();
string GetAbsoluteFile(string);

void Invalidate(GtkWidget *);
gboolean configure(GtkWidget *, GdkEventConfigure *, gpointer);
gboolean configureInfo(GtkWidget *, GdkEventConfigure *, gpointer);
gboolean exposeInfo(GtkWidget *, GdkEventExpose *, gpointer);
gboolean expose(GtkWidget *, GdkEventExpose *, gpointer);
gboolean keyPress(GtkWidget *, GdkEventKey *);
gboolean butPress(GtkWidget *, GdkEventButton *);
gboolean butRelease(GtkWidget *, GdkEventButton *);
gboolean mouseMotion(GtkWidget *, GdkEventMotion *);
gboolean butScroll(GtkWidget *, GdkEventScroll * );

void printgsl( mat4 );
void printgsl( vec4 );
void printgsl( vec3 );
void printDbg(int, string, string);
void printDbg(int, string, double, string, string);
void printDbg(int, string);

void ButtonWidgets(void);
//void GlWidget(char *);
GtkWidget * CreateTopWidget(const char *); 
GtkWidget * CreateGlWidget(GtkWidget *);
void MakeGlWidgetColorBar(void);
void TimerRotate();

void Interpolate(DataObject *, int);

// callbacks:
//void CB_PlotElastic(GtkToggleButton *);
//void CB_MaxDat(GtkAdjustment *);
//void CB_MinDat(GtkAdjustment *);
void CB_Interpolate(GtkAdjustment *);
void CB_SortData(void);

//void CB_AutoRotate();
//void CB_BackGround(GtkAdjustment *);



void CB_Init(void);
void CB_MakeHistogram(void);
void CB_InfoWindow(void);
void CB_GLInfoWindow(void);
void CB_FontInfoWindow(void);
void CB_FontPlot(void);

void CB_ReadDataFile(GtkWidget *, GtkWidget *);
void CB_FontSelection(GtkWidget *, gpointer);
void CB_OpenDataFile(void);


string ChooseFile(string, string);
void CB_TpiFile(void);
void CB_ReOpenDataFile(void);
void CB_WriteBinaryFile();
void CB_WriteASCIIFile();
void CB_SaveImage();
void CB_SaveParamFile();
void CB_OpenParamFile(void);
void CB_OpenDataFile(void);

void CB_PlotType(GtkComboBox * );

void CB_SwapPar(GtkToggleButton *, gpointer);
void CB_ScaleValue(GtkAdjustment *, gpointer);

#endif //_VISU_HH_
