#ifndef K_DEFERRED_H
#define K_DEFERRED_H

#include "uniforms/tonemap.h"
#include "samplers/tonemap.h"

vec3 colorGamma(vec3 clr) {
  return pow(max(clr, vec3_splat(0.0)), vec3_splat(1.0/2.2));
}

float luminance(vec3 clr) {
  return dot(clr, vec3(0.2126, 0.7152, 0.0722));
}

vec3 tonemapReinhard(vec3 x) {
  return x/(1.0 + x);
}

vec3 tonemapReinhardLuminance(vec3 x, float W) {
  float lOld = luminance(x);
  float lNew = (lOld * (1.0 + (lOld / W))) / (1.0 + lOld);
  return x * (lNew / lOld);
}

vec3 tonemapReinhardJodie(vec3 x) {
  float l = luminance(x);
  vec3 tc = x / (1.0 + x);
  return mix(x / (1.0 + l), tc, tc);
}

vec3 uncharted2Tonemap(vec3 x) {
  float a = 0.15;
  float b = 0.50;
  float c = 0.10;
  float d = 0.20;
  float e = 0.02;
  float f = 0.30;
  return ((x * (a * x + c * b) + d * e) / (x * (a * x + b) + d * f)) - e / f;
}

vec3 tonemapUncharted2(vec3 x, float W) {
  const float exposureBias = 2.0;
  vec3 curr = uncharted2Tonemap(exposureBias * x);
  vec3 whiteScale = 1.0 / uncharted2Tonemap(vec3_splat(W));
  return curr * whiteScale;
}

vec3 applyTonemap(vec3 finalHDRi, float brightness, float contrast, float saturation, float exposure, float whitePoint) {
  //float whitePointSquared = whitePoint * whitePoint;
  finalHDRi *= exposure;
  finalHDRi = tonemapReinhard(finalHDRi);
  finalHDRi = colorGamma(finalHDRi);
  finalHDRi = mix(vec3_splat(luminance(finalHDRi)), finalHDRi, max(0.0, saturation));
  finalHDRi = clamp(finalHDRi, 0.0, 1.0);
  return finalHDRi;
}

vec3 kColorProcessing(vec2 texcoord) {
  vec3 finalHDRi = texture2D(s_ColorTexture, texcoord).rgb;
  vec3 color = finalHDRi;

  if (TonemapParams0.y <= 0.5) {
    color = colorGamma(finalHDRi);
  } else {
    float avgLuminance = clamp(texture2D(s_AverageLuminance, vec2(0.5, 0.5)).r, 0.5, 100000.0);
    float maxLuminance = clamp(texture2D(s_MaxLuminance, vec2(0.5, 0.5)).r, 0.5, 100000.0);
    float exposure = 0.1/avgLuminance;
    float whitePoint = exposure*maxLuminance;

    color = applyTonemap(
      finalHDRi,
      TonemapCorrection.x,
      TonemapCorrection.y,
      TonemapCorrection.z,
      exposure,
      whitePoint
    );

  }

  return color;
}

#endif
