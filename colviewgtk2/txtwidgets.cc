
#include "visu.hh"

extern vector <string> tpi;

//extern GdkGLConfig * glconfig;

string tpn[10] = {
  "unknown",
  "gamma",
  "e-   ",
  "e+   "
};



void DisplayWindowText(string str, const char * fontname, const char * color) {
  static GtkWidget * infoWin;

  if(infoWin != NULL) {
    gtk_widget_destroy(infoWin);
  }

  infoWin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW (infoWin), "Info Win");
  //gtk_window_move(GTK_WINDOW(infoWin), gdk_screen_width()/2, 3);
  //gtk_window_set_default_size(GTK_WINDOW(infoWin), 400, 500);

  GtkWidget * textv = gtk_text_view_new();
  //gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textv), GTK_WRAP_WORD);
  PangoFontDescription * fd = pango_font_description_from_string(fontname);
  gtk_widget_modify_font(textv, fd);
  pango_font_description_free(fd);

  GtkWidget * box = gtk_vbox_new(false, 5);
  gtk_box_pack_start(GTK_BOX(box), textv, 1, 1, 0);
  gtk_container_add(GTK_CONTAINER(infoWin), box);

  GtkTextBuffer * buff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textv) );
 
  gtk_text_buffer_set_text(buff, str.c_str(), -1);

  gchar text[1024];
  strcpy(text, "\n .");
  gtk_text_buffer_insert_at_cursor(buff, text, -1);

  /* Change default color throughout the widget */
  GdkColor col;
  gdk_color_parse(color, &col);
  gtk_widget_modify_text(textv, GTK_STATE_NORMAL, &col);

  gtk_widget_show_all(infoWin);
}


void DisplayWindowText(string str) {
  DisplayWindowText(str, "Monospace", "darkgreen");
}


void DisplayFontList() {
  int NoF;
  PangoFontFamily ** fams;
  PangoFontMap * fontmap;

  fontmap = pango_cairo_font_map_get_default();
  pango_font_map_list_families( fontmap, &fams, &NoF);
  cout << "Number of Families: " << NoF << endl;

  ostringstream sfi;

  for(int i = 0; i < NoF; i++) {
    PangoFontFamily * family = fams[i];
    const char * FamilyName = pango_font_family_get_name(family);
    sfi << right << setw(4) << i+1 << " " << left << setw(25) << FamilyName;
    if((i+1)%4) sfi << " ";
    else        sfi << endl;
  }
  g_free(fams);

  DisplayWindowText(sfi.str(), "monospace 11", "black");
}



string BestUnitEnergy( double E) {
  ostringstream oss;
  if     ( E < 1e-1) oss << E*1e3  << " meV";
  else if( E < 1e3 ) oss << E      << "  eV";
  else if( E < 1e6 ) oss << E/1e3  << " keV";
  else               oss << E/1e6  << " MeV";

  //cout << "BestUnitEnergy " << E << " -> " << oss.str() << "|" << endl;
  return oss.str();
}



