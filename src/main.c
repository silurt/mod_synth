#include <stdio.h>
#include <math.h>
#include <portaudio.h>

#define SAMPLE_RATE 44100
#define FREQUENCY 440
#define AMPLITUDE 0.5
#define FRAMES_PER_BUFFER 64

typedef struct
{
  double phase;
} paTestData;

/* This routine will be called by the PortAudio engine when audio is needed. */
static int patestCallback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData)
{
  /* Cast data passed through stream to our structure. */
  paTestData *data = (paTestData *)userData;
  float *out = (float *)outputBuffer;
  unsigned long i;

  (void)inputBuffer; // Prevent unused variable warning

  for (i = 0; i < framesPerBuffer; i++)
  {
    *out++ = (float)(AMPLITUDE * sin(2 * M_PI * FREQUENCY * data->phase / SAMPLE_RATE));
    data->phase += 1.0;
    if (data->phase >= SAMPLE_RATE)
      data->phase -= SAMPLE_RATE;
  }
  return 0;
}

int main()
{
  PaStream *stream;
  PaError err;
  paTestData data;
  data.phase = 0;

  err = Pa_Initialize();
  if (err != paNoError)
    goto error;

  err = Pa_OpenDefaultStream(&stream,
                             0,         // No input channels
                             1,         // Mono output
                             paFloat32, // 32 bit floating point output
                             SAMPLE_RATE,
                             FRAMES_PER_BUFFER,
                             patestCallback,
                             &data);
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
