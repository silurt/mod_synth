#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include "./module.h"

#define MAX_MODULES 100

typedef void (*CreateModuleFunc)(void);
typedef void (*CallbackModuleFunc)(Module *module, Config config);

typedef struct
{
  char name[256];
  void *handle; // Handle to the dynamically-loaded library
  CreateModuleFunc createModule;
  CallbackModuleFunc callbackModule;
} LoadedModule;

LoadedModule *loadModule(const char *moduleName);
