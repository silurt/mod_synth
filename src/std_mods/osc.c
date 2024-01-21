#include "./osc.h"

// The first input of the oscillator module determines the amplitude

void oscillatorCallback(Module *module, Config config)
{
  Oscillator *osc = (Oscillator *)module->moduleData;

  // Calculate the phase increment based on the desired frequency
  double phaseIncrement = 2.0 * M_PI * osc->frequency / config.sampleRate;

  // Update the phase accumulator
  osc->phase += phaseIncrement;

  // Check if the phase has exceeded 2*pi (one cycle) and reset the phase to prevent it from growing indefinitely
  if (osc->phase >= 2.0 * M_PI)
    osc->phase -= 2.0 * M_PI;

  module->out->out = (float)(getGuardedInput(module, 0, osc->amplitude) * sin(osc->phase));
}

// Function to create an oscillator module
Module *createOscillatorModule(char *name, double frequency, double amplitude)
{
  // Allocate memory for the Oscillator
  Oscillator *osc = (Oscillator *)malloc(sizeof(Oscillator));
  if (osc == NULL)
  {
    return NULL;
  }
  osc->frequency = frequency;
  osc->amplitude = amplitude;
  osc->phase = 0;

  // Allocate memory for the Module
  Module *module = (Module *)malloc(sizeof(Module));
  if (module == NULL)
  {
    free(osc);
    return NULL;
  }

  module->name = name;
  // Allocate memory for ModuleOut
  module->out = (ModuleOut *)malloc(sizeof(ModuleOut));
  if (module->out == NULL)
  {
    free(osc);
    free(module);
    return NULL;
  }

  module->out->out = frequency;

  // Initialize the Module fields
  module->moduleData = osc;
  module->callback = oscillatorCallback;

  return module;
}
