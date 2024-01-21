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

  float output = 0.0; // Initialize the output value

  // Switch case to select the waveform
  switch (osc->waveform)
  {
  case TRIANGLE_WAVE:
    // Implement triangle waveform generation here
    // Calculate the triangle waveform based on phase
    if (osc->phase < M_PI)
      output = (-1.0 + (2.0 * osc->phase / M_PI));
    else
      output = (3.0 - (2.0 * osc->phase / M_PI));
    break;
  case SAWTOOTH_WAVE:
    // Implement sawtooth waveform generation here
    // Calculate the sawtooth waveform based on phase
    output = (-1.0 + (2.0 * osc->phase / (2.0 * M_PI)));
    break;
  case SQUARE_WAVE:
    // Implement square waveform generation here
    // Square wave is essentially a pulse with 50% duty cycle
    if (osc->phase < M_PI)
      output = 1.0;
    else
      output = -1.0;
    break;
  case SINE_WAVE:
  default:
    // Default to sine wave if waveform type is not recognized
    output = sin(osc->phase);
    break;
  }

  module->out->out = (float)(getGuardedInput(module, 0, osc->amplitude) * output);
}

// Function to create an oscillator module
Module *createOscillatorModule(char *name, double frequency, double amplitude, WaveformType waveform)
{
  // Allocate memory for the Oscillator
  Oscillator *osc = (Oscillator *)malloc(sizeof(Oscillator));
  if (osc == NULL)
  {
    return NULL;
  }
  osc->frequency = frequency;
  osc->waveform = waveform;
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
