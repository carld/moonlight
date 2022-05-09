/*
 * MLImageFactoryBMP.C (c) 2002 Gregor Mueckl
 *
 * (TODO: insert standard disclaimer here!)
 *
 */

#include "image/bmp/MLImageFactoryBMP.h"

MLImageFactoryBMP::MLImageFactoryBMP()
{
  name="Windows BMP";

  fileExtensions.clear();
  fileExtensions.push_back(".bmp");
}

MLImageReader *MLImageFactoryBMP::getReader()
{
  return new MLImageReaderBMP;
}

MLImageWriter *MLImageFactoryBMP::getWriter()
{
  return 0;
}
