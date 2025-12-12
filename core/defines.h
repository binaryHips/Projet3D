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

#define IMGSIZE 512

constexpr float heightFactor = 0.1;

// easifies the use of enum classes
template <typename E>
constexpr typename std::underlying_type<E>::type to_underlying(E e) {
    return static_cast<typename std::underlying_type<E>::type>(e);
}

enum class FEATURE_LAYERS {
    DESIRED_HEIGHT,
    WATER_INFlOW,
    WATER_OUTFLOW,
    MAX_
};

enum class MAP_LAYERS {
    BEDROCK,
    STONE,
    SAND,
    WATER,
    MAX_
};

enum class ATTRIBUTE_LAYERS {
    SEDIMENT,
    WATER_VELOCITY_U,
    WATER_VELOCITY_V,
    MAX_
};