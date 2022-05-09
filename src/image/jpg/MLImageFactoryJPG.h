/*
 * MLImageFactoryJPG.h (c) 2002 Gregor Mueckl
 *
 + (TODO: insert standard disclaimer here!)
 *
 */

#ifndef MLImageFactoryJPG_H
#define MLImageFactoryJPG_H

#include "image/MLImageFormat.h"

#include "image/jpg/MLImageReaderJPG.h"
#include "image/jpg/MLImageWriterJPG.h"

class MLImageFactoryJPG : public MLImageFactory {
public:
  MLImageFactoryJPG();

  virtual MLImageReader *getReader();
  virtual MLImageWriter *getWriter();
};

#endif
