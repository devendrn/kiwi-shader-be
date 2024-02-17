#ifndef FRAG_INFO_SAMPLERS_H
#define FRAG_INFO_SAMPLERS_H

// DeferredShading.fragment.sc
// RenderChunkForwardPBR.fragment.sc

SAMPLER2D(s_ColorMetalness, 0);
SAMPLER2D(s_SceneDepth, 11);
SAMPLER2D(s_Normal, 6);
SAMPLER2D(s_EmissiveAmbientLinearRoughness, 3);

#endif

