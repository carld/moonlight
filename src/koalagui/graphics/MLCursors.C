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
  MLCursors.C

  Stephane Rehel
  July 23 1997
*/

#include "MLCursors.h"
#include "MLCursor.h"

#include "GraphicsSystem.h"

/////////////////////////////////////////////////////////////////////////////

// static
MLCursor* MLCursors::normal= 0;
MLCursor* MLCursors::highlighted_normal= 0;
MLCursor* MLCursors::right_ptr= 0;
MLCursor* MLCursors::hand= 0;
MLCursor* MLCursors::cross= 0;
MLCursor* MLCursors::pen= 0;
MLCursor* MLCursors::arrows= 0;
MLCursor* MLCursors::hourglass= 0;
MLCursor* MLCursors::hourglass2= 0;

/////////////////////////////////////////////////////////////////////////////

// static
boolean MLCursors::init()
{
  GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();

  if( graphicsSystem == 0 )
    return false;

  int width= 32;
  int height= 32;

#define begin(name)   char* _##name[] = {
#define end(name,x,y) }; name= graphicsSystem->newMLCursor();        \
                      name->create( width, height,                   \
                                    (const char**) _##name,          \
                                    x, y );                          \
                      name->build()
#define endc(name,x,y,back,fore) }; name= graphicsSystem->newMLCursor(); \
                      name->create( width, height,                       \
                                    (const char**) _##name,              \
                                    x, y, back, fore );                  \
                      name->build()

  begin(normal)
    "--                              ",
    "-+-                             ",
    "-++-                            ",
    "-+++-                           ",
    "-++++-                          ",
    "-+++++-                         ",
    "-++++++-                        ",
    "-+++++++-                       ",
    "-++++++++-                      ",
    "-+++++----                      ",
    "-++-++-                         ",
    "-+- -++-                        ",
    "--  -++-                        ",
    "     -++-                       ",
    "     -++-                       ",
    "      --                        ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                "
  end(normal,1,1);

#ifdef __WINDOWS__
  // windows version doesn't use colored cursors
  begin(highlighted_normal)
    "++                              ",
    "+-+                             ",
    "+--+                            ",
    "+---+                           ",
    "+----+                          ",
    "+-----+                         ",
    "+------+                        ",
    "+-------+                       ",
    "+--------+                      ",
    "+-----++++                      ",
    "+--+--+                         ",
    "+-+ +--+                        ",
    "++  +--+                        ",
    "     +--+                       ",
    "     +--+                       ",
    "      ++                        ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                "
  end(highlighted_normal,1,1);
#else
  begin(highlighted_normal)
    "--                              ",
    "-+-                             ",
    "-++-                            ",
    "-+++-                           ",
    "-++++-                          ",
    "-+++++-                         ",
    "-++++++-                        ",
    "-+++++++-                       ",
    "-++++++++-                      ",
    "-+++++----                      ",
    "-++-++-                         ",
    "-+- -++-                        ",
    "--  -++-                        ",
    "     -++-                       ",
    "     -++-                       ",
    "      --                        ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                "
  endc( highlighted_normal,1,1,Color8(255,255,255),Color8(0,0,255));
#endif

  begin(right_ptr)
    "        --                      ",
    "       -+-                      ",
    "      -++-                      ",
    "     -+++-                      ",
    "    -++++-                      ",
    "   -+++++-                      ",
    "  -++++++-                      ",
    " -+++++++-                      ",
    "-++++++++-                      ",
    "----+++++-                      ",
    "   -++-++-                      ",
    "  -++- -+-                      ",
    "  -++-  --                      ",
    " -++-                           ",
    " -++-                           ",
    "  --                            ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                "
  end(right_ptr,8,1);

  begin(hand)
    " --------                       ",
    "-++++++++-                      ",
    "+--------+-                     ",
    "-++++++---+-                    ",
    " --+-------+-                   ",
    "   -+++----+-                   ",
    "  -+-------+-                   ",
    "   -+++---+-+-                  ",
    "  -+-----+---+-                 ",
    "   -++--+-----+-                ",
    "    --++-----+-                 ",
    "     -+--+--+-                  ",
    "      -+---+-                   ",
    "       -+-+-                    ",
    "        -+-                     ",
    "         -                      ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                "
  end(hand,0,2);

  begin(cross)
    "       ---                      ",
    "       -+-                      ",
    "       -+-                      ",
    "       -+-                      ",
    "       -+-                      ",
    "       -+-                      ",
    "       -+-                      ",
    "--------+--------               ",
    "-+++++++++++++++-               ",
    "--------+--------               ",
    "       -+-                      ",
    "       -+-                      ",
    "       -+-                      ",
    "       -+-                      ",
    "       -+-                      ",
    "       -+-                      ",
    "       ---                      ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                "
  end(cross,8,8);

  begin(pen)
    "--+++-                          ",
    "-+   +-                         ",
    "-+   ++-                        ",
    " -+  + +                        ",
    "  ++++ +-                       ",
    "  -+    +                       ",
    "   -+   +-                      ",
    "    +    +                      ",
    "    -+   +-                     ",
    "     +    +                     ",
    "     -+   +-                    ",
    "      -+   +-                   ",
    "       -++++-                   ",
    "        -+++-                   ",
    "         -++-                   ",
    "          -+-                   ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                "
  end(pen,11,15);

  begin(arrows)
    "      ----                      ",
    "      -++-                      ",
    "     -++++-                     ",
    "    -++++++-                    ",
    "   - --++-- -                   ",
    "  -+- -++- -+-                  ",
    "--++---++---++--                ",
    "-++++++++++++++-                ",
    "-++++++++++++++-                ",
    "--++---++---++--                ",
    "  -+- -++- -+-                  ",
    "   - --++-- -                   ",
    "    -++++++-                    ",
    "     -++++-                     ",
    "      -++-                      ",
    "      ----                      ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                "
  end(arrows,7,7);

  begin(hourglass)
    "     ++++++                     ",
    "   ++++++++++                   ",
    "  ++++++++++++                  ",
    " --++++++++++--                 ",
    " ---++++++++---                 ",
    "-----++++++-----                ",
    "------++++------                ",
    "-------+--------                ",
    "--------+-------                ",
    "------++++------                ",
    "-----++++++-----                ",
    " ---++++++++---                 ",
    " --++++++++++--                 ",
    "  ++++++++++++                  ",
    "   ++++++++++                   ",
    "     ++++++                     ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                "
  end(hourglass,7,7);

  begin(hourglass2)
    "     ------                     ",
    "   ----------                   ",
    "  +----------+                  ",
    " +++--------+++                 ",
    " ++++------++++                 ",
    "++++++----++++++                ",
    "+++++++--+++++++                ",
    "+++++++-++++++++                ",
    "++++++++-+++++++                ",
    "+++++++--+++++++                ",
    "++++++----++++++                ",
    " ++++------++++                 ",
    " +++--------+++                 ",
    "  +----------+                  ",
    "   ----------                   ",
    "     ------                     ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                "
  end(hourglass2,7,7);

/*
    "     ------                     ",
    "   --++++++--                   ",
    "  -++++++++++-                  ",
    " --++++++++++--                 ",
    " ---++++++++---                 ",
    "-----++++++-----                ",
    "------++++------                ",
    "-------+--------                ",
    "--------+-------                ",
    "------++++------                ",
    "-----++++++-----                ",
    " ---++++++++---                 ",
    " --++++++++++--                 ",
    "  -++++++++++-                  ",
    "   --++++++--                   ",
    "     ------                     ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                "
*/
/*
    "     ------                     ",
    "   --++++++--                   ",
    "  -++++++++++-                  ",
    " --++++++++++--                 ",
    " - -++++++++- -                 ",
    "-   -++++++-   -                ",
    "-    -++++-    -                ",
    "-     -+--     -                ",
    "-     --+-     -                ",
    "-    -++++-    -                ",
    "-   -++++++-   -                ",
    " - -++++++++- -                 ",
    " --++++++++++--                 ",
    "  -++++++++++-                  ",
    "   --++++++--                   ",
    "     ------                     ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                "
*/

  return true;

#undef end
#undef begin
}

/////////////////////////////////////////////////////////////////////////////

