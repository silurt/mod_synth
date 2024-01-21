#pragma once

#include <stdlib.h>
#include <math.h>

// They dropped it in c99 for some reason: https://ubuntuforums.org/showthread.php?t=583094
#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

#include "../module.h"
#include "../config.h"

// Enumeration for different waveform types
typedef enum
{
  SINE_WAVE,
  TRIANGLE_WAVE,
  SAWTOOTH_WAVE,
  SQUARE_WAVE
} WaveformType;

typedef struct Oscillator
{
  double phase;
  double frequency;
  double amplitude;
  WaveformType waveform;
} Oscillator;

Module *createOscillatorModule(char* name, double frequency, double amplitude, WaveformType waveform);
