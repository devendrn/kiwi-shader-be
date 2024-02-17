#ifndef SHADOW_UNIFORMS_H
#define SHADOW_UNIFORMS_H

// DeferredShading.fragment.sc
// RenderChunkForwardPBR.fragment.sc

uniform vec4 ShadowBias;
uniform vec4 ShadowSlopeBias;
uniform vec4 CloudShadowProj;
uniform vec4 ShadowPCFWidth;
uniform vec4 CascadeShadowResolutions;
uniform vec4 ShadowParams;
uniform vec4 EmissiveMultiplierAndDesaturationAndCloudPCFAndContribution;
uniform vec4 FirstPersonPlayerShadowsEnabledAndResolutionAndFilterWidth;
uniform mat4 PlayerShadowProj;

#endif
