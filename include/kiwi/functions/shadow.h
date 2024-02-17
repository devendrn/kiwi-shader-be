#ifndef K_SHADOW_H
#define K_SHADOW_H

#include "../uniforms/shadow.h"
#include "../uniforms/lighting.h"

#include "../samplers/shadow.h"
#include "../samplers/lighting.h"

/* mostly vanilla deferred code */
struct DirectionalLightParams {
  mat4 shadowProj[4];
  int cascadeCount;
  int isSun;
  int index;
};

int getShadowCascade(int lightIndex, vec3 worldPos, out vec4 projPos) {
  #ifndef DISABLE_DEFERRED
  LightSourceWorldInfo light = s_DirectionalLightSources[lightIndex];
  #else
  LightSourceWorldInfo light; // fallback debug
  #endif

  for (int c=0; c<4; c++) {
    mat4 proj;
    if (c == 0) {
      proj = light.shadowProj0;
    } else if (c == 1) {
      proj = light.shadowProj1;
    } else if (c == 2) {
      proj = light.shadowProj2;
    } else {
      proj = light.shadowProj3;
    }
    projPos = proj * vec4(worldPos, 1.0);
    projPos /= projPos.w;

    vec3 posDiff = clamp(projPos.xyz, vec3(-1.0, -1.0, -1.0), vec3(1.0, 1.0, 1.0)) - projPos.xyz;
    if (length(posDiff) == 0.0) {
      return c;
    }
  }
  return -1;
}

float getFilteredShadow(int cascadeIndex, float projZ, int cascade, vec2 uv) {
  const int maxFilterWidth = 9;
  int filterWidth = clamp(int(ShadowPCFWidth[cascade] * 1.0 + 0.5), 1, maxFilterWidth);
  int filterOffset = filterWidth / 2;

  vec2 baseUv = uv * CascadeShadowResolutions[cascade];
  projZ = projZ * 0.5 + 0.5;
  baseUv.y += 1.0 - CascadeShadowResolutions[cascade];

  float amt = 0.0;
  for (int iy = 0; iy < filterWidth; ++iy) {
    for (int ix = 0; ix < filterWidth; ++ix) {
      float y = float(iy - filterOffset) + 0.5;
      float x = float(ix - filterOffset) + 0.5;
      vec2 offset = vec2(x, y) * ShadowParams.x;
      if (cascadeIndex >= 0) {
        #ifndef DISABLE_DEFERRED
        // s_ShadowCascades not supported in fallback
        amt += shadow2DArray(
          s_ShadowCascades,
          vec4(
            baseUv + (offset * CascadeShadowResolutions[cascade]),
            (float(cascadeIndex) * DirectionalLightToggleAndCountAndMaxDistanceAndMaxCascadesPerLight.w) + float(cascade),
            projZ
          )
        );
        #endif
      } else {
        amt += 1.0;
      }
    }
  }
  return amt / float(filterWidth * filterWidth);
}

float getPlayerShadow(vec3 worldPos, float biasSlope) {
  const int playerCascade = 0;

  vec4 playerProjPos = PlayerShadowProj * vec4(worldPos, 1.0);
  playerProjPos /= playerProjPos.w;

  float bias = ShadowBias[playerCascade] + ShadowSlopeBias[playerCascade] * biasSlope;
  playerProjPos.z -= bias / playerProjPos.w;
  playerProjPos.z = min(playerProjPos.z, 1.0);
  vec2 playerUv = (playerProjPos.xy * 0.5 + 0.5) * FirstPersonPlayerShadowsEnabledAndResolutionAndFilterWidth.y;
  const int maxFilterWidth = 9;
  // int filterWidth = clamp(2, 1, MaxFilterWidth);
  int filterWidth = 2;
  int filterOffset = filterWidth / 2;
  float amt = 0.0;
  playerProjPos.z = playerProjPos.z * 0.5 + 0.5;
  playerUv.y += 1.0 - FirstPersonPlayerShadowsEnabledAndResolutionAndFilterWidth.y;
  for (int iy = 0; iy < filterWidth; ++iy) {
    for (int ix = 0; ix < filterWidth; ++ix) {
      float y = float(iy - filterOffset) + 0.5;
      float x = float(ix - filterOffset) + 0.5;
      vec2 offset = vec2(x, y) * FirstPersonPlayerShadowsEnabledAndResolutionAndFilterWidth.z;
      vec2 newUv = playerUv + (offset * FirstPersonPlayerShadowsEnabledAndResolutionAndFilterWidth.y);
      if (newUv.x >= 0.0 && newUv.x < 1.0 && newUv.y >= 0.0 && newUv.y < 1.0) {
        amt += shadow2D(s_PlayerShadowMap, vec3(newUv, playerProjPos.z));
      } else {
        amt += 1.0;
      }
    }
  }
  return amt / float(filterWidth * filterWidth);
}

float getShadowAmount(int lightIndex, vec3 worldPos, float NdL, float viewDepth) {
  float amt = 1.0;
  vec4 projPos;

  int cascade = getShadowCascade(lightIndex, worldPos, projPos);
  if (cascade != -1) {
    //float biasSlope = clamp(tan(acos(max(NdL, 0.0))), 0.0, 1.0);
    float biasSlope = 1.0;
    float bias = ShadowBias[cascade] + ShadowSlopeBias[cascade] * biasSlope;
    projPos.z -= bias / projPos.w;
    vec2 uv = projPos.xy * 0.5 + 0.5;

    #ifndef DISABLE_DEFERRED
    // s_DirectionalLightSources not supported in fallback
    amt = getFilteredShadow(
      s_DirectionalLightSources[lightIndex].shadowCascadeNumber,
      projPos.z,
      cascade,
      uv
    );
    #endif

    if (int(FirstPersonPlayerShadowsEnabledAndResolutionAndFilterWidth.x) > 0) {
      amt = min(amt, getPlayerShadow(worldPos, biasSlope));
    }

    float d = 1.0 - max(-NdL, 0.0);
    //amt = min(1.0 - d*d*d*d, amt);

    float shadowFade = smoothstep(max(0.0, ShadowParams.y - 8.0), ShadowParams.y, -viewDepth);
    amt += (1.0 - amt)*shadowFade;
  }
  return amt;
}

#endif
