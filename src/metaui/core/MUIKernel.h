/*
 * MUIKernel.h (c) 2002 Gregor Mueckl
 */

#ifndef MUIKernel_H
#define MUIKernel_H

namespace MetaUI {

  class InterfaceProvider;

  /**
   * The MetaUI kernel manages the dynamically loaded wiget lib backends
   * and widgets.
   */
  struct Kernel {
  protected:
    /**
     * Pointer to the currently active interface provider.
     */
    InterfaceProvider *interface;

    /**
     * Handle for the currently loaded backend shared library
     */
    void *libhandle;

  public:
    /**
     * Constructor doing only basic initialisation.
     */
    Kernel();
    /**
     * Destructor. If not yet done, it shuts down the GUI and unloads the
     * widget backend.
     */
    ~Kernel();

    /**
     * Load a widget backend from the given library file. Returns true
     * on succes, false on failure.
     */
    bool loadBackend(char *filename);
    /**
     * Frees all widgets, shuts the UI down  and unloads the currently 
     * loaded backend. Returns true on success, false on failure.
     */
    bool unloadBackend();
    /**
     * Get the currently active InterfaceProvider.
     */
    InterfaceProvider *getInterfaceProvider() { return interface; };
  };

  /**
   * A pointer to the single instance of the Kernel class.
   */
  extern Kernel *kernel;
};

#endif
