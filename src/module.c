#include "module.h"

// Function to free a module
void freeModule(Module *module)
{
  if (module == NULL)
  {
    return;
  }

  // Free the output buffer if it was allocated
  // free(module->out->out);

  // Free the ModuleOut
  free(module->out);

  // Finally, free the module itself
  free(module);
}
