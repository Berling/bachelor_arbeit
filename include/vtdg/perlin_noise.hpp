// C++ version of the improved perlin noise implementation by Ken perlin
// Based on the original Java implementation (http://mrl.nyu.edu/~perlin/noise/)
// JAVA REFERENCE IMPLEMENTATION OF IMPROVED NOISE - COPYRIGHT 2002 KEN PERLIN.

#pragma once

#include <array>

namespace vtdg {
  class perlin_noise {
  private:
    const static std::array<int, 512> p;

  public:
  	static float noise(float x, float y, float z);

  private:
  	static float fade(float t);
  	static float lerp(float t, float a, float b);
  	static float grad(int hash, float x, float y, float z);
  };
}
