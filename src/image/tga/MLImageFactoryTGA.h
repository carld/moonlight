/*
 * MLImageFactoryTGA.h (c) 2002 Gregor Mueckl
 *
 * (TODO: insert standard disclaimer here!)
 *
 */

#ifndef MLImageFactoryTGA_H
#define MLImageFactoryTGA_H

#include "image/MLImageFormat.h"

#include "image/tga/MLImageReaderTGA.h"

class MLImageFactoryTGA : public MLImageFactory {
public:
  MLImageFactoryTGA();

  virtual MLImageReader *getReader();
  virtual MLImageWriter *getWriter();
};

#endif
