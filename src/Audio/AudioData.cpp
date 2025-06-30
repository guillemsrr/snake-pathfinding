#include "AudioData.h"

bool AudioData::Equals(AudioData other) const
{
    return Frequency == other.Frequency && Volume == other.Volume;
}