#include "visu.hh"

extern DataObject * d;
extern GtkWidget * glw;

// Uno de los 4:
//#define WITH_GLUT
//#define WITH_PANGO
//#define WITH_PANGOFT2
//#define WITH_PANGOCAIRO //no llega a funcionar

#ifdef WIN32  //con pango+ft2 no pirula:
#define WITH_PANGO
#else
#define WITH_PANGOFT2
//#define WITH_PANGOCAIRO
#endif

#if defined WITH_GLUT
void PlotText(float x, float y, float z, char * text, void * font) {
  //void * font = GLUT_BITMAP_HELVETICA_10;
  //void * font = GLUT_BITMAP_TIMES_ROMAN_10;
  //cout << "PlotText " << x << " " << y << " " << z << " " << text << endl;

  //if     (fh == 18) font = GLUT_BITMAP_HELVETICA_18;
  //else if(fh == 12) font = GLUT_BITMAP_HELVETICA_12;
  //else if(fh == 24) font = GLUT_BITMAP_HELVETICA_24;

  glRasterPos3f(x, y, z);
    
  for (char * c = text; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
    //glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
  }
}

#elif defined WITH_PANGO

int PlotText(float x, float y, float z, const char * text, const char * fname, int fsize) {
  //GLuint fl; //font list base
  //gint   fh; //font height
  //PangoFontMetrics     *fm;

  // Generate font display lists.
  GLuint fl = glGenLists( 256);
  PangoFontDescription * fd = pango_font_description_from_string(fname);
  pango_font_description_set_size(fd, fsize * PANGO_SCALE);
  PangoFont * font = gdk_gl_font_use_pango_font( fd, 0, 256, fl);
  if(font == NULL) {
    g_print( "*** Can't load font '%s'\n", fname);
    //return(0);
  }

  //fm = pango_font_get_metrics( font, NULL);
  //fh = pango_font_metrics_get_ascent(fm) + pango_font_metrics_get_descent(fm);
  //fh = PANGO_PIXELS( fh);
  //pango_font_metrics_unref(fm);
  //pango_font_description_free(fd);

  glRasterPos3f(x, y, z);
  glListBase(fl);
  glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);

  return(1);
}


#elif defined WITH_PANGOFT2

#include <pango/pangoft2.h>

void gl_pango_ft2_render_layout(PangoLayout *layout) {
  PangoRectangle logical_rect;
  pango_layout_get_extents(layout, NULL, &logical_rect);
  if (logical_rect.width == 0 || logical_rect.height == 0) return;
 
  FT_Bitmap bitmap;
  bitmap.rows = PANGO_PIXELS (logical_rect.height);
  bitmap.width = PANGO_PIXELS (logical_rect.width);
  bitmap.pitch = bitmap.width;
  bitmap.buffer = (unsigned char *) malloc (bitmap.rows * bitmap.width);
  bitmap.num_grays = 256;
  bitmap.pixel_mode = ft_pixel_mode_grays;

  //  cout << "pangopixels: " << bitmap.rows << " x " << bitmap.width
  //   << " " << bitmap.num_grays << endl;
  
  memset (bitmap.buffer, 0, bitmap.rows * bitmap.width);
  //pango_ft2_render_layout(&bitmap, layout, PANGO_PIXELS(-logical_rect.x), 0);
  pango_ft2_render_layout(&bitmap, layout, 0,0);

  GLvoid * pixels = malloc (bitmap.rows * bitmap.width * 4);
  guint32 * p = (guint32 *) pixels;

  GLfloat color[4];
  glGetFloatv(GL_CURRENT_COLOR, color);

#if !defined(GL_VERSION_1_2) && G_BYTE_ORDER == G_LITTLE_ENDIAN
  guint32 rgb =  ((guint32) (color[0] * 255.0)) | (((guint32) (color[1] * 255.0)) << 8) |
    (((guint32) (color[2] * 255.0)) << 16);
#else
  guint32 rgb = (((guint32) (color[0] * 255.0)) << 24) | (((guint32) (color[1] * 255.0)) << 16) |
    (((guint32) (color[2] * 255.0)) << 8);
#endif

  GLfloat a = color[3];
  guint8 * row = bitmap.buffer + bitmap.rows * bitmap.width; /* past-the-end */
  guint8 * row_end = bitmap.buffer;      /* beginning */

  if (a == 1.0) {
    do {
      row -= bitmap.width;
      for(unsigned int i = 0; i < bitmap.width; i++)
#if !defined(GL_VERSION_1_2) && G_BYTE_ORDER == G_LITTLE_ENDIAN
	*p++ = rgb | (((guint32) row[i]) << 24);
#else
      *p++ = rgb | ((guint32) row[i]);
#endif
    }
    while (row != row_end);
  }
  else {
    do {
      row -= bitmap.width;
      for(unsigned int i = 0; i < bitmap.width; i++)
#if !defined(GL_VERSION_1_2) && G_BYTE_ORDER == G_LITTLE_ENDIAN
	*p++ = rgb | (((guint32) (a * row[i])) << 24);
#else
      *p++ = rgb | ((guint32) (a * row[i]));
#endif
    }
    while (row != row_end);
  }

  glPixelStorei (GL_UNPACK_ALIGNMENT, 4);

  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDrawPixels (bitmap.width, bitmap.rows, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, pixels);
  glDisable (GL_BLEND);

  g_free (bitmap.buffer);
  g_free (pixels);
}


