// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

#include <glm/vec3.hpp>

#include "Pathfinding/Algorithms/AStarPathfinder.h"
#include "Pathfinding/Algorithms/AStarPathfinder.h"
#include "Pathfinding/Algorithms/AStarPathfinder.h"
#include "Pathfinding/Algorithms/AStarPathfinder.h"
#include "Pathfinding/Algorithms/AStarPathfinder.h"
#include "Pathfinding/Algorithms/AStarPathfinder.h"
#include "Pathfinding/Algorithms/AStarPathfinder.h"
#include "Pathfinding/Algorithms/AStarPathfinder.h"
#include "Pathfinding/Algorithms/AStarPathfinder.h"
#include "Pathfinding/Algorithms/AStarPathfinder.h"
#include "Pathfinding/Algorithms/AStarPathfinder.h"
#include "Pathfinding/Algorithms/AStarPathfinder.h"

#include <SDL3/SDL_audio.h>

class AudioEngine
{
public:
    void Init();

    void PlaySynthSound(float frequency, float duration);
    void PlayOneShotSound(float frequency, float duration, float volume);

private:
    SDL_AudioStream* stream = nullptr;

    int current_sine_sample = 0;
    int sampleRate = 8000;
    int minNumSamples = 512;
    float synthPhase = 0.0f;
    float currentFreq = 440.0f;

    float GetHarmonic(glm::vec3 harmonicAxis) const;
    int GetNumSamples(float duration) const;
    void SynthesizeAndQueue(float frequency, float duration, float& phase, float volume);
    float ComputeEnvelope(float tSec, float duration) const;
    float GetVariation(float detuneAmount);
    float FrequencyToPhaseRad(float frequency) const;
};