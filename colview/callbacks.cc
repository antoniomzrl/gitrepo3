
#include "visu.hh"

extern DataObject * d;
extern GtkWidget * glw, * btw;


void CB_Init(void) {
  cout << "CB Init" << endl;
  glLoadIdentity();
  InitialPosition();
  Invalidate(glw);
}


void CB_InfoWindow(void) {
  cout << "CB InfoWindow" << endl;
  ComputeHistogram(d);
  DisplayInfo(d);
}


void CB_GLInfoWindow(void) {
  cout << "CB GLInfoWindow" << endl;
  DisplayGLInfo();
}


void CB_OpenDataFile(void) {
  string fn = ChooseFile(string("."), string("*.b*") );
  if(fn.empty() ) return;

  cout << "CB_OpenDataFile (" << fn << ")" << endl;
  delete d;

  if( fn.find(".bin") != NPOS)
    d = ReadBinaryFile(fn.c_str(), string("BIN") );
  else if( fn.find(".bcd") != NPOS)
    d = ReadBinaryFile(fn.c_str(), string("BCD") );
  
  ComputeFrame(d);
  Invalidate(glw);
}



void CB_ReOpenDataFile(void) {
  cout << "CB ReOpenDataFile (" << d->FileName << ")" << endl;
  string Fn = d->FileName;
  string Ft = d->FileType;
  cout << "CB ReOpenDataFile (" << Fn << ")" << endl;
  delete d;
  d = ReadBinaryFile(Fn, Ft);
  ComputeFrame(d);
  glLoadIdentity();
  Invalidate(glw);
}


void CB_WriteASCIIFile() {
  cout << "CB_WriteASCIIFile " << endl;
  //WriteASCIIFile(d->r, d->NumDat, d->FileName);
  WriteASCIIFile(d);
}
void CB_WriteBinaryFile() {
  cout << "CB_WriteBinaryFile " << endl;
  //double t1 = myclock();
  WriteBinaryFile(d);
  //double t2 = myclock();
  //WriteBinaryCompressFile(d);
  //double t3 = myclock();
  //cout << "WBF  " << t2-t1 << " sec" << endl << "WBCF " << t3-t2 << " sec" << endl;
}


void CB_SaveImage() {
  //GdkGLContext * glcontext = gtk_widget_get_gl_context(glw);
  //GdkGLDrawable * gldrawable = gtk_widget_get_gl_drawable(glw);
  
  SaveImage(0, 0, gtk_widget_get_allocated_width(glw), gtk_widget_get_allocated_height(glw) );
}



void CB_SwapPar(GtkToggleButton * but, gpointer Data) {
  string dt = string( (char *)Data );
  int bt = gtk_toggle_button_get_active(but);

  if(      dt == "frm" ) d->vFrame = bt;
  else if( dt == "sxy" ) SwapData(d, string(dt));
  else if( dt == "sxz" ) SwapData(d, string(dt));
  else if( dt == "syz" ) SwapData(d, string(dt));
  else if( dt == "sxx" ) SwapData(d, string(dt));
  else if( dt == "syy" ) SwapData(d, string(dt));
  else if( dt == "szz" ) SwapData(d, string(dt));

  else if( dt == "art" ) d->vAutoRotate = bt;

  //else if( dt == "axs" ) d->vAxis = bt;
  //else if( dt == "plb" ) d->vFrameLabels = bt;
  //else if( dt == "trk" ) d->vTracks = bt;
  else if( dt == "cbr" ) d->vColorBar = bt;
  //else if( dt == "b-p" ) d->vBalls = bt;
  // else if( dt == "ctr" ) {
  //   CenterData(d);
  //   ComputeFrame(d);
  // }
  // else if( dt == "crn" ) {
  //   CornerData(d);
  //   ComputeFrame(d);
  // }
  // else if( dt == "tli" ) {
  //   if(bt) Lights();
  //   else   LightsOff();
  // }
  
  Invalidate(glw);
}



