#pragma once

#include <stdio.h>
#include <math.h>
#include <portaudio.h>

#include "./module.h"

typedef struct
{
  ModuleOut *moduleOut;
} MasterMixerChannel;

typedef struct
{
  MasterMixerChannel *channels;
  int channelCount;
} Mixer;

void outputMixer(Mixer *mixer, Config config, float *out);
