#include "./osc.h"

void oscillatorCallback(Module *module, Config config)
{
  Oscillator *osc = (Oscillator *)module->moduleData;

  // Calculate the phase increment based on the desired frequency
  double phaseIncrement = 2.0 * M_PI * osc->frequency / config.sampleRate;

  // Update the phase accumulator
  osc->phase += phaseIncrement;

  // Check if the phase has exceeded 2*pi (one cycle)
  if (osc->phase >= 2.0 * M_PI)
  {
    // Reset the phase to prevent it from growing indefinitely
    osc->phase -= 2.0 * M_PI;
  }

  module->out->out = (float)(osc->amplitude * sin(osc->phase));

  // This is for testing if the phase is out of whack, will replace with some form of input handling
  osc->frequency += 0.01;
  if (osc->frequency >= 1000)
    osc->frequency = 0;
}

// Function to create an oscillator module
Module *createOscillatorModule(double frequency, double amplitude)
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

  // Allocate memory for ModuleOut
  module->out = (ModuleOut *)malloc(sizeof(ModuleOut));
  if (module->out == NULL)
  {
    free(osc);
    free(module);
    return NULL;
  }

  // Initialize the Module fields
  module->moduleData = osc;
  module->callback = oscillatorCallback;

  return module;
}
