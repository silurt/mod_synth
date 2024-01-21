#include <stdio.h>
#include <math.h>
#include <portaudio.h>

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

int main(void)
{
  PaStream *stream;
  PaError err;

  Rack rack = setupDefaultRack((Config){.framesPerBuffer = 64, .sampleRate = 44100});

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
