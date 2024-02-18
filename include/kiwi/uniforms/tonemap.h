#ifndef TONEMAP_UNIFORMS_H
#define TONEMAP_UNIFORMS_H

// ColorPostProcessing.fragment.sc

uniform vec4 ExposureCompensation;
uniform vec4 LuminanceMinMax;
uniform vec4 ColorGrading_Contrast;
uniform vec4 ColorGrading_Saturation;
uniform vec4 OutputTextureMaxValue;
uniform vec4 RenderMode;
uniform vec4 ScreenSize;
uniform vec4 TonemapCorrection;
uniform vec4 TonemapParams0;

#endif
