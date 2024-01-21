#pragma once

#include <stdio.h>
#include <math.h>
#include <portaudio.h>

#include "./mixer.h"
#include "./config.h"
#include "./std_mods/std_mods.h"

typedef struct
{
  Module **modules;
  int moduleCount;
  Mixer *master;
  Config config;
  int currentModuleIndex;
} Rack;

void handle_rack(Rack *rack, float *out);
Rack setupDefaultRack(Config config);
