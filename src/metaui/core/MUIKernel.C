/*
 * MUIKernel.C (c) 2002 Gregor Mueckl
 */

#include <iostream>

#include "link.h" // specific to UNIX systems only

#include "MUIKernel.h"
#include "MUIInterfaceProvider.h"

using namespace std;

namespace MetaUI {
  Kernel *kernel;

  Kernel::Kernel()
  {
    interface=0;
    kernel=this;
  }

  Kernel::~Kernel()
  {
    if(interface)
      unloadBackend();
    kernel=0;
  }

  bool Kernel::loadBackend(char *filename)
  {
    InterfaceProvider *(*GetInterfaceProvider)();

    // Unload a backend if it is currently loaded
    if(interface!=0) {
      if(!unloadBackend()) {
	cerr << "MetaUI::Kernel::loadBackend: unloadBackend() failed" << endl;
	return false;
      }
    }

    libhandle=dlopen(filename,RTLD_NOW);
    if(!libhandle) {
      cerr << "MetaUI::Kernel::loadBackend: dlopen() failed on " << filename << endl;
      cerr << "reason: " << dlerror() << endl;
      return false;
    }

    GetInterfaceProvider=(InterfaceProvider *(*)())dlsym(libhandle,"GetInterfaceProvider");
    if(GetInterfaceProvider==0) {
      // XXX: crashes when calling dlclose for unknown reasons
      //      dlclose(libhandle);
      cerr << "MetaUI::Kernel::loadBackend: dlsym() failed" << endl;
      cerr << "reason: " << dlerror() << endl;
      return false;
    }

    interface=GetInterfaceProvider();
    if(!interface) {
      // XXX: crashes when calling dlclose for unknown reasons
      //      dlclose(libhandle);
      cerr << "MetaUI::Kernel::loadBackend: GetInterfaceProvider() failed" << endl;
      return false;
    }

    return true;
  }

  bool Kernel::unloadBackend()
  {
    delete interface;
    interface=0;

    // XXX: crashes when calling dlclose for unknown reasons
    //    dlclose(libhandle);
    return true;
  }

  
}
