// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

struct AudioData
{
    float Frequency;
    float Duration;
    float Elapsed;
    float Phase;
    float Volume;

    bool Equals(AudioData other) const;
};