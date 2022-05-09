/*
 * MLImageFactoryGIF.h (c) 2002 Gregor Mueckl
 *
 * (TODO: insert standard disclaimer here!)
 *
 */

#ifndef MLImageFactoryGIF_H
#define MLImageFactoryGIF_H

#include "image/MLImageFormat.h"

#include "image/gif/MLImageReaderGIF.h"

class MLImageFactoryGIF : public MLImageFactory {
public:
  MLImageFactoryGIF();

  virtual MLImageReader *getReader();
  virtual MLImageWriter *getWriter();
};

#endif
