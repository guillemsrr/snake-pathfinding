#include "AudioEngine.h"

#include <algorithm>
#include <random>

#include <glm/vec3.hpp>

#include <SDL3/SDL_init.h>

void AudioEngine::Init()
{
    SDL_AudioSpec spec;
    spec.channels = 1;
    spec.format = SDL_AUDIO_F32;
    spec.freq = sampleRate;
    stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr);

    SDL_ResumeAudioStreamDevice(stream);
}

void AudioEngine::PlaySynthSound(float frequency, float duration)
{
    int numSamples = GetNumSamples(duration);
    int bufferSizeBytes = sizeof(float) * numSamples;

    if (SDL_GetAudioStreamQueued(stream) > bufferSizeBytes)
    {
        return;
    }

    SynthesizeAndQueue(frequency, duration, synthPhase, 0.5f);
}

void AudioEngine::PlayOneShotSound(float frequency, float duration, float volume)
{
    float synthPhaseTemp = 0.0f;
    SynthesizeAndQueue(frequency, duration, synthPhaseTemp, volume);
}

void AudioEngine::SynthesizeAndQueue(float frequency, float duration, float& phase, const float volume)
{
    int numSamples = GetNumSamples(duration);
    const int bufferSizeBytes = sizeof(float) * numSamples;

    std::vector<float> buffer(numSamples);

    float phaseIncrement = FrequencyToPhaseRad(frequency) / sampleRate;
    phaseIncrement *= GetVariation(0.1f);

    for (int i = 0; i < numSamples; ++i)
    {
        float tSec = static_cast<float>(i) / sampleRate;
        float envelope = ComputeEnvelope(tSec, duration);

        float harmonic = SDL_sinf(phase) + SDL_sinf(2.f * phase) + SDL_sinf(3.f * phase);
        buffer[i] = volume * envelope * harmonic;

        phase += phaseIncrement;
        if (phase > 2.f * SDL_PI_F)
        {
            phase -= 2.f * SDL_PI_F;
        }
    }

    SDL_PutAudioStreamData(stream, buffer.data(), bufferSizeBytes);
}

int AudioEngine::GetNumSamples(float duration) const
{
    int numSamples = static_cast<int>(sampleRate * duration);
    return std::max(numSamples, minNumSamples);
}

float AudioEngine::ComputeEnvelope(float tSec, float duration) const
{
    if (duration <= 0.f)
        return 1.0f;

    const float attack = 0.01f;
    const float release = duration * 0.8f;

    if (tSec < attack)
        return tSec / attack;

    return expf(-5.0f * (tSec - attack) / release);
}

float AudioEngine::GetHarmonic(glm::vec3 harmonicAxis) const
{
    return harmonicAxis.y * SDL_sinf(synthPhase) + harmonicAxis.x * SDL_sinf(2.0f * synthPhase) + harmonicAxis.z *
        SDL_sinf(3.0f * synthPhase);
}

float AudioEngine::GetVariation(float detuneAmount)
{
    float randVariation = 2.f * (static_cast<float>(rand()) / RAND_MAX) - 1.f;
    float variation = 1.0f + randVariation * detuneAmount;

    return variation;
}

float AudioEngine::FrequencyToPhaseRad(float frequency) const
{
    return frequency * 2.f * SDL_PI_F;
}