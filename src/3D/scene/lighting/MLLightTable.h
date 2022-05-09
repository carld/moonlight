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
  MLLightTable.h

  Stephane Rehel
  March 7 1997
*/

#ifndef __MLLightTable_h
#define __MLLightTable_h

class MLScene;
class MLLight;

/////////////////////////////////////////////////////////////////////////////

class MLLightTable
{
  friend class MLLighting; // for mapping MLLighting::dynamicLighting as
                           // max_lights
protected:
  MLScene* scene;

  MLLight** lights;
  int max_lights;
  int nLights;

public:
  MLLightTable( MLScene* _scene );

  virtual ~MLLightTable();

  int getNLights() const
    {
    return nLights;
    }

  MLLight* getLight( int light_index ) const
    {
    if( light_index < 1 || light_index > nLights )
      return 0;
    return lights[light_index];
    }

  MLLight* getLightNoCheck( int light_index ) const
    {
    return lights[light_index];
    }

  void addLight( MLLight* light );

  void removeLight( MLLight* light );

  void clear();

  void compileLights();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLLightTable_h
