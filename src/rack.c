#include "./rack.h"
#include "../std_mods/osc/mod.h"

void handle_rack(Rack *rack, float *out)
{
  unsigned long frame;
  for (frame = 0; frame < rack->config.framesPerBuffer; frame++)
  {
    // Iterate over all modules
    for (int moduleIndex = 0; moduleIndex < rack->moduleCount; moduleIndex++)
    {
      // This is probably very inefficient
      LoadedModule *lm = loadModule("osc");
      if (lm && lm->callbackModule)
      {
        lm->callbackModule(rack->modules[moduleIndex], rack->config);
      }
      else
      {
        fprintf(stderr, "Module or callbackModule function is null\n");
      }
    }

    // Mix the output for the current frame
    outputMixer(rack->master, rack->config, &out[frame]);
  }
}

// Set up a default rack
Rack setupDefaultRack(Config config)
{

  // Setup the oscillators
  Oscillator *oscData = (Oscillator *)malloc(sizeof(Oscillator));
  oscData->frequency = 440;
  oscData->waveform = SQUARE_WAVE;
  oscData->amplitude = 1;
  oscData->phase = 0;

  Oscillator *lfoData = (Oscillator *)malloc(sizeof(Oscillator));

  lfoData->frequency = 1;
  lfoData->waveform = SINE_WAVE;
  lfoData->amplitude = 1;
  lfoData->phase = 0;

  Module *oscModule = malloc(sizeof(Module));
  *oscModule = createBaseModule(oscData, (ModuleMeta){.lib = "osc", .name = "Oscillator"});

  Module *lfoModule = malloc(sizeof(Module));
  *lfoModule = createBaseModule(lfoData, (ModuleMeta){.lib = "osc", .name = "LFO"});

  // Set up the LFO to output into the input of the oscillator
  oscModule->in = (ModuleOut **)malloc(sizeof(ModuleOut *) * 1);
  oscModule->in[0] = lfoModule->out;

  // Create and set up a Mixer
  Mixer *mixer = (Mixer *)malloc(sizeof(Mixer));
  if (mixer == NULL)
  {
    // Handle allocation failure
    freeModule(oscModule);
    freeModule(lfoModule);
  }

  // Create and set up a MasterMixerChannel for the oscillator
  MasterMixerChannel oscChannel = (MasterMixerChannel){.moduleOut = oscModule->out};

  // Set the channel count in the mixer
  mixer->channelCount = 1;

  // Allocate memory for the channels array and add the oscillator channel

  mixer->channels = (MasterMixerChannel *)malloc(sizeof(MasterMixerChannel) * mixer->channelCount);
  if (mixer->channels == NULL)
  {
    // Handle allocation failure
    freeModule(oscModule);
    freeModule(lfoModule);
    free(mixer);
  }
  mixer->channels[0] = oscChannel;

  // Allocate memory for the array of Module pointers
  Module **modules = (Module **)malloc(sizeof(Module *) * 2);
  if (modules == NULL)
  {
    // Handle allocation failure
    freeModule(oscModule);
    freeModule(lfoModule);
    free(mixer);
  }
  modules[0] = lfoModule;
  modules[1] = oscModule;

  // Create and return the Rack object
  Rack rack = {
      .modules = modules,
      .moduleCount = 2, // Update module count to 2
      .master = mixer,
      .config = config};

  return rack;
}
