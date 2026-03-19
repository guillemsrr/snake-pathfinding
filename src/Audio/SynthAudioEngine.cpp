#include "SynthAudioEngine.h"
#include <random>
#include <SDL3/SDL_init.h>


float SynthAudioEngine::ComputeEnvelope(float tSec, float duration) const
{
    if (duration <= 0.f)
    {
        return 1.f;
    }

    const float attack = 0.01f;
    const float release = duration * 0.8f;

    if (tSec < attack)
    {
        return tSec / attack;
    }

    return expf(-5.0f * (tSec - attack) / release);
}

float SynthAudioEngine::GetHarmonic(float phase) const
{
    return SDL_sinf(phase) + SDL_sinf(2.f * phase) + SDL_sinf(3.f * phase);
}

float SynthAudioEngine::GetVariation(float detuneAmount)
{
    float randVariation = 2.f * (static_cast<float>(rand()) / RAND_MAX) - 1.f;
    float variation = 1.0f + randVariation * detuneAmount;

    return variation;
}

float SynthAudioEngine::FrequencyToPhaseRad(float frequency) const
{
    return frequency * 2.f * SDL_PI_F;
}

void SynthAudioEngine::HandleAudio(AudioData& voice, float* samples, int count)
{
    //AudioEngine::HandleAudio(voice, samples, count);

    float phaseIncrement = FrequencyToPhaseRad(voice.Frequency) / static_cast<float>(_sampleRate);
    for (int i = 0; i < count; ++i)
    {
        if (voice.Elapsed >= voice.Duration)
        {
            break;
        }

        float envelope = ComputeEnvelope(voice.Elapsed, voice.Duration);
        float harmonic = GetHarmonic(voice.Phase);
        float sampleValue = voice.Volume * envelope * harmonic;
        samples[i] += sampleValue;

        voice.Phase += phaseIncrement;
        float margin = 2.f * SDL_PI_F;
        if (voice.Phase > margin)
        {
            voice.Phase -= margin;
        }

        voice.Elapsed += 1.f / static_cast<float>(_sampleRate);
    }
}
