# Particules – Granular Synthesizer

**Particules** is a real-time **granular synthesis audio plugin** using the **JUCE framework v7.0.5**.

The project explores **grain-based sound generation**, real-time parameter control, and performance-aware audio processing, with a focus on **clarity, stability, and low-latency behavior**.

---

## Features

- Adjustable grain parameters:
  - grain size
  - Emission
  - position / spread
  - pitch
- Envelope shaping per grain
- Deterministic and stochastic grain triggering
- Parameter automation support
- Designed with **real-time audio constraints** in mind (audio thread safety)
  
---

## Technical Overview

---

## Build Instructions

### Requirements
- JUCE (tested with a recent JUCE 7.x version)
- Projucer
- C++17 compatible compiler

### Installation
1. download and install JUCE Framework https://github.com/juce-framework/JUCE
2. Compile Projucer with your IDE
3. Configure Projucer and choose your IDE 
4. Now you can create a new project for Audio Plugin VST
5. Import Particules source folder in the projucer

## Possible Improvments

- adding keyboard shortcuts
- creating presets
- more complexe grains processing
- 
