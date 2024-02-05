#include "./module_loader.h"

LoadedModule loadedModules[MAX_MODULES];
int loadedModuleCount = 0;

LoadedModule *loadModule(const char *moduleName)
{
  // Check if module is already loaded
  for (int i = 0; i < loadedModuleCount; ++i)
  {
    if (strcmp(loadedModules[i].name, moduleName) == 0)
    {
      return &loadedModules[i]; // Return the already loaded module
    }
  }
  printf("loading module '%s'\n", moduleName);

  // If not already loaded, load the module
  char path[1024];
  sprintf(path, "bin/std_mods/%s.dylib", moduleName); // Path to the module

  printf("loading path %s\n", path);
  void *handle = dlopen(path, RTLD_LAZY);
  if (!handle)
  {
    fprintf(stderr, "Error loading module: %s\n", dlerror());
    return NULL;
  }

  // Store the loaded module in the array
  if (loadedModuleCount < MAX_MODULES)
  {
    LoadedModule lm;

    // Copy module name and ensure it's null-terminated
    strncpy(lm.name, moduleName, 255);
    lm.name[255] = '\0';

    // Assign other properties
    lm.handle = handle;
    lm.createModule = (CreateModuleFunc)dlsym(handle, "createModule");
    lm.callbackModule = (CallbackModuleFunc)dlsym(handle, "callbackModule");

    // Store the populated module in the array BEFORE incrementing the count
    loadedModules[loadedModuleCount] = lm;

    // Now, you can safely increment the module count
    ++loadedModuleCount;

    LoadedModule *lmpointer = &loadedModules[loadedModuleCount - 1];

    // Return a pointer to the module you just added
    return lmpointer;
  }
  else
  {
    fprintf(stderr, "Reached maximum module count\n");
    dlclose(handle);
    return NULL;
  }
}
