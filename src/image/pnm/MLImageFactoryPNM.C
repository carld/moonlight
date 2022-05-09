/*
 * MLImageFactoryPNM.C (c) 2002 Gregor Mueckl
 *
 * (TODO: insert standard disclaimer here!)
 *
 */

#include "image/pnm/MLImageFactoryPNM.h"

MLImageFactoryPNM::MLImageFactoryPNM()
{
  name="PNM";

  fileExtensions.clear();
  fileExtensions.push_back(".pbm");
  fileExtensions.push_back(".pgm");
  fileExtensions.push_back(".pnm");
  fileExtensions.push_back(".ppm");
}

MLImageReader *MLImageFactoryPNM::getReader()
{
  return new MLImageReaderPNM;
}

MLImageWriter *MLImageFactoryPNM::getWriter()
{
  return new MLImageWriterPNM;
}
