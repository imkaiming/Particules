# Particules - Granular Synthesizer

Particules is a real-time granular synthesizer built with JUCE.

It transforms an input audio buffer into a cloud of short grains, allowing precise or stochastic control over their distribution in time, position, and pitch.

---

## Overview

The instrument is designed around a simple set of parameters:

- **position**: location in the audio buffer where grains are read
- **span**: size of the region around the position
- **emission**: number of grains generated per second
- **duration**: lifetime of each grain in seconds
- **envelope**: amplitude shape applied to each grain
- **speed**: playback rate of the audio within each grain

This allow creation of:
- stable and textured sounds
- dynamic and evolving material

Keep in mind that the result is highly dependent on the input signal.

---

## Capabilities

- Control over the density of the grains cloud (emission and duration)
- Position and span control over the audio source
- Playback speed control
- Per-grain envelope shaping
- Deterministic or randomized grain behavior (jitter, WIP)

The engine supports continuous parameter changes during playback, enabling smooth transitions between textures.

---

## Architecture (high-level)

The system separates audio processing and interface into two independent parts that communicate in both directions:

- **UI -> DSP** (per audio block)  
  - parameter changes are captured and applied in the audio engine  
  - loading an audio file prepares the engine for playback  

- **DSP -> UI** (~60 Hz)  
  - runtime data such as active grain positions are exposed for visualization  

These exchanges are asynchronous and avoid direct coupling between the audio thread and the UI thread.

The audio engine is responsible for sound generation, while the interface handles interaction and visualization.

---

## Build

### Requirements
- CMake >= 3.22
- C++17 compatible compiler (**C++20 recommended**)
- JUCE (included as submodule)

### Notes on C++ version
- C++20 enables `std::atomic<std::shared_ptr>` (lock-free)
- C++17 uses a mutex-based fallback (not used in the audio thread)

### Build steps

```bash
git clone --recurse-submodules <repo-url>
cd Particules
cmake -B build
cmake --build build
```

---

### Planned Improvements

- Midi input compatibility (8 or 16 voices max)
- Stereo panning control
- Multiple regions selection/exclusion
- Reverse playback
- Improved waveform navigation (zoom)
- Modulator visual indicators
- ADSR implementation for each voices
- Undo manager
- One shot playback mode