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
  MLPixmaps.C

  Stephane Rehel
  July 9 1996
*/

#include "MLPixmap.h"
#include "MLPixmaps.h"

/////////////////////////////////////////////////////////////////////////////

// static
MLPixmap* MLPixmaps::check= 0;
MLPixmap* MLPixmaps::checkedBox= 0;
MLPixmap* MLPixmaps::uncheckedBox= 0;
MLPixmap* MLPixmaps::emptyCircle= 0;
MLPixmap* MLPixmaps::filledCircle= 0;
MLPixmap* MLPixmaps::upArrow= 0;
MLPixmap* MLPixmaps::downArrow= 0;
MLPixmap* MLPixmaps::leftArrow= 0;
MLPixmap* MLPixmaps::rightArrow= 0;
MLPixmap* MLPixmaps::upArrowGray= 0;
MLPixmap* MLPixmaps::downArrowGray= 0;
MLPixmap* MLPixmaps::leftArrowGray= 0;
MLPixmap* MLPixmaps::rightArrowGray= 0;
MLPixmap* MLPixmaps::cross= 0;
MLPixmap* MLPixmaps::emptyCross= 0;
MLPixmap* MLPixmaps::window= 0;
MLPixmap* MLPixmaps::sphere= 0;
MLPixmap* MLPixmaps::smallWhiteArrow= 0;
MLPixmap* MLPixmaps::smallGrayArrow= 0;
MLPixmap* MLPixmaps::enterArrow= 0;
MLPixmap* MLPixmaps::littleButton= 0;
MLPixmap* MLPixmaps::littlePressedButton= 0;

/////////////////////////////////////////////////////////////////////////////

