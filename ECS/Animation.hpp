#pragma once
struct Animation
{
    int index, frames, speed;
    Animation() {}
    Animation(int i, int f, int s)
    {
        index = i;
        frames = f;
        speed = s;
    }
};
