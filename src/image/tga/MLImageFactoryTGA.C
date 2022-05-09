/*
 * MLImageFactoryTGA.C (c) 2002 Gregor Mueckl
 *
 * (TODO: insert standard disclaimer here!)
 *
 */

#include "image/tga/MLImageFactoryTGA.h"

MLImageFactoryTGA::MLImageFactoryTGA()
{
  name="TARGA";

  fileExtensions.clear();
  fileExtensions.push_back(".tga");
}

MLImageReader *MLImageFactoryTGA::getReader()
{
  return new MLImageReaderTGA;
}

MLImageWriter *MLImageFactoryTGA::getWriter()
{
  return 0;
}
