#ifndef K_LIGHTING_H
#define K_LIGHTING_H

#include "../uniforms/lighting.h"
#include "../samplers/lighting.h"
#include "shadow.h"

void evaluateDirectionalLighting(
  inout vec3 light, float viewDepth, vec3 worldPos, vec3 n, float skyAmbient
) {
  int lightCount = int(DirectionalLightToggleAndCountAndMaxDistanceAndMaxCascadesPerLight.y);
  for(int i=0; i<lightCount; i++) {
    #ifndef DISABLE_DEFERRED
      // s_DirectionalLightSources is not supported in fallback
      vec4 lightDirection = s_DirectionalLightSources[i].worldSpaceDirection;
      vec4 colorAndLuminance = s_DirectionalLightSources[i].diffuseColorAndIlluminance;
      vec3 sl = normalize((u_view * s_DirectionalLightSources[i].shadowDirection).xyz);
    #else 
      vec4 lightDirection;
      vec4 colorAndLuminance;
      vec3 sl;
    #endif
    float nDotsl = dot(n, sl);
    float shadow = getShadowAmount(i, worldPos, nDotsl, viewDepth);

    vec3 radiance = colorAndLuminance.rgb * colorAndLuminance.w;

    light += shadow * radiance * skyAmbient;
  }
}


void evaluateDiscreteLighting(inout vec3 light, float blockAmbient) {
  light += pow(blockAmbient, 8.0) * vec3(1.0, 0.8, 0.7) * 60.0;
}

void evaluateIndirectLighting(
  inout vec3 light,
  vec3 skyZenith,
  vec3 skyHorizon,
  float skyAmbient
) {
  float skyShade = skyAmbient;
  light += skyShade * skyShade * skyZenith;
}


#endif
