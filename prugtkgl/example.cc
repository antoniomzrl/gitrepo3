// g++ $(pkg-config --cflags gio-2.0 gtk+-3.0 epoxy) example.cc $(pkg-config --libs gio-2.0 gtk+-3.0 epoxy)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <glib.h>
#include <gdk/gdkx.h>
#include <epoxy/glx.h>
#include <epoxy/gl.h>
#include <gtk/gtk.h>

#include <iostream>
using namespace std;

const GLchar *vsrc ="\n" \
"#version 330  core                            \n" \
"layout(location = 0) in vec2 in_position;     \n" \
"void main() {                                 \n" \
"  gl_Position = vec4(in_position, 0.0, 1.0);  \n" \
"}                                             \n";

const GLchar *vsrc2 ="\n"			   \
"#version 330  core                            \n" \
"layout(location = 0) in vec2 in_position;     \n" \
"void main() {                                 \n" \
"  gl_Position = vec4(in_position,0.0,1.0)*vec4(0.5,0.5,1.0,1.0);\n" \
"}                                             \n";

const GLchar *fsrc ="\n" \
"void main (void) {                            \n" \
"  gl_FragColor = vec4(0, 1, 0, 1);            \n" \
"}                                             \n";

const GLchar *fsrc2 ="\n"			   \
"void main (void) {                            \n" \
"  gl_FragColor = vec4(1, 1, 0, 1);            \n" \
"}                                             \n";


GLuint vao,  buffer,  program,  posID;
GLuint vao2, buffer2, program2, posID2;
GLfloat verts[12], verts2[12];


GLuint LoadShaders(const char * frag_src, const char * vert_src) {
  GLuint frag_shader, vert_shader;
  frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
  vert_shader = glCreateShader(GL_VERTEX_SHADER);
  
  glShaderSource(frag_shader, 1, &frag_src, NULL);
  glShaderSource(vert_shader, 1, &vert_src, NULL);
  
  glCompileShader(frag_shader);
  glCompileShader(vert_shader);

  GLuint prog = glCreateProgram();
  glAttachShader(prog, frag_shader);
  glAttachShader(prog, vert_shader);
  glLinkProgram(prog);
  return prog;
}


void InitObjects() {
  verts[0] =  0.0;       verts2[0] = 0  ; 
  verts[1] =  0.9;       verts2[1] = 0  ; 
  verts[2] = -0.9;       verts2[2] = 1  ; 
  verts[3] = -0.9;       verts2[3] = 0  ; 
  verts[4] = -0.9;       verts2[4] = 1  ; 
  verts[5] = -0.9;       verts2[5] = 0  ; 
  verts[6] = +0.9;       verts2[6] = 0.5; 
  verts[7] = -0.9;       verts2[7] = 1  ; 
  verts[8] = +0.9;       verts2[8] = 0.5; 
  verts[9] = -0.9;       verts2[9] = 1  ; 
  verts[10] = +0.0;      verts2[10] = 0  ;
  verts[11] = +0.9;      verts2[11] = 0  ;

  program  = LoadShaders(fsrc,  vsrc);
  program2 = LoadShaders(fsrc2, vsrc2);

  posID  = glGetAttribLocation(program,  "in_position");
  posID2 = glGetAttribLocation(program2, "in_position");

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &buffer);
  glGenVertexArrays(1, &vao2);
  glGenBuffers(1, &buffer2);
}


static gboolean realise(GtkGLArea *area, GdkGLContext *context) {
  cout << "realise" << endl;

  gtk_gl_area_make_current(GTK_GL_AREA(area));
  if (gtk_gl_area_get_error (GTK_GL_AREA(area)) != NULL) {
    printf("Failed to initialiize buffers\n");
    return FALSE;
  }

  InitObjects();

  
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

  glEnableVertexAttribArray(posID);
  glVertexAttribPointer(posID, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
  //glBindVertexArray(0);
  //glDeleteBuffers(1, &buffer);
  

  glBindVertexArray(vao2);
  glBindBuffer(GL_ARRAY_BUFFER, buffer2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts2), verts2, GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(posID2);
  glVertexAttribPointer(posID2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
  //glBindVertexArray(0);
  //glDeleteBuffers(1, &buffer2);
  
  return TRUE;
}


static gboolean render(GtkGLArea *area, GdkGLContext *context) {
  
  if( gdk_gl_context_is_legacy(context) ) cout << "Render Legacy true" << endl;
  else                                    cout << "Render Legacy false" << endl;
  cout << "VAOs " << vao << " " << vao2 << endl;
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0, 0.0, 0.0, 1.0);


  glUseProgram(program);
  glBindVertexArray(vao);
  glDrawArrays(GL_LINES, 0, 6);


  for( int i=0; i<sizeof(verts); i++) {
    verts2[i]-=0.1;
    //cout << verts2[i] << endl;
  }

  
  glUseProgram(program2);
  glBindVertexArray(vao2);
  glBindVertexArray(vao2);
  glBindBuffer(GL_ARRAY_BUFFER, buffer2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts2), verts2, GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(posID2);
  glVertexAttribPointer(posID2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
  
  
  glDrawArrays(GL_LINES, 0, 6);

  glBindVertexArray (0);
  glUseProgram (0);

  //glFlush();

  return TRUE;
}


int main(int argc, char** argv) {
  gtk_init(&argc, &argv);

  GtkWidget *window  = gtk_window_new(GTK_WINDOW_TOPLEVEL),
            *glarea = gtk_gl_area_new();

  gtk_window_set_default_size(GTK_WINDOW(window), 700, 700);

  g_signal_connect(window,  "delete-event", G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect(glarea, "realize",      G_CALLBACK(realise),       NULL);
  g_signal_connect(glarea, "render",       G_CALLBACK(render),        NULL);

  gtk_container_add(GTK_CONTAINER(window), glarea);

  gtk_widget_show_all(window);

  cout << "GL Info:  "                             << endl
       << "VENDOR:   " << glGetString(GL_VENDOR)   << endl
       << "VERSION:  " << glGetString(GL_VERSION)  << endl
       << "RENDERER: " << glGetString(GL_RENDERER) << endl
       << "GLSL VERSION: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl
       << "Epoxy:  " << epoxy_gl_version() << endl
       << endl;

  gtk_main();

  return 0;
}