int PlotText(float x, float y, float z, const char * text, const char * fname, int fsize) {

  /* Font */
  //PangoContext * f_context = pango_ft2_get_context(96, 96); //deprecated
  PangoFontMap * fontmap = pango_ft2_font_map_new();
  pango_ft2_font_map_set_resolution(PANGO_FT2_FONT_MAP(fontmap), 96, 96);
  PangoContext * f_context = pango_font_map_create_context(PANGO_FONT_MAP(fontmap));
  PangoContext *widget_context = gtk_widget_get_pango_context(glw);
  PangoFontDescription *font_desc = pango_context_get_font_description(widget_context);
  pango_font_description_set_size(font_desc, fsize * PANGO_SCALE);
  pango_font_description_set_family(font_desc, fname);
  pango_context_set_font_description(f_context, font_desc);
  //pango_font_description_free(font_desc);

  /* Text layout */
  PangoLayout *layout = pango_layout_new(f_context);
  //pango_layout_set_width (layout, PANGO_SCALE * widget->allocation.width);
  //pango_layout_set_alignment (layout, PANGO_ALIGN_CENTER);
  pango_layout_set_text(layout, text, -1);

  //glCallList(128);
  glRasterPos3f(x, y, z);
  gl_pango_ft2_render_layout(layout);

  return(1);
}


#elif defined WITH_PANGOCAIRO


unsigned int create_texture (unsigned int width, unsigned int height,
			     unsigned char *pixels) {
  unsigned int texture_id;

  glGenTextures (1, &texture_id);
  glBindTexture (GL_TEXTURE_2D, texture_id);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, width, height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  return texture_id;
}

cairo_t* create_cairo_context (int width, int height, int channels,
			       cairo_surface_t** surf, unsigned char** buffer) {
  *buffer = (unsigned char*)calloc (channels * width * height, sizeof (unsigned char));
  *surf = cairo_image_surface_create_for_data (*buffer, CAIRO_FORMAT_ARGB32, width,
                                                 height, channels * width);
  return cairo_create (*surf);
}

void get_text_size (PangoLayout *layout, unsigned int *width, unsigned int *height) {
  pango_layout_get_size (layout, (int*)width, (int*)height);
  /* Divide by pango scale to get dimensions in pixels. */
  *width /= PANGO_SCALE;
  *height /= PANGO_SCALE;
}

cairo_t* create_layout_context () {
  cairo_surface_t *temp_surface;
  cairo_t *context;

  temp_surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 0, 0);
  context = cairo_create (temp_surface);
  cairo_surface_destroy (temp_surface);
  
  return context;
}

unsigned int render_text (const char *text, unsigned int *text_width,
			  unsigned int *text_height, unsigned int *texture_id) {
  cairo_t *layout_context;
  cairo_t *render_context;
  cairo_surface_t *temp_surface;
  cairo_surface_t *surface;
  unsigned char* surface_data = NULL;
  PangoFontDescription *desc;
  PangoLayout *layout;
    
  layout_context = create_layout_context ();

  /* Create a PangoLayout, set the font and text */
  layout = pango_cairo_create_layout (layout_context);
  pango_layout_set_text (layout, text, -1);

  /* Load the font */
#define FONT "fixed"
  desc = pango_font_description_from_string (FONT);
  pango_layout_set_font_description (layout, desc);
  pango_font_description_free (desc);

  /* Get text dimensions and create a context to render to */
  get_text_size (layout, text_width, text_height);
  render_context = create_cairo_context (*text_width, *text_height,
					 4, &surface, &surface_data);

  /* Render */
  cairo_set_source_rgba (render_context, 1, 1, 1, 1);
  pango_cairo_show_layout (render_context, layout);

  *texture_id = create_texture(*text_width, *text_height, surface_data);

  /* Clean up */
  free (surface_data);
  g_object_unref (layout);
  cairo_destroy (layout_context);
  cairo_destroy (render_context);
  cairo_surface_destroy (surface);
}

void draw_texture (int width, int height, unsigned int texture_id) {
  /* Render a texture in immediate mode. */
  //glMatrixMode (GL_MODELVIEW);
  //glLoadIdentity ();
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glPushMatrix ();
  glBindTexture (GL_TEXTURE_2D, texture_id);
  glColor3f (1.f, 1.0f, 1.0f);

  float width2 =0.2, height2=0.2;
  
  glBegin (GL_QUADS);
  glTexCoord2f (0.0f, 0.0f);
  glVertex2f (0.0f, 0.0f);
  glTexCoord2f (1.0f, 0.0f);
  glVertex2f (width2, 0.0f);
  glTexCoord2f (1.0f, 1.0f);
  glVertex2f (width2 , height2);
  glTexCoord2f (0.0f, 1.0f);
  glVertex2f (0.0f, height2);
  glEnd ();

  glPopMatrix ();
  //SDL_GL_SwapBuffers();
}


