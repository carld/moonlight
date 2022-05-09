/*
 * MLImageFactoryRGB.C (c) 2002 Gregor Mueckl
 *
 * (TODO: insert standard disclaimer here!)
 *
 */

#include "image/rgb/MLImageFactoryRGB.h"

MLImageFactoryRGB::MLImageFactoryRGB()
{
  name="IRIS RGB";

  fileExtensions.clear();
  fileExtensions.push_back(".rgb");
}

MLImageReader *MLImageFactoryRGB::getReader()
{
  return new MLImageReaderRGB;
}

MLImageWriter *MLImageFactoryRGB::getWriter()
{
  return 0;
}
