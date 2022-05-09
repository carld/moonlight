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
  HRCReader.h

  Stephane Rehel

  June 18th, 1996

  ML version: October 8 1997
*/

#ifndef __HRCReader_h
#define __HRCReader_h

#include <stdio.h>

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __Vector_h
#include "tools/Vector.h"
#endif

#ifndef __Vector2_h
#include "tools/Vector2.h"
#endif

#ifndef __Tokenize_h
#include "Tokenize.h"
#endif

#define TokensList DList<Token,TOKENS>

class MLImporter;
class MLMesh;
class MLObject;
class MLAbstractMesh;
class MLScene;

/////////////////////////////////////////////////////////////////////////////

class HRCReader
{
private:
  TokensList* tokensList;
  Tokenize* tk;
  MLImporter* importer;
  FILE* file;
  MLScene* scene;
  boolean verbose;

public:
  HRCReader( MLImporter* _importer );
  ~HRCReader();

  boolean init();
  boolean load();

protected:
  Token* nextToken();
  MLObject* readHRCModel();
  boolean skipBrace();
  void skipToken();
  void skipTokens( int n );
  boolean needChar( Token::Char theChar );
  boolean findChar( Token::Char theChar );
  boolean needName( const MLString& theName );
  boolean getScalar( double& scalar );
  boolean getNamedScalar( const MLString& name, double& scalar );
  boolean getString( MLString& string );
  boolean getVector( Vector& vector );
  boolean getVector2( Vector2& vector );
  boolean getNamedVector( const MLString& name, Vector& vector );
  boolean getNamedVector2( const MLString& name, Vector2& vector );
  boolean getNamedPoint( const MLString& name, Point& point );
  MLAbstractMesh* readMesh( MLAbstractMesh* model );
  MLObject* readSubModel( MLObject* father );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef ifndef __HRCReader_h
