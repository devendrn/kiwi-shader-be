#ifndef K_SKY_H
#define K_SKY_H

#include "../uniforms/sky.h"
#include "../uniforms/fog.h"

void evaluateSkyRadiance(
  inout vec3 skyZenith,
  inout vec3 skyHorizon,
  inout vec3 skyTotal,
  vec3 worldDir
) {

  float intensity = (SunColor.a + MoonColor.a) * 0.05;
  skyZenith = SkyZenithColor.rgb * intensity * 0.4;
  skyHorizon = SkyHorizonColor.rgb * intensity;

  float gradient = pow(1.0 - smoothstep(0.0, 0.9, worldDir.y), 6.0);
  skyTotal = mix(skyZenith, skyHorizon, gradient);
}

// move these to fog.h ? 
float calculateFogFade(float cameraDepth, float maxDistance, float fogStart, float fogEnd, float fogAlpha) {
  float distance = (cameraDepth / maxDistance) + fogAlpha;
  float mist = 0.0 * (distance * distance) / fogEnd;
  float vanillaFade = max((distance - fogStart) / (fogEnd - fogStart), 0.0);
  return smoothstep(0.0, 1.0, mist + vanillaFade);
}

void evaluateFogFade(inout vec3 color, vec3 sky, float viewDistance) {
  float fogFade = calculateFogFade(
    viewDistance, FogAndDistanceControl.z, FogAndDistanceControl.x,
    FogAndDistanceControl.y, RenderChunkFogAlpha.x
  );

  color = mix(color, sky, fogFade);
}

#endif
