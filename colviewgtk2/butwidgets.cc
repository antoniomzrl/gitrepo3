
#include "visu.hh"

extern DataObject * d;


GtkWidget * butwin;



void MyClickButton(const gchar * label, GtkSignalFunc CallBack, GtkWidget * box) {
  GtkWidget * theBut = gtk_button_new_with_label(label);
  g_signal_connect(theBut, "clicked", G_CALLBACK(CallBack), NULL);
  gtk_box_pack_start(GTK_BOX(box), theBut, false, true, 0);
}

void MyHSeparator(GtkWidget * box) {
  GtkWidget * separator = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(box), separator, false, true, 0);
}

void MyVSeparator(GtkWidget * box) {
  GtkWidget * separator = gtk_vseparator_new();
  gtk_box_pack_start(GTK_BOX(box), separator, false, true, 0);
}


void MyCombo(const gchar * label, GtkSignalFunc CallBack, GtkWidget * box, string str) {

  GtkWidget * theLabel = gtk_label_new(label);
  gtk_misc_set_alignment(GTK_MISC(theLabel), 0.8, 0.5);

  GtkWidget * theCombo = gtk_combo_box_new_text();
  istringstream iss(str);
  string word;

  while( iss >> word) {
    //cout << "word: " << word << endl;
    gtk_combo_box_append_text(GTK_COMBO_BOX(theCombo), word.c_str());
  }

  gtk_combo_box_set_active(GTK_COMBO_BOX(theCombo), 1);
  g_signal_connect(G_OBJECT(theCombo), "changed", G_CALLBACK(CallBack), NULL);

  GtkWidget * theBox = gtk_hbox_new(true, 1);
  //GtkWidget * theBox = gtk_vbox_new(true, 1);

  gtk_box_pack_start(GTK_BOX(theBox), theLabel, true, true, 0);
  gtk_box_pack_start(GTK_BOX(theBox), theCombo, true, true, 0);
  gtk_container_add(GTK_CONTAINER(box), theBox);
}



void MyCheckButton(const gchar * label, GtkSignalFunc CallBack, gpointer Data,
		   GtkWidget * box, gboolean st) {
  GtkWidget * theBut = gtk_check_button_new_with_label(label);
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(theBut), st);
  g_signal_connect(theBut, "toggled", G_CALLBACK(CallBack), Data);
  gtk_box_pack_start(GTK_BOX(box), theBut, false, true, 0);
}


// omit gpointer Data:
void MyCheckButton(const gchar * label, GtkSignalFunc CallBack, GtkWidget * box, gboolean st) {
  MyCheckButton(label, CallBack, NULL, box, st);
}


//omit CallBack:
void MyCheckButton(const gchar * label, gpointer Data, GtkWidget * box, gboolean st) {
  MyCheckButton(label, G_CALLBACK(CB_SwapPar), Data, box, st);
}


void MyLabelScale(const gchar * label, GtkSignalFunc CallBack, gpointer Data, GtkWidget * box,
		  float Min, float Max, float Step, float Def) {

  GtkWidget * theLabel = gtk_label_new(label);
  //gtk_misc_set_alignment(GTK_MISC(theLabel), 0.2, 0.8);
  //gtk_widget_set_size_request(theLabel, 100, 20);
  
  GtkObject * adj = gtk_adjustment_new(Def, Min, Max, Step, Step, Step);
  g_signal_connect(adj, "value_changed", G_CALLBACK(CallBack), Data);
  GtkWidget * theScale = gtk_hscale_new(GTK_ADJUSTMENT(adj));
  gtk_scale_set_value_pos( (GtkScale*)theScale, GTK_POS_RIGHT);
  //gtk_scale_set_digits( (GtkScale*)theScale, 2);
  //gtk_widget_set_size_request(theScale, 300, 0);
  //gtk_scale_set_draw_value((GtkScale*)theScale, FALSE);
    
  GtkWidget * theBox = gtk_hbox_new(true, 1);   // container label + scale
  //gtk_box_set_homogeneous(GTK_BOX(theBox), false);
  gtk_box_pack_start(GTK_BOX(theBox), theScale, false, true, 0);
  gtk_box_pack_start(GTK_BOX(theBox), theLabel, false, true, 0);
  gtk_container_add(GTK_CONTAINER(box), theBox);
}


