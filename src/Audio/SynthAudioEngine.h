// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

#include "audio/AudioEngine.h"

class SynthAudioEngine: public AudioEngine
{
public:
    void PlaySynthSound(float frequency, float duration, float volume = 1.f);

    float GetVariation(float detuneAmount);

private:
    float GetHarmonic(float phase) const;
    void SynthesizeAndQueue(float frequency, float duration, float& phase, float volume);
    float ComputeEnvelope(float tSec, float duration) const;
    float FrequencyToPhaseRad(float frequency) const;

    void HandleAudio(AudioData& voice, float* samples, int count) override;
};