// static
boolean MLPixmaps::init()
{
#define begin(name) char* _##name[]= {
#define end(name)   }; name= new MLPixmap; \
                    name->make( (const unsigned char**)_##name, \
                                sizeof(_##name)/sizeof(char*) )
/*
begin(check)
 "        3",
 "       30",
 "      301",
 "30   301 ",
 "30  301  ",
 "301301   ",
 "31301    ",
 "3301     ",
 "301      ",
 "01       "
end(check);
*/

begin(check)
 "        2",
 "       20",
 "      201",
 "20   201 ",
 "20  201  ",
 "201201   ",
 "21201    ",
 "2201     ",
 "201      ",
 "01       "
end(check);

begin(uncheckedBox)
 "111111111111111",
 "111111111111113",
 "113333333333323",
 "113333333333323",
 "113333333333323",
 "113333333333323",
 "113333333333323",
 "113333333333323",
 "113333333333323",
 "113333333333323",
 "113333333333323",
 "113333333333323",
 "113333333333323",
 "122222222222223",
 "333333333333333"
end(uncheckedBox);

begin(checkedBox)
 "111111111111111",
 "111111111111113",
 "113333333332323",
 "113333333320323",
 "113333333201323",
 "113203332013323",
 "113203320133323",
 "113201201333323",
 "113212013333323",
 "113220133333323",
 "113201333333323",
 "113013333333323",
 "113333333333323",
 "122222222222223",
 "333333333333333"
end(checkedBox);

begin(emptyCircle)
 "222221111122222",
 "222110000011222",
 "221002222222122",
 "210122222222222",
 "210222222222222",
 "102222222222223",
 "102222222222223",
 "102222222222223",
 "102222222222223",
 "102222222222223",
 "211222222222232",
 "212222222222232",
 "221222222222322",
 "222232222233222",
 "222223333322222"
end(emptyCircle);

begin(filledCircle)
 "222221111122222",
 "222110000011222",
 "221002222212122",
 "210122333322222",
 "210233333333222",
 "102233333333223",
 "102333333333323",
 "102333333333323",
 "102333333333323",
 "102333333333323",
 "211233333333232",
 "212233333333232",
 "221222333322322",
 "222232222233222",
 "222223333322222"
end(filledCircle);

begin(upArrow)
 "    1    ",
 "    0    ",
 "   101   ",
 "   000   ",
 "  10001  ",
 "  00000  ",
 " 1000001 ",
 " 0000000 ",
 "100000001"
end(upArrow);

begin(downArrow)
 "100000001",
 " 0000000 ",
 " 1000001 ",
 "  00000  ",
 "  10001  ",
 "   000   ",
 "   101   ",
 "    0    ",
 "    1    "
end(downArrow);

begin(leftArrow)
 "        1",
 "      100",
 "    10000",
 "  1000000",
 "100000000",
 "  1000000",
 "    10000",
 "      100",
 "        1"
end(leftArrow);

/*
begin(rightArrow)
 "1        ",
 "001      ",
 "00001    ",
 "0000001  ",
 "000000001",
 "0000001  ",
 "00001    ",
 "001      ",
 "1        "
end(rightArrow);
*/
begin(rightArrow)
 "1    ",
 "01   ",
 "001  ",
 "0001 ",
 "00001",
 "0001 ",
 "001  ",
 "01   ",
 "1    "
end(rightArrow);

begin(upArrowGray)
 "    1    ",
 "    1    ",
 "   111   ",
 "   111   ",
 "  11111  ",
 "  11111  ",
 " 1111111 ",
 " 1111111 ",
 "111111111"
end(upArrowGray);

begin(downArrowGray)
 "111111111",
 " 1111111 ",
 " 1111111 ",
 "  11111  ",
 "  11111  ",
 "   111   ",
 "   111   ",
 "    1    ",
 "    1    "
end(downArrowGray);

begin(leftArrowGray)
 "        1",
 "      111",
 "    11111",
 "  1111111",
 "111111111",
 "  1111111",
 "    11111",
 "      111",
 "        1"
end(leftArrowGray);

begin(rightArrowGray)
 "1        ",
 "111      ",
 "11111    ",
 "1111111  ",
 "111111111",
 "1111111  ",
 "11111    ",
 "111      ",
 "1        "
end(rightArrowGray);

begin(cross)
 "01      10",
 "101    101",
 " 101  101 ",
 "  101101  ",
 "   1001   ",
 "   1001   ",
 "  101101  ",
 " 101  101 ",
 "101    101",
 "01      10"
end(cross);

begin(emptyCross)
 "01      10",
 "101    101",
 " 10    01 ",
 "          ",
 "          ",
 "          ",
 "          ",
 " 10    01 ",
 "101    101",
 "01      10"
end(emptyCross);

begin(window)
 "0000000000",
 "0000000000",
 "0000000000",
 "0222222220",
 "0222222220",
 "0222222220",
 "0222222220",
 "0222222220",
 "0222222220",
 "0000000000"
end(window);

begin(sphere)
 " 1000 ",
 "101111",
 "011222",
 "012233",
 "012333",
 " 1233 "
end(sphere);

begin(smallWhiteArrow)
 "01     ",
 "0211   ",
 "022211 ",
 "0222223",
 "022233 ",
 "0233   ",
 "03     "
end(smallWhiteArrow);

begin(smallGrayArrow)
 "01     ",
 "0311   ",
 "033311 ",
 "0333332",
 "033322 ",
 "0322   ",
 "02     "
end(smallGrayArrow);

begin(enterArrow)
 "          00001",
 "    00    0   3",
 "   011    0   3",
 "  01 100000   3",
 " 01           3",
 "01            3",
 " 0            3",
 "  0  3333333333",
 "   0 3         ",
 "    03         "
end(enterArrow);

begin(littleButton)
 "333333330   ",
 "322222210   ",
 "32222221011 ",
 "32222221011 ",
 "31111111011 ",
 "00000000011 ",
 "  111111111 ",
 "  111111111 ",
 "            "
end(littleButton);

begin(littlePressedButton)
 "333333330   ",
 "322222210   ",
 "322222210   ",
 "322222210111",
 "311111110111",
 "000000000111",
 "   111111111",
 "   111111111",
 "   111111111"
end(littlePressedButton);

#undef begin
#undef end

  return true;
}

/////////////////////////////////////////////////////////////////////////////