void CB_TpiFile(void) {
  string Path = string(".");
  const char * path = getenv("COLVIEWPATH");
  if( path) Path = string(path);

  string fn = ChooseFile(Path, string("tpi*txt") );
  if(fn.empty() ) return;
			   
  InitTpi(fn);
  string Fn = d->FileName;
  string Ft = d->FileType;
  delete d;
  d = ReadBinaryFile(Fn, Ft);
  Invalidate(glw);
}



void CB_PlotType(GtkComboBox * combo) {
  string txt = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo) );
    
  if(     txt == "Particle")    d->vType = 0;
  else if(txt == "Interaction") d->vType = 1;
  else if(txt == "Energy")      d->vType = 3;
  else if(txt == "ELost")       d->vType = 4;
  else if(txt == "EDep")        d->vType = 5;
  
  if(txt == "ColorBar") d->vColorBar = 1;
  else                  d->vColorBar = 0;

  cout << "CB_PlotType " << txt << " " << d->vType << endl;
  d->NeedUpdatePV = true;
  Invalidate(glw);
}




double vls(double M, double m, double v) {
  return ( m + (M-m)*v);
}

unsigned int ScaleStride(double v) {
  double s;
  if( v<1) s = 10.0*v +1.0;
  else     s = pow(10, v);
  cout << "scalestride " << v << " " << s << endl;
  return( s);
}

void CB_ScaleValue(GtkAdjustment * adj, gpointer Data) {

  string dt = string( (char *)Data );
  double vl = gtk_adjustment_get_value(adj);

  if(      dt == "isl" ) { d->IcosahedronLevel = vl; }
  else if( dt == "bsz" ) { d->BallSize = vl*0.3;     }
  else if( dt == "bfc" ) { d->BallFactor = vl;  }
  else if( dt == "fsz" ) d->FontSize = vl;
  else if( dt == "lwd" ) d->LineWidth = vl;
  else if( dt == "bg"  ) d->Background = vl;
  else if( dt == "dpt" ) {d->DiscretizePalette = round(vl);d->NeedUpdatePalette =true;}

  else if( dt == "lpx" ) d->LightPos.x = vls(d->M.x, d->m.x, (1.+vl)/2.);
  else if( dt == "lpy" ) d->LightPos.y = vls(d->M.y, d->m.y, (1.+vl)/2.);
  else if( dt == "lpz" ) d->LightPos.z = vls(d->M.z, d->m.z, (1.+vl)/2.);
  else if( dt == "ldf" ) d->Light.y = vl;
  else if( dt == "lam" ) d->Light.x = vl;
  else if( dt == "lsp" ) d->Light.z = vl;
  else if( dt == "lpw" ) d->Light.w = vl;
  else if( dt == "gat" ) d->greenAttenuation = vl;
  
  //else if( dt == "sm" ) d->ScaleMin = vl/100;
  //else if( dt == "sM" ) d->ScaleMax = vl/100;
  else if( dt == "q0" ) {d->CanvasPoint[0] = vl; d->NeedUpdatePalette = true;}
  else if( dt == "q1" ) {d->CanvasPoint[1] = vl; d->NeedUpdatePalette = true;}
  else if( dt == "q2" ) {d->CanvasPoint[2] = vl; d->NeedUpdatePalette = true;}
  else if( dt == "csc" ) d->vColorScale = vl;
  else if( dt == "cx" ) d->c.x = vls(d->M.x, d->m.x, vl/10);
  else if( dt == "cX" ) d->C.x = vls(d->M.x, d->m.x, vl/10);
  else if( dt == "cy" ) d->c.y = vls(d->M.y, d->m.y, vl/10);
  else if( dt == "cY" ) d->C.y = vls(d->M.y, d->m.y, vl/10);
  else if( dt == "cz" ) d->c.z = vls(d->M.z, d->m.z, vl/10);
  else if( dt == "cZ" ) d->C.z = vls(d->M.z, d->m.z, vl/10);
  else if( dt == "std" )  {
    d->Stride  = ScaleStride(vl);
    d->NeedUpdatePV = true;
  }
  else if( dt == "sstd" ) {
    d->SmartStride = ScaleStride(vl);
    d->NeedUpdatePV = true;
  }
  else if( dt == "cv" ) {
    d->c.f[3] = vls(d->M.f[3], d->m.f[3], vl/10);
    d->c.f[4] = vls(d->M.f[4], d->m.f[4], vl/10);
    d->c.f[5] = vls(d->M.f[5], d->m.f[5], vl/10);
    d->c.f[6] = vls(d->M.f[6], d->m.f[6], vl/10);
    d->c.f[0] = vls(d->M.f[0], d->m.f[0], vl/10);
    d->c.f[1] = vls(d->M.f[1], d->m.f[1], vl/10);
    d->c.f[2] = vls(d->M.f[2], d->m.f[2], vl/10);
  }
  else if( dt == "cV" ) {
    d->C.f[3] = vls(d->M.f[3], d->m.f[3], vl/10);
    d->C.f[4] = vls(d->M.f[4], d->m.f[4], vl/10);
    d->C.f[5] = vls(d->M.f[5], d->m.f[5], vl/10);
    d->C.f[6] = vls(d->M.f[6], d->m.f[6], vl/10);
    d->C.f[0] = vls(d->M.f[0], d->m.f[0], vl/10);
    d->C.f[1] = vls(d->M.f[1], d->m.f[1], vl/10);
    d->C.f[2] = vls(d->M.f[2], d->m.f[2], vl/10);
  }
  else if( dt == "sr" ) {
    d->ScaleX = d->ScaleY = d->ScaleZ = vl/10;
    glLoadIdentity();
    glScalef(d->ScaleX, d->ScaleY, d->ScaleZ);
  }
  else if( dt == "sx" ) {
    d->ScaleX = vl/10;
    glLoadIdentity();
    glScalef(d->ScaleX, d->ScaleY, d->ScaleZ);
  }
  else if( dt == "sy" ) {
    d->ScaleY = vl/10;
    glLoadIdentity();
    glScalef(d->ScaleX, d->ScaleY, d->ScaleZ);
  }
  else if( dt == "sz" ) {
    d->ScaleZ = vl/10;
    glLoadIdentity();
    glScalef(d->ScaleX, d->ScaleY, d->ScaleZ);
  }
  else if( dt == "rsz" ) {
    cout << "Resize xxxxxxxxxx" << vl << endl;
    //  gtk_widget_set_size_request(glw, dim, dim);
    //gtk_drawing_area_size( (GtkDrawingArea *)glw, vl, vl);
    //d->Wsize = vl;
    //Reshape(vl, vl);
  }

  //else if( dt == "lsh" ) d->LightShininess = vl;
  //else if( dt == "lof" ) d->LightShininess = vl;


  // Provisional xxxxx
  Invalidate(glw);
}






