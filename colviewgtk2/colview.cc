
#include "visu.hh"
#include "points.hh"
extern vector <string> tpi, ntpi;

bool Debug = true;

extern DataObject * d;
GtkWidget * tw, * glw;

float verboseFlag=0, glslVersionFlag=-1.0;

void Init(int argc, char **argv) {
  string TpiFn = string("tpi_all.txt");

  for(int i=0; i<argc; i++) {
    if( (string)argv[i]==(string)"-tpi") {
      TpiFn = string( argv[i+1] );
    }
    if( (string)argv[i]==(string)"-glsl") {
      glslVersionFlag = atof( argv[i+1] );
    }
    if( (string)argv[i]==(string)"-v") {
      verboseFlag = atof( argv[i+1] );
      if(verboseFlag < 1) verboseFlag = 1;
    }
  }

  InitTpi(TpiFn);
  if(verboseFlag >6)
    for(int i=tpi.size()-1; i>=0; i--)
      cout << setw(4) << TPI(tpi[i]) << setw(40) << tpi[i]
	   << setw(20) << ntpi[i] << endl;
  

  // 1st try 
  for(int i=0; i<argc; i++) {
    if( (string)argv[i]==(string)"-pet") { //PET
      d = ReadPetFile(argv[i+1]);
    }
    if( (string)argv[i]==(string)"-i") { //bin. file
      d = ReadBinaryFile(string(argv[i+1]), string("BCD") );
    }
    if( (string)argv[i]==(string)"-arce") { //bin. file
      d = ReadBinaryFile(string(argv[i+1]), string("ARCE") );
    }
    if( (string)argv[i]==(string)"-ascii" || (string)argv[i]==(string)"-txt") {
      d = ReadInputFile(argv[i+1]);
    }
    if( (string)argv[i]==(string)"-convert") { //bin. file
      d = ReadBinaryFile(string(argv[i+1]), string("BIN") );
      WriteBinaryFile(d, argv[i+2]);
      exit(0);
    }
  }

  // 2nd try 
  if( d == NULL ) {
    if( argc > 1 ) {
      if( strstr(argv[argc-1], ".bcd.gz") != NULL)
	d = ReadBinaryFile(string(argv[argc-1]), string("BCDGZ") );
      else if( strstr(argv[argc-1], ".bin") != NULL)
	d = ReadBinaryFile(string(argv[argc-1]), string("BIN") );
      else if( strstr(argv[argc-1], ".bcd") != NULL)
	d = ReadBinaryFile(string(argv[argc-1]), string("BCD"));
      else if((string)argv[argc-1]==(string)".txt" || (string)argv[argc-1]==(string)".ascii")
	d = ReadInputFile(argv[argc-1] );
    }
  }

  
  if( d == NULL ) {
    string fn = ChooseFile(string("."), string("*.b*") );
    if( !fn.empty() ) {
      if( fn.find(".bin") != NPOS)
	d = ReadBinaryFile(fn.c_str(), string("BIN") );
      else if( fn.find(".bcd") != NPOS)
	d = ReadBinaryFile(fn.c_str(), string("BCD") );
    }
  }

  ComputeFrame(d);
  CenterData(d);
  ComputeFrame(d);
}


//static void Activate(GtkApplication * app, gpointer user_data) {
void Activate() {
  tw  = CreateTopWidget( d->FileName.c_str() );
  glw = CreateGlWidget( tw);
  gtk_widget_show_all(tw);
  //gtk_window_present(GTK_WINDOW(glw) );
  //InitGLEW();  // GLEW after glwidget is showed

  
  ButtonWidgets();
  
  InitShadersIcosahedrons();
  InitShadersLines();
  InitShadersText();

   TimerRotate();
}


//#include <fenv.h> 

int main(int argc, char **argv) {
  //feenableexcept(FE_INVALID | FE_OVERFLOW);

  Init(argc, argv);

  gtk_init(&argc, &argv);
  //gtk_gl_init(&argc, &argv);
  //glutInit(&argc, argv);

  // GtkApplication * app = gtk_application_new("colview", G_APPLICATION_FLAGS_NONE);
  // g_signal_connect(app, "activate", G_CALLBACK(Activate), NULL);
  // g_application_run(G_APPLICATION (app), argc, argv);
  Activate();
  
  gtk_main(); // window manager loop.
}
