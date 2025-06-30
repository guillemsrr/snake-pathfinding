#include "AudioEngine.h"

#include <algorithm>
#include <random>

#include <SDL3/SDL_init.h>

AudioEngine::~AudioEngine()
{
    SDL_ClearAudioStream(_stream);
}

void AudioEngine::Init()
{
    SDL_AudioSpec spec;
    spec.channels = 2;
    spec.format = SDL_AUDIO_F32;
    spec.freq = _sampleRate;

    _stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, FeedAudioCallback, this);

    SDL_ResumeAudioStreamDevice(_stream);
}

void AudioEngine::PlaySynthSoundMix(float frequency, float duration, float volume)
{
    if (duration <= 0.f)
    {
        return;
    }

    duration = std::max(duration, _minDuration);

    AudioData audioData;
    audioData.Frequency = frequency;
    audioData.Duration = duration;
    audioData.Volume = volume;
    audioData.Elapsed = 0.f;
    audioData.Phase = 0.f;

    _activeAudios.push_back(audioData);
}

void AudioEngine::PlaySynthSound(float frequency, float duration, float volume)
{
    SDL_ClearAudioStream(_stream);

    float synthPhaseTemp = 0.0f;
    SynthesizeAndQueue(frequency, duration, synthPhaseTemp, volume);
}

void AudioEngine::SynthesizeAndQueue(float frequency, float duration, float& phase, const float volume)
{
    int numSamples = GetNumSamples(duration);
    const int bufferSizeBytes = sizeof(float) * numSamples;

    std::vector<float> buffer(numSamples);

    float phaseIncrement = FrequencyToPhaseRad(frequency) / _sampleRate;
    phaseIncrement *= GetVariation(0.1f);

    for (int i = 0; i < numSamples; ++i)
    {
        float tSec = static_cast<float>(i) / _sampleRate;
        float envelope = ComputeEnvelope(tSec, duration);
        float harmonic = GetHarmonic(phase);
        buffer[i] = volume * envelope * harmonic;

        phase += phaseIncrement;
        if (phase > 2.f * SDL_PI_F)
        {
            phase -= 2.f * SDL_PI_F;
        }
    }

    SDL_PutAudioStreamData(_stream, buffer.data(), bufferSizeBytes);
}

int AudioEngine::GetNumSamples(float duration) const
{
    int numSamples = static_cast<int>(_sampleRate * duration);
    return numSamples;
}

float AudioEngine::ComputeEnvelope(float tSec, float duration) const
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

float AudioEngine::GetHarmonic(float phase) const
{
    return SDL_sinf(phase) + SDL_sinf(2.f * phase) + SDL_sinf(3.f * phase);
}

float AudioEngine::GetVariation(float detuneAmount)
{
    float randVariation = 2.f * (static_cast<float>(rand()) / RAND_MAX) - 1.f;
    float variation = 1.0f + randVariation * detuneAmount;

    return variation;
}

void AudioEngine::FeedAudioCallback(void* userdata, SDL_AudioStream* stream, int additional, int total)
{
    auto* engine = static_cast<AudioEngine*>(userdata);
    if (!engine)
    {
        return;
    }

    additional /= sizeof(float);

    if (engine->_activeAudios.empty())
    {
        return;
    }

    while (additional > 0)
    {
        const int count = SDL_min(additional, 128);
        float samples[128] = {0.0f};
        
        for (AudioData& voice : engine->_activeAudios)
        {
            if (voice.Elapsed >= voice.Duration)
                continue;

            float phaseIncrement = engine->FrequencyToPhaseRad(voice.Frequency) / static_cast<float>(engine->
                _sampleRate);
            for (int i = 0; i < count; ++i)
            {
                if (voice.Elapsed >= voice.Duration)
                {
                    break;
                }

                float envelope = engine->ComputeEnvelope(voice.Elapsed, voice.Duration);
                float harmonic = engine->GetHarmonic(voice.Phase);
                float sampleValue = voice.Volume * envelope * harmonic;
                samples[i] += sampleValue;

                voice.Phase += phaseIncrement;
                float margin = 2.f * SDL_PI_F;
                if (voice.Phase > margin)
                {
                    voice.Phase -= margin;
                }

                voice.Elapsed += 1.f / static_cast<float>(engine->_sampleRate);
            }
        }

        for (int i = 0; i < count; ++i)
        {
            samples[i] *= engine->_baseVolume;
            samples[i] = std::clamp(samples[i], -1.0f, 1.0f);
        }

        SDL_PutAudioStreamData(stream, samples, static_cast<int>(count) * sizeof(float));
        additional -= count;
    }

    std::erase_if(engine->_activeAudios,
                  [](const AudioData& v)
                  {
                      return v.Elapsed >= v.Duration;
                  });
}

float AudioEngine::FrequencyToPhaseRad(float frequency) const
{
    return frequency * 2.f * SDL_PI_F;
}