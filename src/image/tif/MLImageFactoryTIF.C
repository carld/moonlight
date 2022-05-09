/*
 * MLImageFactoryTIF.C (c) 2002 Gregor Mueckl
 *
 * (TODO: insert standard disclaimer here)
 *
 */

#include "image/tif/MLImageFactoryTIF.h"

MLImageFactoryTIF::MLImageFactoryTIF()
{
  name="TIFF";

  fileExtensions.clear();
  fileExtensions.push_back(".tif");
  fileExtensions.push_back(".tiff");
}

MLImageReader *MLImageFactoryTIF::getReader()
{
  return new MLImageReaderTIF;
}

MLImageWriter *MLImageFactoryTIF::getWriter()
{
  return new MLImageWriterTIF;
}
