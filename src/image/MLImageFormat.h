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
  MLImageFormat.h

  Stephane Rehel

  September 20 1997
*/

#ifndef __MLImageFormat_h
#define __MLImageFormat_h

#include <vector>
#include <string>

class MLImage;
class MLImageReader;
class MLImageWriter;

/////////////////////////////////////////////////////////////////////////////

class MLImageFactory {
public:
  /**
   * This function returns a \ref MLImageReader for the given format or NULL,
   * if no reader is available for this file type.
   */
  virtual MLImageReader *getReader() = 0;
  /**
   * This function returns a \ref MLImageWriter for the given format or NULL,
   * if no writer is available for this file type.
   */
  virtual MLImageWriter *getWriter() = 0;

  /**
   * This vector contains all file extensions that are used for this type of
   * file. The first entry in this array is used as default extension for this
   * file type.
   */
  std::vector<std::string> fileExtensions;

  /**
   * This string must contain a clear text name of the format.
   */
  std::string name;
};

class MLImageFormat
{
public:
  MLImageFormat();
  ~MLImageFormat();

  static MLImageFormat *instance;

  static MLImageFormat *getInstance() { return instance; };

  // XXX: for compatibilty only - Gregor, 2002-08-18
  typedef int FORMAT;

  static const int NULL_FMT=-1;
  static const int JPG=0;
  static const int TIF=1;
  static const int GIF=2;
  static const int RGB=3;
  static const int TGA=4;
  static const int PNM=5;
  static const int BMP=6;
  static const int LAST_FMT=7;

  void registerFactory(MLImageFactory *factory);

  int getNFormats();

  // 1 <= i <= nFormats
  int getFromIndex( int i );

  std::string getName( int fmt );

  int getFromName( const std::string& name );

  std::string getExtension( int fmt );

  int getFromFilename( const std::string& filename );

  bool availableReader( int fmt );
  bool availableWriter( int fmt );

  MLImageReader* getReader( int fmt );

  MLImageReader* guessReader( const std::string& filename );

  MLImageWriter* getWriter( int fmt );

protected:
  std::vector<MLImageFactory *> factories;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLImageFormat_h
