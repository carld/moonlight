/*
 * MLImageFactoryGIF.C (c) 2002 Gregor Mueckl
 *
 * (TODO: insert standard disclaimer here!)
 *
 */

#include "image/gif/MLImageFactoryGIF.h"

MLImageFactoryGIF::MLImageFactoryGIF()
{
  name="GIF";

  fileExtensions.clear();
  fileExtensions.push_back(".gif");
}

MLImageReader *MLImageFactoryGIF::getReader()
{
  return new MLImageReaderGIF;
}

MLImageWriter *MLImageFactoryGIF::getWriter()
{
  return 0;
}
