// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

#include <vector>
#include "AudioData.h"

#include <SDL3/SDL_audio.h>

class AudioEngine
{
public:
    ~AudioEngine();
    void Init();

    void PlaySynthSoundMix(float frequency, float duration, float volume = 1.f);
    void PlaySynthSound(float frequency, float duration, float volume = 1.f);

    float GetVariation(float detuneAmount);

private:
    static void SDLCALL FeedAudioCallback(void* userdata, SDL_AudioStream* stream, int additional, int total);

    SDL_AudioStream* _stream = nullptr;

    std::vector<AudioData> _activeAudios;

    int _sampleRate = 48000;

    const float _minDuration = 0.1f;
    const float _baseVolume = 0.5f;

    float GetHarmonic(float phase) const;
    int GetNumSamples(float duration) const;
    void SynthesizeAndQueue(float frequency, float duration, float& phase, float volume);
    float ComputeEnvelope(float tSec, float duration) const;
    float FrequencyToPhaseRad(float frequency) const;
};