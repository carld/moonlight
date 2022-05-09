// This file is part of Moonlight Creator
//   Copyright (C) 1996-1998  Stephane Rehel
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

/*
  ModelRenderWindow.h

  Stephane Rehel

  September 21 1997
*/

#ifndef __ModelRenderWindow_h
#define __ModelRenderWindow_h

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __MLEventGrabber_h
#include "graphics/MLEventGrabber.h"
#endif

class ModelRender;
class SystemWindow;
class MLPicture;
class Interface;
class MLFont;

/////////////////////////////////////////////////////////////////////////////

class ModelRenderWindow: public MLEventGrabber
{
protected:
  Interface* interface;

  ModelRender* modelRender;

  SystemWindow* window;

  int x, y; // pos for rendered image
  int image_shrink; // > 1 for large image

  int traced_to_line;

  MLFont* font;
  MLString message;
  int message_length;

  boolean need_to_redraw;
  boolean user_break;
  boolean user_action;

  MLEventGrabber* prev_grabber;

public:
  ModelRenderWindow( ModelRender* _modelRender );

  virtual ~ModelRenderWindow();

  boolean open();
  void close();

  int getWidth() const;
  int getHeight() const;

  void setPicture( MLPicture* _picture );

private:
  virtual boolean eventGrabber( MLWindow*, MLEvent& );

public:
  // return false is user break
  boolean process_events();

  void wait_for_user_action();

protected:
  boolean current();

public:
  void eraseWindow();
  void redraw();

public:
  // origin is at the top of the raytraced window
  void reset_trace();
  // j begining at height-1 to 0
  void set_traced_from_origin_to( int j );

  void setMessage( const MLString& msg );

protected:
  void draw_lines( int j1, int j2 );
  void draw_message();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __ModelRenderwindow_h
