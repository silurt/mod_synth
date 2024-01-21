# mod_synth

This is more for my personal education on sound stuff and c in general so horribly inefficient and also only builds on macOS

### Setup
```
  brew install portaudio
```
Note that mod_synth_c is optimized for macOS and links with PortAudio in /opt/homebrew/Cellar/.

### Build

### Compile
```
make
```

### Clean
```
make clean
```

### Dev Mode
```
make watch
```

### To-Dos:

- Implement dynamic loading for C modules.
- Set up audio routing between modules.
- Develop a real-time audio processing engine.
- Create an API for custom module creation.
- Build a GUI for module and signal path manipulation.
- Explore various synthesis techniques.