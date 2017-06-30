
#include "colv.hh"

//xxxx GdkGLConfig * glconfig;

int GetAttrib(int attrib) {
  //xxxxxxxxxx
  // glconfig = gdk_gl_config_new_by_mode( (GdkGLConfigMode)
  // 					(GDK_GL_MODE_RGB |
  // 					 GDK_GL_MODE_DEPTH |
  // 					 GDK_GL_MODE_DOUBLE));
  // if( !glconfig) {
  //   cout << "Error glconfig" << endl;
  //   //g_assert_not_reached();
  // }

  int value=0;
  // gdk_gl_config_get_attrib( glconfig, attrib, &value);
  return value;
}



string GLInfo() {
  ostringstream sfi;

 
  sfi << "GL Info:  "                             << endl
      << "VENDOR:   " << glGetString(GL_VENDOR)   << endl
      << "VERSION:  " << glGetString(GL_VERSION)  << endl
      << "RENDERER: " << glGetString(GL_RENDERER) << endl
      << "GLSL VERSION: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl
      << "Epoxy:        " << epoxy_gl_version()   << endl
      << endl;
  
  // gint major, minor;
  // gdk_gl_query_version(&major, &minor);

  // sfi << "GL Version: " << major << "." << minor << endl
  //     << "gdkgl_RGBA          " << gdk_gl_config_is_rgba(glconfig)            << endl
  //     << "gdkgl_DOUBLE_BUFF   " << gdk_gl_config_is_double_buffered(glconfig) << endl
  //     << "gdkgl_STEREO        " << gdk_gl_config_is_stereo(glconfig)          << endl
  //     << "gdkgl_HAS_ALPHA     " << gdk_gl_config_has_alpha(glconfig)          << endl
  //     << "gdkgl_DEPTH_BUFFER  " << gdk_gl_config_has_depth_buffer(glconfig)   << endl
  //     << "gdkgl_STENCIL_BUFF  " << gdk_gl_config_has_stencil_buffer(glconfig) << endl
  //     << "gdkgl_ACCUM_BUFFER  " << gdk_gl_config_has_accum_buffer(glconfig)   << endl
  //     << "GDK_GL_USE_GL       " << GetAttrib(GDK_GL_USE_GL)       << endl
  //     << "GDK_GL_BUFFER_SIZE  " << GetAttrib(GDK_GL_BUFFER_SIZE)  << endl
  //     << "GDK_GL_LEVEL        " << GetAttrib(GDK_GL_LEVEL)        << endl            
  //     << "GDK_GL_RGBA         " << GetAttrib(GDK_GL_RGBA)         << endl             
  //     << "GDK_GL_DOUBLEBUFFER " << GetAttrib(GDK_GL_DOUBLEBUFFER) << endl     
  //     << "GDK_GL_STEREO       " << GetAttrib(GDK_GL_STEREO)       << endl           
  //     << "GDK_GL_AUX_BUFFERS  " << GetAttrib(GDK_GL_AUX_BUFFERS)  << endl      
  //     << "GDK_GL_RED_SIZE     " << GetAttrib(GDK_GL_RED_SIZE)     << endl         
  //     << "GDK_GL_GREEN_SIZE   " << GetAttrib(GDK_GL_GREEN_SIZE)   << endl       
  //     << "GDK_GL_BLUE_SIZE    " << GetAttrib(GDK_GL_BLUE_SIZE)    << endl        
  //     << "GDK_GL_ALPHA_SIZE   " << GetAttrib(GDK_GL_ALPHA_SIZE)   << endl       
  //     << "GDK_GL_DEPTH_SIZE   " << GetAttrib(GDK_GL_DEPTH_SIZE)   << endl       
  //     << "GDK_GL_STENCIL_SIZE " << GetAttrib(GDK_GL_STENCIL_SIZE) << endl     
  //     << "GDK_GL_ACCUM_RED_SIZE   " << GetAttrib(GDK_GL_ACCUM_RED_SIZE)   << endl   
  //     << "GDK_GL_ACCUM_GREEN_SIZE " << GetAttrib(GDK_GL_ACCUM_GREEN_SIZE) << endl 
  //     << "GDK_GL_ACCUM_BLUE_SIZE  " << GetAttrib(GDK_GL_ACCUM_BLUE_SIZE)  << endl  
  //     << "GDK_GL_ACCUM_ALPHA_SIZE " << GetAttrib(GDK_GL_ACCUM_ALPHA_SIZE) << endl 
  //   ;

  return( sfi.str() );
}
