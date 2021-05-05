/*！

 * @file       filter.c
 * @brief      滤波算法
 * @author     康夏涛（epic)
 * @version    v2.0
 * @date       2017-05-14
 
------------------------------------
*/

#include "filter.h"


/*

 二阶低通滤波算法
 
 算法来自开源四轴 - 向编写者致敬
 
 TOBE FIXED: Use filter object instead of repeated code
 
------------------------------------
*/
static float           _cutoff_freq1;
static float           _a11;
static float           _a21;
static float           _b01;
static float           _b11;
static float           _b21;
static float           _delay_element_11;        // buffered sample -1
static float           _delay_element_21;        // buffered sample -2
void LPF2pSetCutoffFreq_1(float sample_freq, float cutoff_freq)
{
    float fr =0;
    float ohm =0;
    float c =0;

    fr= sample_freq/cutoff_freq;
    ohm=tanf(M_PI_F/fr);
    c=1.0f+2.0f*cosf(M_PI_F/4.0f)*ohm + ohm*ohm;

    _cutoff_freq1 = cutoff_freq;
    if (_cutoff_freq1 > 0.0f)
    {
        _b01 = ohm*ohm/c;
        _b11 = 2.0f*_b01;
        _b21 = _b01;
        _a11 = 2.0f*(ohm*ohm-1.0f)/c;
        _a21 = (1.0f-2.0f*cosf(M_PI_F/4.0f)*ohm+ohm*ohm)/c;
    }
}

float LPF2pApply_1(float sample)
{

    float delay_element_0 = 0, output=0;
    if (_cutoff_freq1 <= 0.0f) {
        // no filtering
        return sample;
    }
    else
    {
        delay_element_0 = sample - _delay_element_11 * _a11 - _delay_element_21 * _a21;
        // do the filtering
        if (isnan(delay_element_0) || isinf(delay_element_0)) {
            // don't allow bad values to propogate via the filter
            delay_element_0 = sample;
        }
        output = delay_element_0 * _b01 + _delay_element_11 * _b11 + _delay_element_21 * _b21;

        _delay_element_21 = _delay_element_11;
        _delay_element_11 = delay_element_0;

        // return the value.  Should be no need to check limits
        return output;
    }
}

static float           _cutoff_freq2;
static float           _a12;
static float           _a22;
static float           _b02;
static float           _b12;
static float           _b22;
static float           _delay_element_12;        // buffered sample -1
static float           _delay_element_22;        // buffered sample -2
void LPF2pSetCutoffFreq_2(float sample_freq, float cutoff_freq)
{
    float fr =0;
    float ohm =0;
    float c =0;

    fr= sample_freq/cutoff_freq;
    ohm=tanf(M_PI_F/fr);
    c=1.0f+2.0f*cosf(M_PI_F/4.0f)*ohm + ohm*ohm;

    _cutoff_freq2 = cutoff_freq;
    if (_cutoff_freq2 > 0.0f)
    {
        _b02 = ohm*ohm/c;
        _b12 = 2.0f*_b02;
        _b22 = _b02;
        _a12 = 2.0f*(ohm*ohm-1.0f)/c;
        _a22 = (1.0f-2.0f*cosf(M_PI_F/4.0f)*ohm+ohm*ohm)/c;
    }
}

float LPF2pApply_2(float sample)
{

    float delay_element_0 = 0, output=0;
    if (_cutoff_freq2 <= 0.0f) {
        // no filtering
        return sample;
    }
    else
    {
        delay_element_0 = sample - _delay_element_12 * _a12 - _delay_element_22 * _a22;
        // do the filtering
        if (isnan(delay_element_0) || isinf(delay_element_0)) {
            // don't allow bad values to propogate via the filter
            delay_element_0 = sample;
        }
        output = delay_element_0 * _b02 + _delay_element_12 * _b12 + _delay_element_22 * _b22;

        _delay_element_22 = _delay_element_12;
        _delay_element_12 = delay_element_0;

        // return the value.  Should be no need to check limits
        return output;
    }
}

static float           _cutoff_freq3;
static float           _a13;
static float           _a23;
static float           _b03;
static float           _b13;
static float           _b23;
static float           _delay_element_13;        // buffered sample -1
static float           _delay_element_23;        // buffered sample -2
void LPF2pSetCutoffFreq_3(float sample_freq, float cutoff_freq)
{
    float fr =0;
    float ohm =0;
    float c =0;

    fr= sample_freq/cutoff_freq;
    ohm=tanf(M_PI_F/fr);
    c=1.0f+2.0f*cosf(M_PI_F/4.0f)*ohm + ohm*ohm;

    _cutoff_freq3 = cutoff_freq;
    if (_cutoff_freq3 > 0.0f)
    {
        _b03 = ohm*ohm/c;
        _b13 = 2.0f*_b03;
        _b23 = _b03;
        _a13 = 2.0f*(ohm*ohm-1.0f)/c;
        _a23 = (1.0f-2.0f*cosf(M_PI_F/4.0f)*ohm+ohm*ohm)/c;
    }
}

float LPF2pApply_3(float sample)
{

    float delay_element_0 = 0, output=0;
    if (_cutoff_freq3 <= 0.0f) {
        // no filtering
        return sample;
    }
    else
    {
        delay_element_0 = sample - _delay_element_13 * _a13 - _delay_element_23 * _a23;
        // do the filtering
        if (isnan(delay_element_0) || isinf(delay_element_0)) {
            // don't allow bad values to propogate via the filter
            delay_element_0 = sample;
        }
        output = delay_element_0 * _b03 + _delay_element_13 * _b13 + _delay_element_23 * _b23;

        _delay_element_23 = _delay_element_13;
        _delay_element_13 = delay_element_0;

        // return the value.  Should be no need to check limits
        return output;
    }
}