void DisplayInfo(DataObject *d) {

  ostringstream sfi;
  sfi << "\nFrame:\n"
      << "\tx (" << d->m.x << " , " << d->M.x << ")\tDx: " << d->M.x-d->m.x << " " << d->SpatialUnits << endl
      << "\ty (" << d->m.y << " , " << d->M.y << ")\tDy: " << d->M.y-d->m.y << " " << d->SpatialUnits << endl
      << "\tz (" << d->m.z << " , " << d->M.z << ")\tDz: " << d->M.z-d->m.z << " " << d->SpatialUnits << endl
      << "\tRmax |" << d->rMax << "|" << endl
    //<< "spunit: " << d->SpatialUnits << " spf: " << d->SpatialFactor << endl
      << "Volume: " << d->Volume << " " << d->SpatialUnits << "3" << endl
      << "\tshift xyz: " << (d->m.x+d->M.x)*0.5 << ","
      << (d->m.y+d->M.y)*0.5 << "," << (d->m.z+d->M.z)*0.5 << endl

      << "\tf[0] Particle  (" << d->m.f[0] << "," << d->M.f[0] << ")\n"
      << "\tf[1] Interact  (" << d->m.f[1] << "," << d->M.f[1] << ")\n"
      << "\tf[3] Energy    (" << d->m.f[3] << "," << d->M.f[3] << ")\n"
      << "\tf[4] Egy loss  (" << d->m.f[4] << "," << d->M.f[4] << ")\n"
      << "\tf[5] Egy Depos (" << d->m.f[5] << "," << d->M.f[5] << ")"
      << endl << endl;


  sfi << "N# interactions: " << d->NumDat << endl
      << "Energy Dep:      " << BestUnitEnergy(d->EdepTotal)
      << endl << endl;

  sfi << "Histogram:" << endl
      << "   Type                     N#     %                Edep     Edep/col \n";

  for(unsigned int i=0; i<tpi.size(); i++) {
    //if( d->Hist[i] || d->Hist[i-1] || d->Hist[i+1] )
    if( d->Hist[i] )
      sfi << right << setw(2) << i << " " 
	  << left  << setw(15) << tpi[i]
  	  << right << setw(12) << d->Hist[i] << "  "
  	  << left  << setw(12) << (float)d->Hist[i]/(float)d->NumDat
  	  << right << setw(12) << BestUnitEnergy(d->Hist2[i]);
    if(d->Hist[i] != 0)
      sfi << right << setw(13) << BestUnitEnergy(d->Hist2[i]/d->Hist[i])
  	  << endl;
  }

  float rho = 0.0;  // (g/cm3)
  for(unsigned long i=0; i< d->NumDat; i++) {
    rho += d->r[i].f[6];
  }
  rho /= d->NumDat;

  float mass = rho * d->Volume/pow(d->SpatialFactor,3)/1e12; // (Kg)
  float Dose = d->EdepTotal * 1.60217733e-19 / mass;

  sfi << endl
      << "Dens: " << rho << " g/cm3" << endl
      << "mass: " << mass << " Kg " << mass*1e3 << " g "
      << mass*1e6 << " ug " << mass*1e9 << " ng " << endl
      << "E. Dep: " << d->EdepTotal << " eV = " << d->EdepTotal*1.60217733e-19 << " Joule" << endl
      << "Dose: " << Dose << " Gy" << endl;

  DisplayWindowText(sfi.str());
}




void DisplayGLInfo() {
  DisplayWindowText( GLInfo() );
}



void DisplayNearData(double xM, double yM, DataObject *d) {
  unsigned long Im = 0;
  double Dm = sqrt( pow((d->r[0].x-xM),2)+pow((d->r[0].y-yM),2) );

  for(unsigned long i=0; i< d->NumDat; i++) {
    double Dist = sqrt( pow((d->r[i].x-xM),2)+pow((d->r[i].y-yM),2) );
    if(Dist < Dm) {
      Dm = Dist;
      Im = i;
    }
  }


  int itpn = (int)d->r[Im].f[0];
  if(itpn > 3) itpn = 0;

  ostringstream sfi;
  sfi << "Point: (" << d->r[Im].x << ", " << d->r[Im].y << ", " << d->r[Im].z << ")\n"
      << "Particle:    (" << d->r[Im].f[0] << ") " << tpn[itpn] << endl
      << "Interaction: (" << d->r[Im].f[1] << ") " << tpi[(int)d->r[Im].f[1]] << endl
      << "Material:     " << d->r[Im].f[6] << " g/cm3\n"
      << "Energy:       " << d->r[Im].f[3] << " eV\n"
      << "Lost:         " << d->r[Im].f[4] << " eV\n"
      << "Deposited     " << d->r[Im].f[5] << " eV\n";


  DisplayWindowText(sfi.str());
}