// omit gpointer Data:
void MyLabelScale(const gchar * label, GtkSignalFunc CallBack, GtkWidget * box,
		  float Min, float Max, float Step, float Def) {
  MyLabelScale(label, CallBack, NULL, box, Min, Max, Step, Def);
}

// omit CallBack:
void MyLabelScale(const gchar * label, gpointer Data, GtkWidget * box,
		  float Min, float Max, float Step, float Def) {
  MyLabelScale(label, G_CALLBACK(CB_ScaleValue), Data, box, Min, Max, Step, Def);
}


void ButtonWidgets(void) {

  // widgets window
  butwin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_move(GTK_WINDOW(butwin), 3, 3);

  //gint ww, wh;
  //gtk_window_get_size(GTK_WINDOW(window), &ww, &wh);
  //cout << "w width "  << ww << endl;
  //cout << "w height " << wh << endl;
  //gtk_window_move(GTK_WINDOW(window), gdk_screen_width()-ww-100, 3);

  //gtk_window_set_title(GTK_WINDOW(window), "gtk demo");
  gtk_window_set_title(GTK_WINDOW(butwin), d->FileName.c_str() );

  gtk_signal_connect(GTK_OBJECT(butwin), "delete_event", GTK_SIGNAL_FUNC(gtk_main_quit), NULL);
  gtk_signal_connect(GTK_OBJECT(butwin), "destroy", GTK_SIGNAL_FUNC(gtk_main_quit), NULL);
  gtk_container_set_border_width(GTK_CONTAINER(butwin), 5);

  // containers
  GtkWidget * notebook = gtk_notebook_new();
  //gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook), GTK_POS_TOP);
  
  GtkWidget * boxa  = gtk_vbox_new(false, 5);
  GtkWidget * boxa12= gtk_hbox_new(false, 5);
  GtkWidget * boxa1 = gtk_vbox_new(false, 5);
  GtkWidget * boxa2 = gtk_vbox_new(false, 5);
  GtkWidget * boxa3 = gtk_vbox_new(false, 5);
  GtkWidget * boxb  = gtk_vbox_new(false, 5);
  GtkWidget * boxb1 = gtk_vbox_new(false, 5);
  GtkWidget * boxb2 = gtk_vbox_new(false, 5);
  GtkWidget * boxc  = gtk_vbox_new(false, 5);
  GtkWidget * boxd  = gtk_vbox_new(false, 5);
  GtkWidget * boxe  = gtk_vbox_new(false, 5);
  

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
  MyCheckButton("Swap X Y",    (gpointer)"sxy", boxa2, false);
  MyCheckButton("Swap X Z",    (gpointer)"sxz", boxa2, false);
  MyCheckButton("Swap Y Z",    (gpointer)"syz", boxa2, false);
  MyCheckButton("Swap Y -Y",   (gpointer)"syy", boxa2, false);
  MyCheckButton("Swap Z -Z",   (gpointer)"szz", boxa2, false);
  MyClickButton("TPI File",    G_CALLBACK(CB_TpiFile), boxa2);

  MyCombo("Plot type:",        G_CALLBACK(CB_PlotType), boxa3,
	  "Particle Interaction Energy ELost EDep ColorBar");
  MyLabelScale("Icosah Lvl",   (gpointer)"isl",  boxa3,  0,   6, 1,    0);
  MyLabelScale("Ball Size",    (gpointer)"bsz",  boxa3,  0,   9, 0.1, d->BallSize/0.3);
  MyLabelScale("Size Factor",  (gpointer)"bfc",  boxa3,  0,   2, 0.1,  0.2);
  MyLabelScale("Background",   (gpointer)"bg",   boxa3,  0,   1, 0.1,  0);
  MyLabelScale("Stride",       (gpointer)"std",  boxa3,  0,   9, 0.1,  0);
  MyLabelScale("Smart Stride", (gpointer)"sstd", boxa3,  0,   9, 0.1,  0);
  MyLabelScale("Line Width",   (gpointer)"lwd",  boxa3,  0,   5, 0.1,  1);
  MyLabelScale("Palette Discr",(gpointer)"dpt",  boxa3,  1,  18, 1,   16);
  MyLabelScale("Canvas Max",   (gpointer)"q2",   boxa3,-0.5, 1.5, 0.1,  1);
  MyLabelScale("Canvas Med",   (gpointer)"q1",   boxa3,   0,   1, 0.1,  0.5);
  MyLabelScale("Canvas Min",   (gpointer)"q0",   boxa3,-0.5, 1.5, 0.1,  0);
  MyLabelScale("Font Size",    (gpointer)"fsz",  boxa3,   5,  40,   1,  d->FontSize);

  //MyCombo("Spatial Units:",   G_CALLBACK(CB_SpatialUnits),boxa3, "mm μm nm Angstrom Best");
  //MyCheckButton("Swap Z -Z",    G_CALLBACK(CB_SwapDataZZ), boxa1, false);
  //MyHSeparator(boxa1);

  // labeled Scales
  MyLabelScale("Background:",    (gpointer)"bg",  boxb1,   0,  1,  0.1, 0);
  MyLabelScale("Light PosX:",    (gpointer)"lpx", boxb1, -20, 20,  1,   1);
  MyLabelScale("Light PosY:",    (gpointer)"lpy", boxb1, -20, 20,  1,   1);
  MyLabelScale("Light PosZ:",    (gpointer)"lpz", boxb1, -20, 20,  1,   1);
  MyLabelScale("Light Int:",     (gpointer)"lpw", boxb1,   0,  5,  0.1, 0);
  MyLabelScale("green Attenu:",  (gpointer)"gat", boxb1,   0, 0.5, 0.1, d->greenAttenuation);
  MyLabelScale("LightDiffuse :", (gpointer)"ldf", boxb1,   0,  1,  0.1, 0.9);
  MyLabelScale("LightAmbient :", (gpointer)"lam", boxb1,   0,  1,  0.1, 0.4);
  MyLabelScale("LightSpecular:", (gpointer)"lsp", boxb1,   0,  1,  0.1, 0.9);
  MyLabelScale("LightShininess:",(gpointer)"lsh", boxb1,   0,  1,  0.1, 0.2);
  //  MyCheckButton("Lights On/Off", (gpointer)"tli", boxb2, true);

  //  MyHSeparator(boxa2);
  MyLabelScale("Scale  ",      (gpointer)"sr",  boxc, 0.1, 10, 0.1, 1);
  MyLabelScale("Scale X",      (gpointer)"sx",  boxc, 0.1, 10, 0.1, 1);
  MyLabelScale("Scale Y",      (gpointer)"sy",  boxc, 0.1, 10, 0.1, 1);
  MyLabelScale("Scale Z",      (gpointer)"sz",  boxc, 0.1, 10, 0.1, 1);
  MyLabelScale("ColorSc",      (gpointer)"csc", boxc, 0,    7, 1 ,  0);

  MyLabelScale("Clip x min:", (gpointer)"cx", boxd, 0, 10, 0.1,  0);
  MyLabelScale("Clip X MAX:", (gpointer)"cX", boxd, 0, 10, 0.1, 10);
  MyLabelScale("Clip y min:", (gpointer)"cy", boxd, 0, 10, 0.1,  0);
  MyLabelScale("Clip Y MAX:", (gpointer)"cY", boxd, 0, 10, 0.1, 10);
  MyLabelScale("Clip z min:", (gpointer)"cz", boxd, 0, 10, 0.1,  0);
  MyLabelScale("Clip Z MAX:", (gpointer)"cZ", boxd, 0, 10, 0.1, 10);
  MyLabelScale("Clip v min:", (gpointer)"cv", boxd, 0, 10, 0.1,  0);
  MyLabelScale("Clip V MAX:", (gpointer)"cV", boxd, 0, 10, 0.1, 10);

  MyClickButton("GL Info", G_CALLBACK(CB_GLInfoWindow),   boxe);
  MyClickButton("Fonts TXT",   G_CALLBACK(CB_FontInfoWindow), boxe);
  MyClickButton("Fonts GL",    G_CALLBACK(CB_FontPlot), boxe);
  MyLabelScale("Resize Win:",  (gpointer)"rsz", boxe, 400, 2500, 100, d->Wsize);
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

  gtk_widget_show_all(butwin);
}