// void CB_Interpolate(GtkAdjustment * adj) {
//   int np = (int)adj->value;
//   Interpolate(d, np);
//   Invalidate(glw);
// }


// void CB_MakeHistogram(void) {
//   ComputeHistogram(d);
// }

// void CB_SaveParamFile() {
//   SaveParamFile(d);
// }



void CB_FontInfoWindow(void) {
  cout << "CB FontInfoWindow" << endl;
  DisplayFontList();
}

void CB_FontPlot(void) {
  cout << "CB FontPlot" << endl;
  d->vFontList = 1;
  //PlotFontList();
  Invalidate(glw);
}

void CB_SortData(void) {
  cout << "CB SortData" << endl;
  SortData(d);
  Invalidate(glw);
}

// void CB_FontSelection(GtkWidget *widget, gpointer label) {
//   cout << "CB FontSelection" << endl;
//   GtkWidget * dialog = gtk_font_selection_dialog_new("Select Font");
//   gint result = gtk_dialog_run(GTK_DIALOG(dialog));

//   if(result == GTK_RESPONSE_OK || result == GTK_RESPONSE_APPLY) {
//     gchar * fontname =
//       gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(dialog));
//     PangoFontDescription * font_desc = pango_font_description_from_string(fontname);
//     gtk_widget_modify_font(GTK_WIDGET(btw), font_desc);
//     g_free(fontname);
//     cout << "fontname: "  << fontname << "\nfontdesc" << font_desc << endl;
//     gtk_widget_show_all(btw);
//    }

//   gtk_widget_destroy(dialog);
// }

