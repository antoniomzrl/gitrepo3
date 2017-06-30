
#include "colv.hh"

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
  
  SaveImage(0, 0, d->PlotSize.x, d->PlotSize.y);
}



void CB_SwapPar(GtkToggleButton * but, gpointer Data) {
  string dt = string( (char *)Data );
  bool bt = gtk_toggle_button_get_active(but);

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


void CB_CompressTpi(void) {
  CompressTpi();
  Invalidate(glw);
}

void CB_PlotType(GtkComboBox * combo) {
  string txt = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo) );
    
  if(     txt == "Particle")    d->ac = 0;
  else if(txt == "Interaction") d->ac = 1;
  else if(txt == "Energy")      d->ac = 3;
  else if(txt == "ELost")       d->ac = 4;
  else if(txt == "EDep")        d->ac = 5;
  
  cout << "CB_PlotType " << txt << " " << d->ac << endl;
  d->NeedUpdatePV = true;
  Invalidate(glw);
}




double vls(double m, double M, double v) {
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

  else if( dt == "lpx" ) d->LightPos.x = vls(d->m.x, d->M.x, (1.+vl)/2.);
  else if( dt == "lpy" ) d->LightPos.y = vls(d->m.y, d->M.y, (1.+vl)/2.);
  else if( dt == "lpz" ) d->LightPos.z = vls(d->m.z, d->M.z, (1.+vl)/2.);
  else if( dt == "ldf" ) d->Light.y = vl;
  else if( dt == "lam" ) d->Light.x = vl;
  else if( dt == "lsp" ) d->Light.z = vl;
  else if( dt == "lpw" ) d->Light.w = vl;
  else if( dt == "gat" ) d->greenAttenuation = vl*0.1;
  
  //else if( dt == "sm" ) d->ScaleMin = vl/100;
  //else if( dt == "sM" ) d->ScaleMax = vl/100;
  else if( dt == "q0" ) {d->CanvasPoint.x = vl; d->NeedUpdatePalette = true;}
  else if( dt == "q1" ) {d->CanvasPoint.y = vl; d->NeedUpdatePalette = true;}
  else if( dt == "q2" ) {d->CanvasPoint.z = vl; d->NeedUpdatePalette = true;}
  else if( dt == "cx" ) d->clip.x = vls(d->m.x, d->M.x, vl/10);
  else if( dt == "cX" ) d->Clip.x = vls(d->m.x, d->M.x, vl/10);
  else if( dt == "cy" ) d->clip.y = vls(d->m.y, d->M.y, vl/10);
  else if( dt == "cY" ) d->Clip.y = vls(d->m.y, d->M.y, vl/10);
  else if( dt == "cz" ) d->clip.z = vls(d->m.z, d->M.z, vl/10);
  else if( dt == "cZ" ) d->Clip.z = vls(d->m.z, d->M.z, vl/10);
  else if( dt == "cv" ) {
    d->clip.f[3] = vls(d->m.f[3], d->M.f[3], vl/10);
    d->clip.f[4] = vls(d->m.f[4], d->M.f[4], vl/10);
    d->clip.f[5] = vls(d->m.f[5], d->M.f[5], vl/10);
    d->clip.f[6] = vls(d->m.f[6], d->M.f[6], vl/10);
    d->clip.f[0] = vls(d->m.f[0], d->M.f[0], vl/10);
    d->clip.f[1] = vls(d->m.f[1], d->M.f[1], vl/10);
    d->clip.f[2] = vls(d->m.f[2], d->M.f[2], vl/10);
  }
  else if( dt == "cV" ) {
    d->Clip.f[3] = vls(d->m.f[3], d->M.f[3], vl/10);
    d->Clip.f[4] = vls(d->m.f[4], d->M.f[4], vl/10);
    d->Clip.f[5] = vls(d->m.f[5], d->M.f[5], vl/10);
    d->Clip.f[6] = vls(d->m.f[6], d->M.f[6], vl/10);
    d->Clip.f[0] = vls(d->m.f[0], d->M.f[0], vl/10);
    d->Clip.f[1] = vls(d->m.f[1], d->M.f[1], vl/10);
    d->Clip.f[2] = vls(d->m.f[2], d->M.f[2], vl/10);
  }
  else if( dt == "std" )  {
    d->Stride  = ScaleStride(vl);
    d->NeedUpdatePV = true;
  }
  else if( dt == "sstd" ) {
    d->SmartStride = ScaleStride(vl);
    d->NeedUpdatePV = true;
  }
  else if( dt == "sr" ) d->Scale = vec3(vl, vl, vl)/10.0f;
  else if( dt == "sx" ) d->Scale.x = vl/10;
  else if( dt == "sy" ) d->Scale.y = vl/10;
  else if( dt == "sz" ) d->Scale.z = vl/10;
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
  //d->vFontList = 1;
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
