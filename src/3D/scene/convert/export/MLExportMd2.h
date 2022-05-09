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

// MLExportMd2 based on MLExportVRML1.h by Stephane Rehel

/*
        MLExportMd2.h

        phooky <phooky@gyral.com>

  based on MLExportVRML1.h by Stephane Rehel

  March 11 1998
*/

#ifndef __MLExportMd2_h
#define __MLExportMd2_h

#ifndef __MLExport_h
#include "MLExport.h"
#endif

class MLObject;
class MLMesh;

/////////////////////////////////////////////////////////////////////////////

class MLExportMd2: public MLExport
{
public:
  MLExportMd2( MLScene* _scene );

  virtual ~MLExportMd2();

  virtual boolean run();

};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLExportMd2_h