// void CB_ReadDataFile(GtkWidget * widget, GtkWidget * fileWg) {
//   const char * Fn = gtk_file_selection_get_filename(GTK_FILE_SELECTION(fileWg));
//   cout << "CB ReadDataFile (" << Fn << ")" << endl;
//   delete d;
//   d = ReadBinaryFile((char*)Fn, (char*)"BCD");
//   ComputeFrame(d);
//     Invalidate(glw);
//   gtk_widget_destroy(fileWg);
// }


// void CB_OpenDataFile(void) {
//   cout << "CB OpenDataFile" << endl;
//   GtkWidget * fileWg = gtk_file_selection_new("Select New Data File");
//   g_signal_connect(GTK_FILE_SELECTION(fileWg)->ok_button, "clicked",
// 		   G_CALLBACK(CB_ReadDataFile), fileWg);
//   gtk_signal_connect_object(GTK_OBJECT(GTK_FILE_SELECTION(fileWg)->caddialoncel_button),
// 			    "clicked", GTK_SIGNAL_FUNC(gtk_widget_destroy),
// 			    GTK_OBJECT(fileWg));
//   gtk_widget_show(fileWg);
// }


string ChooseFile(string path, string pattern) {
  string title = path + " " + pattern;
  GtkWidget * dialog =
    gtk_file_chooser_dialog_new(title.c_str(),
				GTK_WINDOW(btw),
				GTK_FILE_CHOOSER_ACTION_OPEN,
				"_Cancel",
				GTK_RESPONSE_CANCEL,
				"_Open",
				GTK_RESPONSE_ACCEPT,
				NULL);

  GtkFileFilter * filter = gtk_file_filter_new ();
  gtk_file_filter_add_pattern (filter, pattern.c_str() );
  gtk_file_chooser_set_filter (GTK_FILE_CHOOSER(dialog), GTK_FILE_FILTER(filter) );
  
  gtk_file_chooser_set_current_folder( GTK_FILE_CHOOSER(dialog), path.c_str() );
  
  string filename;
  if( gtk_dialog_run( GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT ) {
    char * fn = gtk_file_chooser_get_filename( GTK_FILE_CHOOSER(dialog));
    filename = string(fn);
  }
  
  gtk_widget_destroy(dialog);
  return filename;
}




// void CB_OpenDataFile(void) {
//   GtkWidget * dialog =
//     gtk_file_chooser_dialog_new( "Select Data File", (GtkWindow*)btw, GTK_FILE_CHOOSER_ACTION_OPEN,
// 				 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN,
// 				 GTK_RESPONSE_ACCEPT, NULL);

//   if( gtk_dialog_run( GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT ) {
//     const char * Fn = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
//     cout << "CB ReadDataFile (" << Fn << ")" << endl;
//     delete d;
//     d = ReadBinaryFile((char*)Fn, (char*)"BCD");
//     ComputeFrame(d);
//   }
//   gtk_widget_destroy( dialog);
// }



/*void CB_OpenParamFile1(void) {
  cout << "CB ReadParamFile" << endl;
  GtkWidget * fileWg = gtk_file_selection_new("Select Paramet. File");
  g_signal_connect(GTK_FILE_SELECTION(fileWg)->ok_button, "clicked",
		   G_CALLBACK(CB_ReadParamFile), fileWg);
  gtk_signal_connect_object(GTK_OBJECT(GTK_FILE_SELECTION(fileWg)->cancel_button),
			    "clicked", GTK_SIGNAL_FUNC(gtk_widget_destroy),
			    GTK_OBJECT(fileWg));
  gtk_widget_show(fileWg);
}
*/


void CB_ReadParamFile(GtkWidget * widget, GtkWidget * fileWg) {
  //const char * Fn = gtk_file_selection_get_filename(GTK_FILE_SELECTION(fileWg));
  cout << "CB ReadParamFile ( << Fn << )xxxxxxxxx" << endl;

  // ReadParamFile((char*)Fn, d);
  // ComputeFrame(d);
  // Invalidate(glw);
  // gtk_widget_destroy(fileWg);
}


char * ChooseFileFromList(char * filter) {

  GtkWidget * fileWg = gtk_file_chooser_dialog_new("Select Paramet. File",
						   GTK_WINDOW(btw),
						   GTK_FILE_CHOOSER_ACTION_OPEN,
						   "_Cancel",
						   GTK_RESPONSE_CANCEL,
						   "_Open",
						   GTK_RESPONSE_ACCEPT,
						   NULL);
  GtkFileFilter * ff = gtk_file_filter_new();
  gtk_file_filter_add_pattern(ff, filter);
  gtk_file_chooser_add_filter( (GtkFileChooser*)fileWg, ff);

  char * fn;
  if (gtk_dialog_run(GTK_DIALOG(fileWg)) == GTK_RESPONSE_ACCEPT)
    fn = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fileWg));
  else
    fn = NULL;

  gtk_widget_destroy(fileWg);
  return(fn);
}



