#include "rack.h"

void handle_rack(Rack *rack, float *out)
{
  unsigned long frame;
  for (frame = 0; frame < rack->config.framesPerBuffer; frame++)
  {
    // Iterate over all modules
    for (int moduleIndex = 0; moduleIndex < rack->moduleCount; moduleIndex++)
    {
      rack->modules[moduleIndex]->callback(rack->modules[moduleIndex], rack->config);
    }

    // Mix the output for the current frame
    outputMixer(rack->master, rack->config, &out[frame]);
  }
}

// Set up a default rack
Rack setupDefaultRack(Config config)
{
  // Create LFO to modify the oscillator
  Module *lfo = createOscillatorModule("LFO", 2, 1);
  if (lfo == NULL)
  {
    // Handle allocation failure
  }

  Module *osc = createOscillatorModule("OSC", 440, 1);
  if (osc == NULL)
  {
    // Handle allocation failure
  }

  // Set up the LFO to output into the input of the oscillator
  osc->in = (ModuleOut **)malloc(sizeof(ModuleOut *) * 1);
  osc->in[0] = lfo->out;

  // Create and set up a Mixer
  Mixer *mixer = (Mixer *)malloc(sizeof(Mixer));
  if (mixer == NULL)
  {
    // Handle allocation failure
    freeModule(osc);
    freeModule(lfo);
  }

  // Create and set up a MasterMixerChannel for the oscillator
  MasterMixerChannel oscChannel = (MasterMixerChannel){.moduleOut = osc->out};

  // Set the channel count in the mixer
  mixer->channelCount = 1;

  // Allocate memory for the channels array and add the oscillator channel
  mixer->channels = (MasterMixerChannel *)malloc(sizeof(MasterMixerChannel *) * mixer->channelCount);
  if (mixer->channels == NULL)
  {
    // Handle allocation failure
    freeModule(osc);
    freeModule(lfo);
    free(mixer);
  }
  mixer->channels[0] = oscChannel;

  // Allocate memory for the array of Module pointers
  Module **modules = (Module **)malloc(sizeof(Module *) * 2);
  if (modules == NULL)
  {
    // Handle allocation failure
    freeModule(osc);
    freeModule(lfo);
    free(mixer);
  }
  modules[0] = lfo;
  modules[1] = osc;

  // Create and return the Rack object
  Rack rack = {
      .modules = modules,
      .moduleCount = 2, // Update module count to 2
      .master = mixer,
      .config = config};

  return rack;
}
