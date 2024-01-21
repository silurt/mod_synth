#include <stdio.h>
#include <math.h>
#include <portaudio.h>

#include "std_mods/osc.h"
#include "rack.h"

static int paCallback(const void *inputBuffer, void *outputBuffer,
                      unsigned long framesPerBuffer,
                      const PaStreamCallbackTimeInfo *timeInfo,
                      PaStreamCallbackFlags statusFlags,
                      void *rackData)
{
  // Mark unused parameters to avoid compiler warnings
  (void)inputBuffer;
  (void)framesPerBuffer;
  (void)timeInfo;
  (void)statusFlags;

  handle_rack((Rack *)rackData, (float *)outputBuffer);

  return 0;
}

Rack setUpRack(Config config)
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

int main(void)
{
  PaStream *stream;
  PaError err;

  Rack rack = setUpRack((Config){.framesPerBuffer = 64, .sampleRate = 44100});

  err = Pa_Initialize();
  if (err != paNoError)
    goto error;

  err = Pa_OpenDefaultStream(&stream,
                             0,         // No input channels
                             1,         // Mono output
                             paFloat32, // 32 bit floating point output
                             rack.config.sampleRate,
                             rack.config.framesPerBuffer,
                             paCallback,
                             &rack);
  if (err != paNoError)
    goto error;

  err = Pa_StartStream(stream);
  if (err != paNoError)
    goto error;

  printf("Playing a sine wave. Press Enter to stop.\n");
  getchar();

  err = Pa_StopStream(stream);
  if (err != paNoError)
    goto error;

  err = Pa_CloseStream(stream);
  if (err != paNoError)
    goto error;

  Pa_Terminate();
  printf("Finished.\n");
  return 0;

error:
  Pa_Terminate();
  fprintf(stderr, "An error occurred while using the PortAudio stream\n");
  fprintf(stderr, "Error number: %d\n", err);
  fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
  return -1;
}
