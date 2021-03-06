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
  QuitInterfaceDlg.C
  Fri Jan  2 13:43:04 1998

  Automatically generated by kBuilder
  Stephane Rehel, August 1996
*/

#include "QuitInterfaceDlg.h"

const int QuitInterfaceDlg::width = 254;
const int QuitInterfaceDlg::height= 104;

const unsigned char QuitInterfaceDlg::data[955] = {
 0x00,0x00,0x03,0xb7,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x0b,0x00,0x00,
 0x00,0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x38,0x00,0x00,0x00,0x2e,
 0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x66,0x78,0x9c,0xed,0x96,0x2b,0x72,
 0xc3,0x30,0x14,0x45,0x0d,0x0a,0x02,0xbb,0x8c,0xc2,0x80,0x6e,0xa2,0x4b,
 0xe8,0x12,0x0a,0x03,0x4b,0x0b,0x02,0x03,0x0a,0x0c,0x0d,0x02,0x0c,0x0a,
 0x04,0x02,0xb4,0x8d,0xc0,0x82,0x82,0xc2,0x2e,0xa0,0xc0,0xa0,0x20,0xbd,
 0xcd,0x4d,0x9e,0x5f,0xfc,0xd5,0x27,0x1f,0x65,0xa6,0x1e,0x8d,0xe6,0x45,
 0x92,0xa3,0xa3,0xe3,0x27,0xd9,0x76,0x65,0xb3,0xec,0x36,0xfd,0xb2,0xf9,
 0xd9,0xe4,0xaf,0x79,0xe2,0x05,0x9c,0x55,0x55,0x5d,0x1c,0xc3,0x85,0x73,
 0xd8,0x67,0xb9,0x2c,0x53,0x68,0xbf,0x52,0x9f,0x7a,0x15,0x49,0xc5,0x57,
 0xe7,0x93,0xe4,0xc9,0xd6,0xff,0x3e,0x4f,0xe7,0xf3,0xe4,0x9b,0xa2,0x08,
 0xbf,0x37,0xc6,0x67,0xb6,0xbf,0x46,0x46,0x6e,0xf1,0x26,0x37,0x13,0x14,
 0xc6,0xc1,0x3e,0x7d,0x6f,0x24,0xde,0xdd,0xe3,0x03,0xee,0x35,0xa5,0xe1,
 0xcf,0x81,0xf1,0x20,0x7c,0x9a,0x2d,0x30,0x7e,0x74,0x64,0xff,0x8c,0xde,
 0x3e,0x77,0x84,0xdf,0x15,0x6b,0xb4,0x80,0xa1,0xd3,0x2d,0xf0,0x6a,0xb0,
 0xa2,0xc4,0xf8,0xcf,0x8f,0x2f,0xd2,0x46,0xfa,0x64,0x19,0x8e,0x35,0x27,
 0xd6,0x08,0x80,0xf9,0x7c,0x41,0x06,0x9d,0x0c,0xb8,0xe8,0x10,0x23,0x31,
 0xa6,0xc8,0x8b,0xe9,0xfd,0x74,0xbd,0x7e,0x27,0xa7,0xe3,0x5c,0x12,0xc7,
 0xf8,0xc4,0xbc,0xa8,0x81,0x47,0x4b,0x0c,0xe8,0x8d,0xed,0x3a,0xe6,0xba,
 0x58,0x62,0x7c,0xba,0x17,0x72,0xb2,0x60,0x76,0x62,0x48,0x2c,0xf9,0xa0,
 0x63,0x19,0x2f,0x59,0xea,0x35,0x23,0x39,0x03,0x7d,0x56,0xf5,0xec,0x28,
 0x70,0x3b,0x1c,0x93,0x90,0x86,0xcf,0xe6,0x13,0x17,0x39,0x99,0x9f,0xb2,
 0x47,0x24,0x46,0xaf,0x3c,0xf7,0xbf,0x31,0xcb,0x52,0xe2,0x00,0x99,0xe2,
 0x53,0xa7,0xae,0xae,0xfb,0xda,0x65,0xa7,0xe0,0x5e,0x16,0xb2,0x0d,0xc7,
 0xed,0x43,0xc9,0x7d,0xde,0x00,0x9f,0x6d,0xb7,0xee,0x57,0xc4,0x44,0x75,
 0x7e,0xea,0x55,0xa4,0x16,0x47,0xfa,0x2c,0x0f,0xdf,0x9e,0x03,0xb6,0x63,
 0xa6,0xe8,0xf4,0xe9,0x45,0x88,0x37,0xce,0xf3,0x7c,0x31,0x7b,0xc9,0xe5,
 0x9d,0xd8,0x20,0x64,0xef,0x51,0x9e,0x7b,0x98,0x4f,0x32,0xec,0xce,0xf6,
 0x6d,0x8b,0xa6,0x45,0x6c,0xde,0xea,0xde,0xa2,0x34,0xa4,0x8d,0xf4,0x29,
 0x3f,0xed,0xca,0xba,0xc4,0x84,0x01,0x03,0x37,0x32,0x79,0x8c,0xb1,0x20,
 0x97,0x2e,0xdd,0x0b,0x66,0x91,0xec,0x3b,0x57,0x8c,0x4f,0x92,0xf0,0xc0,
 0x91,0xe3,0x54,0xac,0xca,0x79,0x25,0x5d,0xb4,0x1a,0xa3,0x54,0x7c,0x92,
 0xdc,0xb1,0x16,0x99,0x60,0x90,0x83,0x54,0xc3,0x34,0xba,0x88,0x4a,0xdb,
 0xbe,0x73,0xed,0x67,0x0c,0xf4,0x49,0x51,0x64,0x60,0xa0,0xbf,0x97,0xd8,
 0x2e,0xbd,0xe4,0x8c,0xd9,0x4a,0x61,0x3e,0x1b,0x8f,0x5e,0x8c,0xc9,0xd6,
 0x16,0xdb,0x3a,0x81,0xd1,0xde,0xfd,0x6f,0x85,0x9f,0x4f,0xaf,0xc4,0x26,
 0xaa,0x40,0xa2,0x96,0xf4,0x63,0x62,0x74,0x2e,0xc1,0xeb,0xff,0x0f,0xe7,
 0x0a,0xf4,0x69,0x8d,0xed,0x7c,0x2d,0xea,0x1c,0x6e,0x5f,0x01,0x99,0xa9,
 0x7d,0x06,0x40,0xf2,0xfb,0xc7,0xe5,0x91,0x69,0xec,0x30,0xc8,0x52,0xbd,
 0x8f,0xc2,0x6e,0xe7,0xc1,0xde,0x9b,0x75,0x66,0x97,0x7b,0xd9,0xf6,0x93,
 0xd5,0x7d,0x51,0xa3,0x3e,0x59,0xda,0xc3,0x7a,0xdb,0x8d,0x95,0x77,0x90,
 0x06,0x6e,0x77,0x05,0xfe,0xff,0xbe,0x3d,0x3c,0x3f,0xcf,0x5b,0x8b,0x4f,
 0xbd,0xa2,0x04,0xe3,0x03,0x9f,0x11,0xf9,0x73,0xe4,0xba,0x45,0xd2,0xf0,
 0xd9,0x5e,0x51,0x22,0xed,0xbd,0xf9,0xd9,0x76,0x7b,0xce,0x96,0xb1,0xfc,
 0x4c,0xb6,0xe8,0xf3,0x93,0x25,0xb5,0x58,0x7c,0x82,0xf3,0xe2,0xba,0x5c,
 0x7c,0x5e,0xd1,0x73,0xbf,0x38,0x86,0x23,0xa7,0xfe,0x06,0x4b,0x33,0x00,
 0xea,0x2f,0x88,0xa0,0x28,0x81,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x03,
 0x00,0x00,0x00,0x04,0x51,0x75,0x69,0x74,0x00,0x00,0x00,0x10,0x51,0x75,
 0x69,0x74,0x49,0x6e,0x74,0x65,0x72,0x66,0x61,0x63,0x65,0x44,0x6c,0x67,
 0x00,0x00,0x00,0xfe,0x00,0x00,0x00,0x68,0x00,0x00,0x00,0x06,0x00,0x00,
 0x00,0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x47,0x00,0x00,0x00,0x08,
 0x00,0x00,0x00,0xae,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x9e,0x00,0x00,
 0x00,0x1e,0x52,0x65,0x61,0x6c,0x6c,0x79,0x20,0x71,0x75,0x69,0x74,0x20,
 0x4d,0x6f,0x6f,0x6e,0x6c,0x69,0x67,0x68,0x74,0x20,0x43,0x72,0x65,0x61,
 0x74,0x6f,0x72,0x3f,0x00,0x00,0x00,0x1b,0x00,0x00,0x00,0x04,0x00,0x00,
 0x00,0x08,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x38,0x00,0x00,0x00,0x2e,
 0x00,0x00,0x00,0x9e,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x16,0x00,0x00,
 0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3e,0x00,0x00,0x00,0xfe,
 0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x9a,0x00,0x00,0x00,0x03,0x00,0x00,
 0x00,0x03,0x00,0x00,0x00,0x47,0x00,0x00,0x00,0x1f,0x00,0x00,0x00,0x55,
 0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x9c,0x00,0x00,0x00,0x07,0x26,0x43,
 0x61,0x6e,0x63,0x65,0x6c,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x02,0x00,
 0x00,0x00,0xa3,0x00,0x00,0x00,0x1f,0x00,0x00,0x00,0x55,0x00,0x00,0x00,
 0x18,0x00,0x00,0x00,0x9c,0x00,0x00,0x00,0x03,0x26,0x4f,0x4b,0x00,0x00,
 0x00,0x03,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x48,
 0x00,0x00,0x00,0xf0,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x9c,0x00,0x00,
 0x00,0x19,0x4f,0x6e,0x6c,0x79,0x20,0x26,0x69,0x63,0x6f,0x6e,0x69,0x66,
 0x79,0x20,0x69,0x74,0x20,0x28,0x41,0x6c,0x74,0x2d,0x46,0x39,0x29,0x00,
 0x00,0x00,0x00 };
