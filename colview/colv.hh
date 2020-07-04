
#ifndef _COLV_HH_
#define _COLV_HH_

	 
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


#define NPOS std::string::npos

//#include <iostream.h>
//#include <iomanip.h>

#ifdef WIN32
#include <windows.h>
#include <sys/types.h>
//#define GLEW_STATIC
#endif

//#include <GL/glew.h>
#include <epoxy/gl.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

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
  float BallSize, BallFactor, BallScale,
    Background, FontSize, LineWidth,
    DiscretizePalette, greenAttenuation,
    FoV, AR,
    theHorizAngle, theVertAngle, theFrontAngle, theZoom,
    theHorizDisp, theVertDisp, theDepthDisp;

  int IcosahedronLevel, Stride, SmartStride;
    
  vec4 Light; //Ambient, Diffuse, Specular, Intensity
  vec3 LightPos, CanvasPoint, Scale;
  bool vFrame, vSelection, vEconomic, vAutoRotate;
  vec4 MouseSelection; // xBegin, yBegin, xEnd, yEnd
  vec3 PlotSize;

  int ac; // active column

  
  float  SpatialFactor;
  string SpatialUnits;
  double EdepTotal;

  string FileName, FileType;

  Point m, M, clip, Clip; // min, max, center, semi-width, clip min, clip max
  vec3 c;  // center
  vec4 sw; // semiwidth x, y, z, max
  bool NeedUpdatePV, NeedUpdatePalette;
  
  unsigned long Hist[100]; //type of interaction
  double Hist2[100];       //edep per interaction

  vector<Point> r;

  DataObject(); // constructor
  DataObject(int);
} DataObject;




// Prototypes
void PlotFrame(vec3, vec3, vec4); 
void PlotAxis(DataObject *);
void PlotSelection(DataObject *);
void PlotOrthoLines();
void PlotColorBarLines();
void UpdateLines();
void PlotColorBarText();

void DisplayWindowText(string str);

void PlotLorLines(DataObject *);
void PlotTracks(DataObject *);
void PlotPoints(DataObject *);
void PlotBalls(DataObject *);

void InitialPosition();
void RotateScene(int, int, int);
void ScaleScene(int );
void TranslateScene(int, int);

void InitShadersIcosahedrons();
void InitShadersLines();
void InitShadersText();
void BindAndAttrib(GLuint, GLuint, GLuint, GLuint, GLuint);
void UpdateGpuBuffer(GLuint, unsigned int, void *);
void UpdateTransformMatrices();

// update gpu-buffer
void Draw(void);
void DrawInfo(void);
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
void RenderText(string, vec3, vec3, vec3);

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
gboolean configure(GtkGLArea *, GdkEventConfigure *, gpointer);
gboolean expose(GtkGLArea *, GdkEventExpose *, gpointer);
gboolean resize(GtkGLArea *, gint, gint, gpointer);
gboolean keyPress(GtkWidget *, GdkEventKey *);
gboolean keyRelease(GtkWidget *, GdkEventKey *);
gboolean butPress(GtkWidget *, GdkEventButton *);
gboolean butRelease(GtkWidget *, GdkEventButton *);
gboolean mouseMotion(GtkWidget *, GdkEventMotion *);
gboolean butScroll(GtkWidget *, GdkEventScroll * );

void printglm( mat4 );
void printglm( vec4 );
void printglm( vec3 );
void printDbg(int, string, string);
void printDbg(int, string, double, string, string);
void printDbg(int, string);

GtkWidget * GetControlWidget();

//void GlWidget(char *);
GtkWidget * GetTopWidget(const char *); 
//void InitGlWidget(GtkWidget *);
GtkWidget * GetGlWidget();
GdkRectangle * ScreenDimensions();
vec4 WindowDimensions(GdkRectangle *);
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
void CB_CompressTpi(void);
void CompressTpi(void);
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

#endif //_COLV_HH_
