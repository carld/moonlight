/*
 * MLImageFactoryTIF.h (c) 2002 Gregor Mueckl
 *
 * (TODO: insert standard disclaimer here!)
 *
 */

#ifndef MLImageFactoryTIF_H
#define MLImageFactoryTIF_H

#include "image/MLImageFormat.h"

#include "image/tif/MLImageReaderTIF.h"
#include "image/tif/MLImageWriterTIF.h"

class MLImageFactoryTIF : public MLImageFactory {
public:
  MLImageFactoryTIF();

  virtual MLImageReader *getReader();
  virtual MLImageWriter *getWriter();
};

#endif
