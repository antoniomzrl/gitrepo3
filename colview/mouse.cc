
#include "colv.hh"

extern DataObject * d;

float xBegin, yBegin, xEnd, yEnd; // selection
float xMouse, yMouse; // motion
int  mouseX, mouseY;
bool mouseLeft, mouseLeftCtrl, mouseLeftShft,
  mouseRight,   mouseRightCtrl,
  mouseMiddle,  mouseMiddleCtrl,
  mouseMoved;


void PositionFromPixel(float * px, float * py, int x, int y) {
  //*px = ( 2.0*x/gtk_widget_get_allocated_width(glw) -1.0) *d->FoV*d->AR-d->AR+1;
  //*py = (-2.0*y/gtk_widget_get_allocated_height(glw) +1.0) *d->FoV;

  *px =  (x/(d->PlotSize.x/2.0)  -1.0) * d->FoV*d->AR-d->AR+1;
  *py = -(y/(d->PlotSize.y/2.0) -1.0) * d->FoV;
}


gboolean butScroll(GtkWidget * glw, GdkEventScroll * event) {
  //g_message("scrolled!");
  cout << "Mouse wheel scroll: " << event->direction << endl;

  if(event->direction == GDK_SCROLL_DOWN)    ScaleScene(10);
  else if(event->direction == GDK_SCROLL_UP) ScaleScene(-10);
  InvalidateGlw();  
  return true;
}


gboolean butPress(GtkWidget * glw, GdkEventButton * event) {
  PositionFromPixel( &xMouse, &yMouse, (int)event->x, (int)event->y);

  cout << "butPress " << endl;
  xBegin = xMouse;
  yBegin = yMouse;

  if(event->state & GDK_SHIFT_MASK) {
    if(event->button == 1)
      mouseLeftShft = true;
    return true;
  }

  if(event->state & GDK_CONTROL_MASK) {
    if(     event->button == 1) mouseLeftCtrl = true;
    else if(event->button == 2) mouseMiddleCtrl = true;
    else if(event->button == 3) mouseRightCtrl = true;
    return true;
  }

  if(event->button == 1) {
    mouseLeft = true;
    return true;
  }
  if(event->button == 2) {
    mouseMiddle = true;
    return true;
  }
  if(event->button == 3) {
    mouseRight = true;
    return true;
  }

  return false;
}



gboolean butRelease(GtkWidget * glw, GdkEventButton * event) {
  cout << "butRelease " << endl;
  d->vEconomic = false;

  if(mouseMoved) {
    InvalidateGlw();
    mouseMoved = false;
  }
  

  PositionFromPixel( &xMouse, &yMouse, (int)event->x, (int)event->y);
  
  if(event->state & GDK_SHIFT_MASK) {
    if(event->button == 1) {
      mouseLeftShft = false;
      //DisplayNearData(xMouse, yMouse, d);
    }
    return true;
  }
  if(event->state & GDK_CONTROL_MASK) {
    if(event->button == 2) {
      //CommitRotationHalfPi();
      //Invalidate(glw);
      return true;
    }
    if(event->button == 3) {
      cout << "Ctrl 3 " << xMouse << " " << yMouse << endl;
      //ShiftData(d, xMouse, yMouse, 0.0);
      //ComputeFrame(d);
      //Invalidate(glw);
      return true;
    }
  }
 
  if(event->button == 1) {
    mouseLeft = false;
    if(mouseLeftCtrl) {
      mouseLeftCtrl = false;
      d->vSelection = false;
      ClipData(d);
      InvalidateGlw();
    }
    return true;
  }
  if(event->button == 2) {
    //CommitRotation();
    //Invalidate(glw);
    mouseMiddle = false;
    return true;
  }
  if(event->button == 3) {
    mouseRight  = false;
    if(mouseRightCtrl) {
      mouseRightCtrl = false;
      InvalidateGlw();
    }
    return true;
  }

  return false;
}



gboolean mouseMotion(GtkWidget * glw, GdkEventMotion * event) {
  //cout << "mouseMotion " << endl;
  
  GdkModifierType state;
  int x, y;

  // Avoid lags
  if(event->is_hint) {
    //gdk_window_get_pointer(event->window, &x, &y, &state);
    gdk_window_get_device_position(gtk_widget_get_window(glw),
				   gtk_get_current_event_device(),
				   &x, &y, &state);
  }
  else {
    x = (int)event->x;
    y = (int)event->y;
    state = (GdkModifierType) event->state;
    //cout << "mouseMotion NOT-HINT" << endl;
  }

  int dx = x-mouseX;
  int dy = y-mouseY;

  // Small displacement
  //  if( dx==0 && dy==0) {
  if( abs(dx)<15 && abs(dy)<15) {
    //cout << "small displacement" << endl;
    return true;
  }

  float px, py;
  PositionFromPixel( &px, &py, x, y);

  mouseMoved = false;

  if( mouseLeft) {  // Rotation
    RotateScene( dy, dx, 0);
    mouseMoved = true;
  }
  else if( mouseMiddle) {   // Zoom
    ScaleScene(dy);
    mouseMoved = true;
  }
  else if( mouseRight)	{ // Translation
    TranslateScene(dx, dy);
    xMouse = px;
    yMouse = py;
    mouseMoved = true;
  }
  else if( mouseLeftCtrl) { // Mark Selection rectangle
    xEnd = px;
    yEnd = py;
    d->vSelection = true;
    mouseMoved = true;
  }
  else if( mouseRightCtrl) { // Mark Selection rectangle
    xEnd = px;
    yEnd = py;
    d->LightPos.x = px*3;
    d->LightPos.y = py*3;
    InvalidateGlw();
    cout << "mouseRight\n";
  }
  else if( mouseMiddleCtrl) { 
    d->LightPos.z += dx+dy;
    InvalidateGlw();
    cout << "mouseMiddle\n";
  }

  mouseX = x;
  mouseY = y;

  if( mouseMoved) {
    d->vEconomic = true;
    InvalidateGlw();
  }

  d->MouseSelection = vec4(std::min(xBegin, xEnd), std::max(xBegin, xEnd),
			   std::min(yBegin, yEnd), std::max(yBegin, yEnd));
  
  return true;
}
