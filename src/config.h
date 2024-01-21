#pragma once

#include <stdio.h>
#include <math.h>
#include <portaudio.h>

#include "./module.h"

typedef struct
{
  unsigned long sampleRate;
  unsigned long framesPerBuffer;
} Config;
