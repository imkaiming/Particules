#pragma once


inline float lerp(float a, float b, float f) { return a * (1.0f - f) + (b * f); }