void CB_OpenParamFile(void) {
  cout << "CB ReadParamFile" << endl;

  char * fn = ChooseFileFromList( (char *)"*.pf");

  if( fn != NULL) {
    ReadParamFile(fn, d);
    ComputeFrame(d);
    Invalidate(glw);
  }

}



/*
int efactor(string unit) {
  int ef;

  if(     unit == "m")  ef = 0;
  else if(unit == "cm") ef = -2;
  else if(unit == "mm") ef = -3;
  else if(unit == "um") ef = -6;
  else if(unit == "nm") ef = -9;
  else if(unit == "Angstrom") ef = -10;

  return(ef);
}



string BestUnit() {
  string BU = "           ";

  int ef = efactor(d->SpatialUnits) - efactor("m");
  double scalef = pow(10, ef);

  ScaleData(d, scalef, scalef, scalef);
  ComputeFrame(d);

  if(      d->rMax > 1    ) BU = "m";
  else if( d->rMax > 0.01 ) BU = "cm";
  else if( d->rMax > 1e-3 ) BU = "mm";
  else if( d->rMax > 1e-6 ) BU = "um";
  else if( d->rMax > 1e-9 ) BU = "nm";
  else if( d->rMax > 1e-10) BU = "Angstrom";

  cout << "BestUnit: " << BU << endl;
  return(BU);
}



void CB_SpatialUnits(GtkComboBox * combo) {

  // Obtain currently selected string from combo box
  string newunit = gtk_combo_box_get_active_text(combo);

  cout << "newunit " << newunit << endl;
  if( newunit == (string)"Best") {
    d->SpatialUnits = "m";
    newunit = BestUnit();
  }

  int ef = efactor(d->SpatialUnits) - efactor(newunit);
  double scalef = pow(10, ef);

  ScaleData(d, scalef, scalef, scalef);
  ComputeFrame(d);
  Invalidate(glw);

  cout << "CB_SpatialUnits (" << d->SpatialUnits << " -> " << newunit
       << ") factor -> " << scalef << endl;
  d->SpatialUnits = newunit;
}
*/








/*
void CB_DataScaleM() {
  ScaleData(d, 10., 10., 10.);
  ComputeFrame(d);
  Invalidate(glw);
}

void CB_DataScaleD() {
  ScaleData(d, 0.1, 0.1, 0.1);
  ComputeFrame(d);
  Invalidate(glw);
}

void CB_AutoRotate() {
  d->vAutoRotate = (d->vAutoRotate+1)%2; //togle 0,1
  cout << "CB_AutoRotate " << d->vAutoRotate << endl;
  Invalidate(glw);
}
*/


// void CB_PlotType() {
//   d->vType = (d->vType+1)%4; //togle 0,1,2,3
//   cout << "CB_PlotType " << d->vType << endl;
//   Invalidate(glw);
// }



// void CB_PlotElastic(GtkToggleButton * but) {
//   d->vElastic = (int)but->active;
//   Invalidate(glw);
// }





/*
void CB_MaxDat(GtkAdjustment * adj) {
  d->MaxDat = (int)adj->value;
  Invalidate(glw);
}

void CB_MinDat(GtkAdjustment * adj) {
  d->MinDat = (int)adj->value;
  Invalidate(glw);
}
*/

