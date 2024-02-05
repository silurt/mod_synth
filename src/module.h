#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <portaudio.h>

#include "config.h"

typedef struct
{
  float out;
} ModuleOut;

typedef struct
{
  // The library its supposed to run
  char *lib;
  char *name;
} ModuleMeta;

typedef struct Module
{
  void *moduleData;
  // void (*callback)(struct Module *self, Config config);
  ModuleOut *out;
  ModuleOut **in;

  ModuleMeta meta;
} Module;

void freeModule(Module *module);
float getGuardedInput(Module *module, int inputIndex, float fallback);
Module createBaseModule(void *moduleData, ModuleMeta meta);
