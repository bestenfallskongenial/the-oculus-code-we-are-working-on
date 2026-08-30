#include "kernel.h" 

void CKernel::generateWaveSinus(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];
    const float last = (float)(p_count - 1);

    for (int i = 0; i < p_count; ++i)
    {
        float t = (float)i / last;
        float s = sinf(3.14159265358979323846f * t);

        f_buffer[i] =
            (uint16_t)(s * s * (float)WAVE_MAX + 0.5f);
    }
}

/*
void CKernel::generateWaveCenterPeak(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];
    const float last = (float)(p_count - 1);

    for (int i = 0; i < p_count; ++i)
    {
        float x = (2.0f * (float)i / last) - 1.0f;
        float y = 1.0f - x * x;

        f_buffer[i] =
            (uint16_t)(y * (float)WAVE_MAX + 0.5f);
    }
}
*/

void CKernel::generateWaveTriangle(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];
    const float last = (float)(p_count - 1);

    for (int i = 0; i < p_count; ++i)
    {
        float t = (float)i / last;
        float y = 1.0f - fabsf(2.0f * t - 1.0f);

        f_buffer[i] =
            (uint16_t)(y * (float)WAVE_MAX + 0.5f);
    }
}

void CKernel::generateWaveRampUp(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];
    const float last = (float)(p_count - 1);

    for (int i = 0; i < p_count; ++i)
    {
        float t = (float)i / last;

        f_buffer[i] =
            (uint16_t)(t * (float)WAVE_MAX + 0.5f);
    }
}

void CKernel::generateWaveRampDown(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];
    const float last = (float)(p_count - 1);

    for (int i = 0; i < p_count; ++i)
    {
        float t = (float)i / last;
        float y = 1.0f - t;

        f_buffer[i] =
            (uint16_t)(y * (float)WAVE_MAX + 0.5f);
    }
}

void CKernel::generateWaveTrapezoid(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];
    const float last = (float)(p_count - 1);

    for (int i = 0; i < p_count; ++i)
    {
        float t = (float)i / last;
        float y;

        if (t < 0.25f)
            y = t * 4.0f;
        else if (t <= 0.75f)
            y = 1.0f;
        else
            y = (1.0f - t) * 4.0f;

        f_buffer[i] =
            (uint16_t)(y * (float)WAVE_MAX + 0.5f);
    }
}

void CKernel::generateWaveSmoothUp(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];
    const float last = (float)(p_count - 1);

    for (int i = 0; i < p_count; ++i)
    {
        float t = (float)i / last;
        float y = t * t * (3.0f - 2.0f * t);

        f_buffer[i] =
            (uint16_t)(y * (float)WAVE_MAX + 0.5f);
    }
}

void CKernel::generateWaveSmoothDown(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];
    const float last = (float)(p_count - 1);

    for (int i = 0; i < p_count; ++i)
    {
        float t = (float)i / last;
        float y = 1.0f - t * t * (3.0f - 2.0f * t);

        f_buffer[i] =
            (uint16_t)(y * (float)WAVE_MAX + 0.5f);
    }
}

/*
void CKernel::generateWaveSmoothPeak(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];
    const float last = (float)(p_count - 1);
    const float half = last * 0.5f;

    for (int i = 0; i < p_count; ++i)
    {
        float t;

        if ((float)i <= half)
            t = (float)i / half;
        else
            t = (last - (float)i) / half;

        float y = t * t * (3.0f - 2.0f * t);

        f_buffer[i] =
            (uint16_t)(y * (float)WAVE_MAX + 0.5f);
    }
}
*/

void CKernel::generateWaveExpRiseSlowFastDecay(char** p_buffer, int p_index, int p_count)
{
    uint16_t* f_buffer = (uint16_t*)p_buffer[p_index];

    const float k    = 5.0f;
    const float last = (float)(p_count - 1);
    const float half = last * 0.5f;
    const float norm = expf(k) - 1.0f;

    for (int i = 0; i < p_count; ++i)
    {
        float y;

        if ((float)i <= half)
        {
            float t = (float)i / half;

            y = (expf(k * t) - 1.0f) / norm;
        }
        else
        {
            float t = ((float)i - half) / half;

            y = 1.0f -
                (expf(k * t) - 1.0f) / norm;
        }

        f_buffer[i] =
            (uint16_t)(y * (float)WAVE_MAX + 0.5f);
    }
}