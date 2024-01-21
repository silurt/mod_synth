#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <portaudio.h>

#include "config.h"

typedef struct ModuleOut
{
  float out;
} ModuleOut;

typedef struct Module
{
  void *moduleData;
  void (*callback)(struct Module *self, Config config);
  ModuleOut *out;
} Module;

void freeModule(Module *module);
