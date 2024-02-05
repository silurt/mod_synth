#include "module.h"

// Function to free a module
void freeModule(Module *module)
{
  if (module == NULL)
  {
    return;
  }

  free(module->out);
  free(module);
}

float getGuardedInput(Module *module, int inputIndex, float fallback)
{
  if (module->in == NULL || module->in[inputIndex] == NULL)
    return fallback;
  return module->in[inputIndex]->out;
}

Module createBaseModule(void *moduleData, ModuleMeta meta)
{
  Module mod = (Module){.meta = meta, .moduleData = moduleData};
  mod.out = malloc(sizeof(*mod.out));
  if (mod.out == NULL)
  {
    fprintf(stderr, "Failed to allocate memory for module->out\n");
  }

  return mod;
}
