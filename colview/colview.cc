
#include "colv.hh"
#include "points.hh"
extern vector <string> tpi, ntpi;
DataObject * d;

bool JoinWindows = false;
GtkWidget *glw, *btw;
float verboseFlag=0, glslVersionFlag=-1.0;
int WidgetSize=0;

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
    if( (string)argv[i]==(string)"-jw") {
      JoinWindows = true;
    }
    if( (string)argv[i]==(string)"-wsize") {
      WidgetSize = atoi( argv[i+1] );
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


//#include <fenv.h> 

int main(int argc, char **argv) {
  //feenableexcept(FE_INVALID | FE_OVERFLOW);

  Init(argc, argv);

  gtk_init(&argc, &argv);

  GtkWidget * tw   = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  GtkWidget * tw2  = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  GtkWidget * box  = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  GtkWidget * box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  GtkWidget * box2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  InitTopWidget(tw, d->FileName.c_str() );

  glw = GetGlWidget();
  
  if( WidgetSize == 0) {
    GdkRectangle * screenSz = ScreenDimensions(tw);
    d->PlotSize = WindowDimensions(screenSz);
    d->FontSize = d->PlotSize.y * 13.0/1000.0;
  }
  else {
    d->PlotSize.x = WidgetSize;
    d->PlotSize.y = WidgetSize *3/4;
  }

  btw = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  InitControlWidget(btw);

  gtk_box_pack_start(GTK_BOX(box1), glw, true, true, 0);
  gtk_box_pack_start(GTK_BOX(box2), btw, true, true, 0);


  if(JoinWindows) {
    gtk_widget_set_size_request(box1, d->PlotSize.x, d->PlotSize.y);
    gtk_widget_set_size_request(box2, 300, d->PlotSize.y);
    gtk_box_pack_start(GTK_BOX(box), box2, true, true, 0);
    gtk_box_pack_start(GTK_BOX(box), box1, true, true, 0);
    gtk_container_add(GTK_CONTAINER(tw), box);
    gtk_widget_show_all(tw);
  }
  else {  
    gtk_window_set_default_size(GTK_WINDOW(tw), d->PlotSize.x, d->PlotSize.y);
    gtk_container_add(GTK_CONTAINER(tw), box1);
    gtk_container_add(GTK_CONTAINER(tw2), box2);
    gtk_window_move(GTK_WINDOW(tw2), 10, 10);
    gtk_widget_show_all(tw2);
    //gtk_window_move(GTK_WINDOW(tw), screenSz->width-d->PlotSize.x-3, 3);
    gtk_widget_show_all(tw);
  }




  // gint mw, nw;
  // gtk_widget_get_preferred_width(btw, &mw, &nw);
  // int aw = gtk_widget_get_allocated_height(btw);
  // cout << "mwnwaw: " << mw << " " << nw << " " << aw   << endl;


  // glewExperimental = GL_TRUE;
  // if( glewInit() != GLEW_OK) {
  //   cout << "Failed to init GLEW: " << glewGetErrorString(glewInit()) << endl;
  //   exit(-1);
  // }
  
  
  InitShadersText();
  InitShadersIcosahedrons();
  InitShadersLines();

  InitialPosition();

  TimerRotate();
  
  gtk_main(); // window manager loop.
}
