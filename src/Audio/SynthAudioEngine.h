// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

#include "audio/AudioEngine.h"

class SynthAudioEngine: public AudioEngine
{
public:
    float GetVariation(float detuneAmount);

private:
    float GetHarmonic(float phase) const;
    float ComputeEnvelope(float tSec, float duration) const;
    float FrequencyToPhaseRad(float frequency) const;

    void HandleAudio(AudioData& voice, float* samples, int count) override;
};