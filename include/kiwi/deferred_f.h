#ifndef K_DEFERRED_H
#define K_DEFERRED_H

#include "samplers/frag_info.h"

#include "functions/frag_info.h"
#include "functions/lighting.h"
#include "functions/sky.h"

vec4 kDeferredLighting(vec2 v_texcoord0, vec3 v_projPosition) {

  FragmentInfo fragInfo = getFragmentInfo(
    v_projPosition,
    v_texcoord0,
    s_ColorMetalness,
    s_Normal,
    s_EmissiveAmbientLinearRoughness,
    s_SceneDepth
  );

  vec3 n = fragInfo.viewNrm;
  float viewDepth = fragInfo.viewPos.z;
  float viewDistance = length(fragInfo.worldPos);

  vec3 light = vec3_splat(0.0);
  vec3 skyHorizonRadiance = vec3_splat(0.0);
  vec3 skyZenithRadiance = vec3_splat(0.0);
  vec3 skyTotalRadiance = vec3_splat(0.0);

  evaluateSkyRadiance(skyZenithRadiance, skyHorizonRadiance, skyTotalRadiance, fragInfo.worldDir);
  evaluateIndirectLighting(light, skyZenithRadiance, skyHorizonRadiance, fragInfo.skyAmbient);
  evaluateDirectionalLighting(light, viewDepth, fragInfo.worldPos, n, fragInfo.skyAmbient);
  evaluateDiscreteLighting(light, fragInfo.blockAmbient);

  vec3 color = fragInfo.albedo * light;

  evaluateFogFade(color, skyTotalRadiance, viewDistance);

  return vec4(color, 1.0);
}

#endif
