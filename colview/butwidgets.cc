
#include "visu.hh"

extern DataObject * d;


void MyClickButton(const gchar * label, GCallback CallBack, GtkWidget * box) {
  GtkWidget * theBut = gtk_button_new_with_label(label);
  g_signal_connect(theBut, "clicked", G_CALLBACK(CallBack), NULL);
  gtk_box_pack_start(GTK_BOX(box), theBut, false, true, 0);
}

void MyHSeparator(GtkWidget * box) {
  GtkWidget * separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_box_pack_start(GTK_BOX(box), separator, false, true, 0);
}

void MyVSeparator(GtkWidget * box) {
  GtkWidget * separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
  gtk_box_pack_start(GTK_BOX(box), separator, false, true, 0);
}


void MyCombo(const gchar * label, GCallback CallBack, GtkWidget * box, string str) {

  GtkWidget * theLabel = gtk_label_new(label);
  //gtk_misc_set_alignment(GTK_MISC(theLabel), 0.8, 0.5);

  GtkWidget * theCombo = gtk_combo_box_text_new();
  istringstream iss(str);
  string word;

  while( iss >> word) {
    cout << "word: " << word << endl;
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(theCombo), NULL, word.c_str());
  }

  gtk_combo_box_set_active(GTK_COMBO_BOX(theCombo), 1);
  g_signal_connect(G_OBJECT(theCombo), "changed", G_CALLBACK(CallBack), NULL);

  GtkWidget * theBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);

  gtk_box_pack_start(GTK_BOX(theBox), theLabel, true, true, 0);
  gtk_box_pack_start(GTK_BOX(theBox), theCombo, true, true, 0);
  gtk_container_add(GTK_CONTAINER(box), theBox);
}


void MyCheckButton(const gchar * label, GCallback CallBack, gpointer Data,
		   GtkWidget * box, gboolean st) {
  GtkWidget * theBut = gtk_check_button_new_with_label(label);
  gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(theBut), st);
  g_signal_connect(theBut, "toggled", G_CALLBACK(CallBack), Data);
  gtk_box_pack_start(GTK_BOX(box), theBut, false, true, 0);
}


// omit gpointer Data:
void MyCheckButton(const gchar * label, GCallback CallBack, GtkWidget * box, gboolean st) {
  MyCheckButton(label, CallBack, NULL, box, st);
}


//omit CallBack:
void MyCheckButton(const gchar * label, gpointer Data, GtkWidget * box, gboolean st) {
  MyCheckButton(label, G_CALLBACK(CB_SwapPar), Data, box, st);
}


void MyLabelScale(const gchar * label, GCallback CallBack, const gchar * Data, GtkWidget * box,
		  float Min, float Max, float Step, float Def) {

  GtkWidget * theLabel = gtk_label_new(label);
  gtk_label_set_xalign(GTK_LABEL(theLabel), 0.1);
  gtk_widget_set_size_request(theLabel, 130,-1);
  
  GtkAdjustment * adj = gtk_adjustment_new(Def, Min, Max, Step, Step, Step);
  g_signal_connect(adj, "value_changed", G_CALLBACK(CallBack), (gpointer)Data);
  GtkWidget * theScale = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL, GTK_ADJUSTMENT(adj));
  gtk_scale_set_value_pos( (GtkScale*)theScale, GTK_POS_RIGHT);
  //gtk_scale_set_digits( (GtkScale*)theScale, 2);
  //gtk_widget_set_size_request(theScale, 300, 0);
  gtk_scale_set_draw_value((GtkScale*)theScale, FALSE);
  //gtk_widget_set_valign(theScale, GTK_ALIGN_START);
  gtk_widget_set_hexpand(theScale, TRUE);
  
  GtkWidget * theBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);   // container label + scale
  gtk_box_pack_start(GTK_BOX(theBox), theLabel, false, true, 0);
  gtk_box_pack_start(GTK_BOX(theBox), theScale, false, true, 0);
  gtk_container_add(GTK_CONTAINER(box), theBox);
}


// omit gpointer Data:
void MyLabelScale(const gchar * label, GCallback CallBack, GtkWidget * box,
		  float Min, float Max, float Step, float Def) {
  MyLabelScale(label, CallBack, NULL, box, Min, Max, Step, Def);
}

// omit CallBack:
void MyLabelScale(const gchar * label, const gchar * Data, GtkWidget * box,
		  float Min, float Max, float Step, float Def) {
  MyLabelScale(label, G_CALLBACK(CB_ScaleValue), Data, box, Min, Max, Step, Def);
}


