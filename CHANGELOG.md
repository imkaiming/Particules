# Changelog

## [0.2.3] - Work in progress

### Added
- linked button that links the emission and duration parameters together
- sliders value are now displayed inside rotary sliders itself
- gradient-based glow to the rotary sliders arc value 
- composite components: main rotary slider with auxiliary jitter parameter
- updated SVG assets for play/pause button
- custom button menu with updated parameter visualization
- composite components: button menu controlled by an associated secondary slider
- vertical output meter 
- vignette overlay effect
- development tool to generate SVG shapes from LUT data for visual debugging
- on mouse hover feedback for interactive UI elements
- subtle grid overlay in audio thumbnail display

### Changed
- updated and pinned all git submodules to latest versions
- reworked all rotary sliders custom look and feel
- position and span controls are now into overlays for improved layout 
- centralized mathematical utilities shared between DSP and GUI (utils/math)
- merged Constants and PluginParams to centralized constants

### Fixed
- 

### Removed
- reduced JUCE module dependencies in sub-libraries

---

## [0.2.2] - 2026-03-25

### Changed
- renamed internal "frame" components to "panel" for clarity

### Fixed
- play button initialization state
- restored SharedCode interface in CMake configuration for compatibility with tests and benchmarks
- audio thumbnail persistence after UI reconstruction
- input buffer lifetime now persist across UI reopen

### Removed
- precompiled headers due to build instability
- temporarily disabled unit tests

---

## [0.2.1] - 2026-03-24

### Added
- introduced modular CMake targets to decouple DSP and GUI builds
- added project-wide namespace `particules`
- introduced enum classes for type safety (TraversalMode, EnvelopeMode, etc.)

### Changed
- refactored PhaseGenerator and LookUpTable using composition for improved modularity and reuse

### Fixed
- resolved build issues on Linux and macOS
- updated GitHub Actions workflows

---

## [0.2.0] - 2026-03-19

### Added
- migrated from Projucer to Pamplejuce template
- channel mixer with ITU-standard downmixing rules
- introduced parameter snapshot system for thread-safe DSP access
- lookup table system for grain envelope and modulation
- implemented a phase generator
- guard sample to eliminate modulo operations in interpolation
- introduced atomic shared pointer wrapper for audio buffer safety
- CI/CD pipeline with GitHub Actions
- some unit tests (WIP)
- debug preset loading for development : can direcly load file at plugin startup
- Melatonin Inspector integration for UI debugging

### Changed
- refactored grain processing loop from grains -> samples -> channels to samples -> grains -> channels order 
- migrated scheduler from block-based to sample-accurate processing
- converted grain class to POD for improved cache efficiency
- removed all allocations from audio thread for real-time safety
- replaced direct DSP access in GUI with lock-free double-buffered snapshots
- restructured project to reduce module dependencies
- decoupled AudioFileLoader from GUI and moved ownership to processor
- PluginProcessor role to orchestration and now use callback-based buffer injection

### Fixed
- grain timing desynchronization
- envelope discontinuities created audible clicks at low sustain ratios
- crashes at high grain densities (>200 grains/sec)
- Linux plugin validation (virtual display configuration)
- macOS code signing issues
- macOS packaging path resolution

### Removed
- removed deprecated `Particules.jucer` file

---

## [0.1.0] - 2026-02-04

### Added
- initial MVC-based architecture
- initial granular synthesis engine
- GUI prototype with custom rotary slider look and feel
- parameter system with APVTS attachments
- audio file loading into AudioBuffer
- audio thumbnail generation and callback system
- state persistence for parameters across UI reconstruction
- real-time visual feedback of grain positions
- migrated from from 7.0.3 to JUCE 8.0.12
- added project licensing GPLv3