#pragma once
#include <iostream>
#include <cmath>

// Definitions

#ifndef NDEBUG
#define FMDEBUG false
#else
#define FMDEBUG true
#endif

// Type aliases

using u32 = unsigned int;
using u16 = unsigned short;
using u8 = unsigned char;

using ParticleId = u32;
using Pixel = float;