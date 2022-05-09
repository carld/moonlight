/*
 * MLImageFactoryBMP.h (c) 2002 Gregor Mueckl
 *
 * (TODO: insert standard disclaimer here!)
 *
 */

#ifndef MLImageFactoryBMP_H
#define MLImageFactoryBMP_H

#include "image/MLImageFormat.h"

#include "image/bmp/MLImageReaderBMP.h"

class MLImageFactoryBMP : public MLImageFactory {
public:
  MLImageFactoryBMP();

  virtual MLImageReader *getReader();
  virtual MLImageWriter *getWriter();
};

#endif
