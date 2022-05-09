/*
 * MLImageFactoryRGB.C (c) 2002 Gregor Mueckl
 *
 * (TODO: insert standard disclaimer here!)
 *
 */

#ifndef MLImageFactoryRGB_H
#define MLImageFactoryRGB_H

#include "image/MLImageFormat.h"

#include "image/rgb/MLImageReaderRGB.h"

class MLImageFactoryRGB : public MLImageFactory {
public:
  MLImageFactoryRGB();

  virtual MLImageReader *getReader();
  virtual MLImageWriter *getWriter();
};

#endif
