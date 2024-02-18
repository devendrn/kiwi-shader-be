#ifndef TONEMAP_SAMPLERS_H
#define TONEMAP_SAMPLERS_H

// ColorPostProcessing.fragment.sc

SAMPLER2D(s_AverageLuminance, 0);
SAMPLER2D(s_ColorTexture, 1);
SAMPLER2D(s_CustomExposureCompensation, 2);
SAMPLER2D(s_MaxLuminance, 3);
SAMPLER2D(s_PreExposureLuminance, 4);
SAMPLER2D(s_RasterColor, 5);
SAMPLER2D(s_RasterizedColor, 6);

#endif
