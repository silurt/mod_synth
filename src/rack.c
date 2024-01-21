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