int PlotText(float x, float y, float z, const char * text, const char * fname, int fsize) {

  unsigned int texture_id;
  unsigned int text_width = 0;
  unsigned int text_height = 0;

  render_text("Hola", &texture_id, &text_width, &text_height);
  draw_texture((int)text_width, (int)text_height, (int)texture_id);

  return(1);
}
#endif



void PlotText(float x, float y, float z, char * text) {
  //string defaultFont( "fixed");
  //string defaultFont( "courier");
  //string defaultFont( "helvetica");
  string defaultFont( "Monospace");
  
  PlotText(x, y, z, text, defaultFont.c_str(), d->FontSize);
}



void PlotFontList() {
  int NoF;
  PangoFontFamily ** fams;
  PangoFontMap * fontmap = pango_cairo_font_map_get_default();
  pango_font_map_list_families( fontmap, &fams, &NoF);
  cout << "Number of Families: " << NoF << endl;

  float y = 0.8;
  float delta = 0.1;

  for(int i = 0; i < NoF; i++) {
    PangoFontFamily * family = fams[i];
    const char * FamilyName = pango_font_family_get_name(family);
    char txt[128];
    sprintf(txt, "%d %s ----->   213.456 mm", i, FamilyName);

    //if( strstr(FamilyName, "ono") != NULL)
      if( PlotText(-0.5, y, 0.0, txt, FamilyName, 18) == 1)
	y -= delta;
  }
  g_free(fams);
}



void PlotLegend(DataObject * d) {
  char label[9][128];

  sprintf(label[0], "Plot - %s", d->Label[d->vType].c_str() );
 
  if(d->Background < 0.5) glColor3f(1.0, 1.0, 1.0);
  else                    glColor3f(0.0, 0.0, 0.0);

  glPushMatrix();
  glLoadIdentity();

  float rho = 0.0;  // (g/cm3)
  for(unsigned long i=0; i< d->NumDat; i++) {
    rho += d->r[i].f[6];
  }
  rho /= d->NumDat;

  float mass = rho * d->Volume/pow(d->SpatialFactor,3)/1e12; // (Kg)
  float Dose = d->EdepTotal * 1.60217733e-19 / mass;
    
  sprintf(label[1], "Density: %g g/cm3", rho);
  sprintf(label[2], "Mass:    %g g",     mass);
  sprintf(label[3], "Edep:    %g eV",    d->EdepTotal);
  sprintf(label[4], "         %g Joule", d->EdepTotal*1.60217733e-19);
  sprintf(label[5], "Dose:    %g Gy",    Dose);
  sprintf(label[6], "width:   %g %s", d->xn*2, d->SpatialUnits.c_str() );
  sprintf(label[7], "height:  %g %s", d->yn*2, d->SpatialUnits.c_str() );
  sprintf(label[8], "depth:   %g %s", d->zn*2, d->SpatialUnits.c_str() );

  for(int i=0; i<9; i++) {
    float y = -0.2 - i*0.06;
    //PlotText(-0.95, y, 0.0, label[i], "Mono", d->FontSize);
    PlotText(-0.95, y, 0.0, label[i]);
  }
  
  glPopMatrix();
}



void PlotFrameLabels(DataObject * d) {
  float Dx = (d->M.x - d->m.x)   + (d->M.y - d->m.y)/5;
  float Dy = (d->M.x - d->m.x)/5 + (d->M.y - d->m.y);
  Dx/=10;
  Dy/=10;
  //if(     Dx > Dy*10) Dy*=10;
  //else if(Dy > Dx*10) Dx*=10;
  
  char mx[24], my[24], Mx[24], My[24];

  cout << "PlotFrameLabels: " << d->m.x << " " << d->M.x << "\t"
       << d->m.y << " " << d->M.y << "\t" << Dx << " " << Dy << endl;

  sprintf(mx, "%.3g %s", d->m.x, d->SpatialUnits.c_str() );
  sprintf(Mx, "%.3g %s", d->M.x, d->SpatialUnits.c_str() );
  sprintf(my, "%.3g %s", d->m.y, d->SpatialUnits.c_str() );
  sprintf(My, "%.3g %s", d->M.y, d->SpatialUnits.c_str() );

  PlotText(d->m.x,      d->m.y-Dy, -d->m.z, mx);
  PlotText(d->M.x-Dx/3, d->m.y-Dy, -d->m.z, Mx);
  PlotText(d->m.x-Dx,   d->m.y,    -d->m.z, my);
  PlotText(d->m.x-Dx,   d->M.y,    -d->m.z, My);
}