void InitControlWidget(GtkWidget * butwin) {

  //gint ww, wh;
  //gtk_window_get_size(GTK_WINDOW(window), &ww, &wh);
  //cout << "w width "  << ww << endl;
  //cout << "w height " << wh << endl;
  //gtk_window_move(GTK_WINDOW(window), gdk_screen_width()-ww-100, 3);

  //gtk_window_move(GTK_WINDOW(butwin), 3, 3);
  //gtk_window_set_title(GTK_WINDOW(window), "gtk demo");
  //gtk_window_set_title(GTK_WINDOW(butwin), d->FileName.c_str() );

  g_signal_connect(G_OBJECT(butwin), "delete_event", G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect(G_OBJECT(butwin), "destroy",      G_CALLBACK(gtk_main_quit), NULL);
  gtk_container_set_border_width(GTK_CONTAINER(butwin), 5);

  // containers
  GtkWidget * notebook = gtk_notebook_new();
  //gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook), GTK_POS_TOP);
  
  GtkWidget * boxa  = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget * boxa12= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  GtkWidget * boxa1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget * boxa2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget * boxa3 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget * boxb  = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget * boxb1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget * boxb2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget * boxc  = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget * boxd  = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget * boxe  = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

  
  // click buttons
  MyClickButton("Quit",            G_CALLBACK(gtk_main_quit),     boxa1);
  MyClickButton("Init",            G_CALLBACK(CB_Init),           boxa1);
  MyClickButton("Info",            G_CALLBACK(CB_InfoWindow),     boxa1);
  MyClickButton("Read New File",   G_CALLBACK(CB_OpenDataFile),   boxa1);
  MyClickButton("ReOpen File",     G_CALLBACK(CB_ReOpenDataFile), boxa1);
  MyClickButton("Save ASCII",      G_CALLBACK(CB_WriteASCIIFile), boxa1);
  MyClickButton("Save Bin",        G_CALLBACK(CB_WriteBinaryFile),boxa1);
  //  MyClickButton("Save Param. File",G_CALLBACK(CB_SaveParamFile),  boxa1);
  //  MyClickButton("Read Param. File",G_CALLBACK(CB_OpenParamFile),  boxa1);
  MyClickButton("Save Image",      G_CALLBACK(CB_SaveImage),      boxa1);

  //MyClickButton("AutoRotate",      G_CALLBACK(CB_AutoRotate),     boxa1);

  // MyHSeparator(boxa1);

  // checkbuttons
  //MyCheckButton("Center Data", (gpointer)"ctr", boxa2, false);
  //MyCheckButton("Corner Data", (gpointer)"crn", boxa2, false);
  //MyCheckButton("Plot Axis",   (gpointer)"axs", boxa2, true);
  //MyCheckButton("Plot Labels", (gpointer)"plb", boxa2, true);
  //MyCheckButton("Plot ColorBar", (gpointer)"cbr", boxa2, false);
  //MyCheckButton("Ball Point",  (gpointer)"b-p", boxa2, false);
  MyCheckButton("Frame",       (gpointer)"frm", boxa2, true);
  MyCheckButton("Swap X Y",    (gpointer)"sxy", boxa2, true);
  MyCheckButton("Swap X Z",    (gpointer)"sxz", boxa2, true);
  MyCheckButton("Swap Y Z",    (gpointer)"syz", boxa2, true);
  MyCheckButton("Swap Y -Y",   (gpointer)"syy", boxa2, true);
  MyCheckButton("Swap Z -Z",   (gpointer)"szz", boxa2, true);
  MyCombo("", G_CALLBACK(CB_PlotType), boxa2, "Particle Interaction Energy ELost EDep");

  //MyCombo("Spatial Units:",   G_CALLBACK(CB_SpatialUnits),boxa3, "mm μm nm Angstrom Best");

  MyClickButton("TPI File",    G_CALLBACK(CB_TpiFile), boxa2);
  MyClickButton("Compress SB",    G_CALLBACK(CB_CompressTpi), boxa2);

  MyLabelScale("Icosah Lvl   ", "isl",  boxa3,  0,   6, 1,    0);
  MyLabelScale("Ball Size    ", "bsz",  boxa3,  0,   9, 0.1, d->BallSize/0.3);
  MyLabelScale("Size Factor  ", "bfc",  boxa3,  0,   2, 0.1,  0.2);
  MyLabelScale("Background   ", "bg",   boxa3,  0,   1, 0.1,  0);
  MyLabelScale("Stride       ", "std",  boxa3,  0,   9, 0.1,  0);
  MyLabelScale("Smart Stride ", "sstd", boxa3,  0,   9, 0.1,  0);
  MyLabelScale("Line Width   ", "lwd",  boxa3,  0,   5, 0.1,  1);
  MyLabelScale("Canvas Max   ", "q2",   boxa3,-0.5, 1.5, 0.1,  1);
  MyLabelScale("Canvas Med   ", "q1",   boxa3,   0,   1, 0.1,  0.5);
  MyLabelScale("Canvas Min   ", "q0",   boxa3,-0.5, 1.5, 0.1,  0);
  MyLabelScale("Palette Discr", "dpt",  boxa3,  1,  18, 1,   16);
  MyLabelScale("Font Size    ", "fsz",  boxa3,   5,  40,   1,  d->FontSize);

  //MyCheckButton("Swap Z -Z",    G_CALLBACK(CB_SwapDataZZ), boxa1, false);
  //MyHSeparator(boxa1);

  // labeled Scales
  MyLabelScale("Background:    ", "bg",  boxb1,  0, 1,  0.1, 0);
  MyLabelScale("Light PosX:    ", "lpx", boxb1, -5, 5,  1,   1);
  MyLabelScale("Light PosY:    ", "lpy", boxb1, -5, 5,  1,   1);
  MyLabelScale("Light PosZ:    ", "lpz", boxb1, -5, 5,  1,   1);
  MyLabelScale("Light Int:     ", "lpw", boxb1,  0, 5,  0.1, 0);
  MyLabelScale("green Attenu:  ", "gat", boxb1,  0, 5,  0.1, d->greenAttenuation*10);
  MyLabelScale("LightDiffuse : ", "ldf", boxb1,  0, 1,  0.1, 0.9);
  MyLabelScale("LightAmbient : ", "lam", boxb1,  0, 1,  0.1, 0.4);
  MyLabelScale("LightSpecular: ", "lsp", boxb1,  0, 1,  0.1, 0.9);
  MyLabelScale("LightShininess:", "lsh", boxb1,  0, 1,  0.1, 0.2);
  //  MyCheckButton("Lights On/Off", (gpointer)"tli", boxb2, true);

  //  MyHSeparator(boxa2);
  MyLabelScale("Scale  ",     "sr",  boxc, 0.1, 20, 0.1, 10);
  MyLabelScale("Scale X",     "sx",  boxc, 0.1, 20, 0.1, 10);
  MyLabelScale("Scale Y",     "sy",  boxc, 0.1, 20, 0.1, 10);
  //MyLabelScale("Scale Z",     "sz",  boxc, 0.1, 20, 0.1, 10);
  //MyLabelScale("ColorSc",     "csc", boxc, 0,    7, 1 ,  0);

  MyLabelScale("Clip x min:", "cx", boxd, 0, 10, 0.1,  0);
  MyLabelScale("Clip x MAX:", "cX", boxd, 0, 10, 0.1, 10);
  MyLabelScale("Clip y min:", "cy", boxd, 0, 10, 0.1,  0);
  MyLabelScale("Clip y MAX:", "cY", boxd, 0, 10, 0.1, 10);
  MyLabelScale("Clip z min:", "cz", boxd, 0, 10, 0.1,  0);
  MyLabelScale("Clip z MAX:", "cZ", boxd, 0, 10, 0.1, 10);
  MyLabelScale("Clip v min:", "cv", boxd, 0, 10, 0.1,  0);
  MyLabelScale("Clip v MAX:", "cV", boxd, 0, 10, 0.1, 10);

  MyClickButton("GL Info", G_CALLBACK(CB_GLInfoWindow),   boxe);
  MyClickButton("Fonts TXT",   G_CALLBACK(CB_FontInfoWindow), boxe);
  MyClickButton("Fonts GL",    G_CALLBACK(CB_FontPlot), boxe);
  //  MyLabelScale("Resize Win:",  "rsz", boxe, 400, 2500, 100, d->Wsize);
  MyClickButton("Sort Data",   G_CALLBACK(CB_SortData), boxe);
  MyCheckButton("AutoRotate",  (gpointer)"art", boxe, false);
  //MyCheckButton("Plot Traj.",  (gpointer)"trk", boxe, false);

  gtk_container_add(GTK_CONTAINER(boxa12), boxa1);
  MyVSeparator(boxa12);
  gtk_container_add(GTK_CONTAINER(boxa12), boxa2);

  gtk_container_add(GTK_CONTAINER(boxa), boxa12);
  MyHSeparator(boxa);
  gtk_container_add(GTK_CONTAINER(boxa), boxa3);

  gtk_container_add(GTK_CONTAINER(boxb), boxb1);
  MyHSeparator(boxb);
  gtk_container_add(GTK_CONTAINER(boxb), boxb2);

  gtk_container_add(GTK_CONTAINER(butwin), notebook);

  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), boxa,  gtk_label_new("Main  ") );
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), boxb,  gtk_label_new("Lights") );
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), boxc,  gtk_label_new("Scale") );
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), boxd,  gtk_label_new("Clip") );
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), boxe,  gtk_label_new("Misc") );
}
