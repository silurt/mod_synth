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
  // Create an oscillator module that outputs A4
  Module *osc = createOscillatorModule(440, 0.5);
  if (osc == NULL)
  {
    // Handle allocation failure
  }

  // Allocate memory for the array of Module pointers
  Module **modules = (Module **)malloc(sizeof(Module *) * 1);
  if (modules == NULL)
  {
    // Handle allocation failure
    freeModule(osc);
  }
  modules[0] = osc;

  // Create and set up a MasterMixerChannel for the oscillator
  MasterMixerChannel *oscChannel = (MasterMixerChannel *)malloc(sizeof(MasterMixerChannel));
  if (oscChannel == NULL)
  {
    // Handle allocation failure
    freeModule(osc);
    free(modules);
  }
  *oscChannel = (MasterMixerChannel){.moduleOut = osc->out};

  // Create and set up a Mixer
  Mixer *mixer = (Mixer *)malloc(sizeof(Mixer));
  if (mixer == NULL)
  {
    // Handle allocation failure
    freeModule(osc);
    free(modules);
    free(oscChannel);
  }
  *mixer = (Mixer){.channels = oscChannel, .channelCount = 1};

  // Create and return the Rack object
  Rack rack = {
      .modules = modules,
      // Lets just manually set the count for now
      .moduleCount = 1,
      .master = mixer,
      .config = config};

  return rack;
}
