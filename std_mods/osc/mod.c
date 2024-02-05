#include "./mod.h"

float getLocalGuardedInput(Module *module, int inputIndex, float fallback)
{
  if (module->in == NULL || module->in[inputIndex] == NULL)
    return fallback;
  return module->in[inputIndex]->out;
}

void callbackModule(Module *module, Config config)
{
  if (!module || !module->moduleData)
  {
    fprintf(stderr, "Null pointer encountered in callbackModule\n");
    return; // Exit the function early if any pointers are NULL
  }

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
  module->out->out = (float)(getLocalGuardedInput(module, 0, osc->amplitude) * output);
}

Module createModule(char *name)
{
  // Allocate memory for the Oscillator
  Oscillator *osc = (Oscillator *)malloc(sizeof(Oscillator));
  if (osc == NULL)
  {
    return (Module){0};
  }

  osc->frequency = 440;
  osc->waveform = SAWTOOTH_WAVE;
  osc->amplitude = 1;
  osc->phase = 0;

  // Allocate memory for the Module
  Module module;

  module.meta = (ModuleMeta){.name = name, .lib = "osc"};
  module.out = (ModuleOut *)malloc(sizeof(ModuleOut));
  module.out->out = osc->frequency;

  module.moduleData = osc;

  return module;
}
