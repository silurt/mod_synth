#include "mixer.h"

float clamp(float value, float min, float max) {
  if (value < min) return min;
  if (value > max) return max;
  return value;
}

void outputMixer(Mixer *mixer, Config config, float *outFrame)
{
  // Initialize the output for this frame to zero
  *outFrame = 0.0f;
  (void)config;

  // Iterate over all channels in the mixer
  for (int channelIndex = 0; channelIndex < mixer->channelCount; channelIndex++)
  {
    MasterMixerChannel *channel = &(mixer->channels[channelIndex]);

    // Mix (sum) the output of each channel into the output frame
    // Assuming channel->moduleOut->out contains the current frame's data
    *outFrame += channel->moduleOut->out; // Add the channel's output to the mix
  }

  // Apply normalization or clipping here if necessary
  *outFrame = clamp(*outFrame, -1.0f, 1.0f);
}
