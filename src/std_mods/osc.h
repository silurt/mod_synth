#pragma once

#include <stdlib.h>
#include <math.h>

#include "../module.h"
#include "../config.h"

typedef struct Oscillator
{
  double phase;
  double frequency;
  double amplitude;
} Oscillator;

Module *createOscillatorModule(double frequency, double amplitude);
