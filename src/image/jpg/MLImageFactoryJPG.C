/*
 * MLImageFactoryJPG.C (c) 2002 Gregor Mueckl
 *
 * (TODO: insert standard disclaimer here!)
 *
 */

#include "image/jpg/MLImageFactoryJPG.h"

MLImageFactoryJPG::MLImageFactoryJPG()
{
  name="JPEG";

  fileExtensions.clear();
  fileExtensions.push_back(".jpg");
  fileExtensions.push_back(".jpeg");
}

MLImageReader *MLImageFactoryJPG::getReader()
{
  return new MLImageReaderJPG;
}

MLImageWriter *MLImageFactoryJPG::getWriter()
{
  return new MLImageWriterJPG;
}
