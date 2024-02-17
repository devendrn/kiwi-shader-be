#ifndef LIGHTING_SAMPLERS_H
#define LIGHTING_SAMPLERS_H

// DeferredShading.fragment.sc
// RenderChunkForwardPBR.fragment.sc

struct LightSourceWorldInfo {
  vec4 worldSpaceDirection;
  vec4 diffuseColorAndIlluminance;
  vec4 shadowDirection;
  mat4 shadowProj0;
  mat4 shadowProj1;
  mat4 shadowProj2;
  mat4 shadowProj3;
  int isSun;
  int shadowCascadeNumber;
  int pad0;
  int pad1;
};

#ifndef DISABLE_DEFERRED
// not supported in fallback
BUFFER_RO(s_DirectionalLightSources, LightSourceWorldInfo, 2);
#endif

#endif
