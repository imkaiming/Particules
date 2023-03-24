// definition des macros à utiliser pour les variables et parametres

#define MIX_ID "MIX"
#define MIX_NAME "Mix"
#define MIX_MIN 0 
#define MIX_MAX 100
#define MIX_DEFAULT 100

#define GAIN_ID "GAIN"
#define GAIN_NAME "Gain"
#define GAIN_MIN -96.0
#define GAIN_MAX 0.0
#define GAIN_DEFAULT -1.0

// grain emissions per sec
#define DENSITY_ID "DENSITY"
#define DENSITY_NAME "Density"
#define DENSITY_MIN 0.1f // 0.1 grain per seconds (1 grain / 10 sec)
#define DENSITY_MAX 500.0f // 500 grains per seconds (1 grain / 0.002 s)
#define DENSITY_DEFAULT 1.0f

// length of emitted grains in sec
#define DURATION_ID "DURATION"
#define DURATION_NAME "Duration"
#define DURATION_MIN 0.002f // 0.002s for every emitted grains (1/ 0.002 = 500)
#define DURATION_MAX 10.0f // 10 sec for every emitted grains (1/ 10 = 0.1)
#define DURATION_DEFAULT 1.0f

#define PITCH_ID "PITCH"
#define PITCH_NAME "Pitch"
#define PITCH_MIN (-12.f)
#define PITCH_MAX 12.f
#define PITCH_DEFAULT 0.f

#define SPEED_ID "SPEED"
#define SPEED_NAME "Speed"
#define SPEED_MIN 0.1f
#define SPEED_MAX 2.f
#define SPEED_DEFAULT 1.f

#define ENVTYPE_ID "ENVTYPE"
#define ENVTYPE_NAME "EnvType"
#define ENVTYPE_DEFAULT 1
#define ENVTYPE_1 "Hann"
#define ENVTYPE_2 "Triangular"
#define ENVTYPE_3 "Hamming"

//#define FILEPATH_ID "FILEPATH"
//#define FILEPATH_NAME "FilePath"
//#define FILEPATH_DEFAULT ""

#define POSITION_ID "POSITION"
#define POSITION_NAME "Position"
#define POSITION_MIN 0.0f
#define POSITION_MAX 1.0f
#define POSITION_DEFAULT 0.0f

#define SELECTION_ID "SELECTION"
#define SELECTION_NAME "Selection"
#define SELECTION_MIN 0.01f
#define SELECTION_MAX 1.0f
#define SELECTION_DEFAULT 1.0f

#define ENVWIDTH_ID "ENVWIDTH"
#define ENVWIDTH_NAME "EnvWidth"
#define ENVWIDTH_MIN 0.01f
#define ENVWIDTH_MAX 1.0f
#define ENVWIDTH_DEFAULT 0.5f

typedef juce::AudioProcessorValueTreeState ValueTreeState;


