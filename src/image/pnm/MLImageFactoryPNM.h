/*
 * MLImageFactoryPNM.h (c) 2002 Gregor Mueckl
 *
 * (TODO: insert standard disclaimer here!)
 *
 */

#ifndef MLImageFactoryPNM_H
#define MLImageFactoryPNM_H

#include "image/MLImageFormat.h"

#include "image/pnm/MLImageReaderPNM.h"
#include "image/pnm/MLImageWriterPNM.h"

class MLImageFactoryPNM : public MLImageFactory {
public:
  MLImageFactoryPNM();

  virtual MLImageReader *getReader();
  virtual MLImageWriter *getWriter();
};

#endif
