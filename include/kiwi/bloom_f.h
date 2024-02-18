#ifndef K_BLOOM_H
#define K_BLOOM_H

#include "uniforms/bloom.h"
#include "samplers/bloom.h"

// #if BLOOM_HIGH_PASS
vec4 highPass(vec4 col, float threshold) {
  float lum = dot(col.rgb, vec3(0.2126, 0.7152, 0.0722));
  float slope = 10.0;
  col.rgb /= 1.0 + exp(slope*(threshold - lum) - 1.5);
  return vec4(col.rgb, lum);
}

vec4 highPassDfDownsample(sampler2D srcImg, sampler2D depthImg, vec2 uv, vec2 offset, float brightnessThreshold) {
  vec4 col = 0.6 * highPass(texture2D(srcImg, uv), brightnessThreshold);
  col += 0.1 * highPass(texture2D(srcImg, uv + vec2(offset.x, offset.y)), brightnessThreshold);
  col += 0.1 * highPass(texture2D(srcImg, uv + vec2(-offset.x, offset.y)), brightnessThreshold);
  col += 0.1 * highPass(texture2D(srcImg, uv + vec2(offset.x, -offset.y)), brightnessThreshold);
  col += 0.1 * highPass(texture2D(srcImg, uv + vec2(-offset.x, -offset.y)), brightnessThreshold);
  // if (bool(BloomParams2.z)) {
  //   float minRange = BloomParams2.x;
  //   float maxRange = BloomParams2.y;
  //   float depth = texture2D(depthImg, uv).r;
  //   depth = clamp(((depth * maxRange) - minRange) / (maxRange - minRange), BloomParams1.w, 1.0);
  //   col *= pow(depth, BloomParams1.z);
  // }
  return col;
}
// #elif DFDOWN_SAMPLE
vec4 dualFilterDownsample(sampler2D srcImg, vec2 uv, vec2 offset) {
  vec4 col = 0.6 * texture2D(srcImg, uv);
  col += 0.1 * texture2D(srcImg, uv + vec2(offset.x, offset.y));
  col += 0.1 * texture2D(srcImg, uv + vec2(-offset.x, offset.y));
  col += 0.1 * texture2D(srcImg, uv + vec2(offset.x, -offset.y));
  col += 0.1 * texture2D(srcImg, uv + vec2(-offset.x, -offset.y));
  return col;
}
// #elif DFDOWN_SAMPLE_WITH_DEPTH_EROSION
vec4 dualFilterDownsampleWithDepthErosion(sampler2D srcImg, vec2 uv, vec2 offset) {
  vec4 a = texture2D(srcImg, uv);
  vec4 b = texture2D(srcImg, uv + vec2(offset.x, offset.y));
  vec4 c = texture2D(srcImg, uv + vec2(-offset.x, offset.y));
  vec4 d = texture2D(srcImg, uv + vec2(offset.x, -offset.y));
  vec4 e = texture2D(srcImg, uv + vec2(-offset.x, -offset.y));
  
  vec4 col;
  col.a = max(a.a, max(b.a, max(c.a, max(d.a, e.a))));
  col.rgb = 0.6 * a.rgb;
  col.rgb += 0.1 * b.rgb;
  col.rgb += 0.1 * c.rgb;
  col.rgb += 0.1 * d.rgb;
  col.rgb += 0.1 * e.rgb;
  return col;
}
// #elif defined(DFUP_SAMPLE) || defined(BLOOM_BLEND)
vec4 dualFilterUpsample(sampler2D srcImg, vec2 uv, vec2 offset) {
  vec4 col = 0.104 * texture2D(srcImg, uv);
  col += 0.15 * texture2D(srcImg, uv + vec2(0.5 * offset.x, 0.5 * offset.y));
  col += 0.15 * texture2D(srcImg, uv + vec2(-0.5 * offset.x, 0.5 * offset.y));
  col += 0.15 * texture2D(srcImg, uv + vec2(0.5 * offset.x, -0.5 * offset.y));
  col += 0.15 * texture2D(srcImg, uv + vec2(-0.5 * offset.x, -0.5 * offset.y));
  col += 0.074 * texture2D(srcImg, uv + vec2(offset.x, offset.y));
  col += 0.074 * texture2D(srcImg, uv + vec2(-offset.x, offset.y));
  col += 0.074 * texture2D(srcImg, uv + vec2(offset.x, -offset.y));
  col += 0.074 * texture2D(srcImg, uv + vec2(-offset.x, -offset.y));
  return col;
}
// #endif

vec4 kBloomPasses(vec2 uv) {
  vec2 uvOffset = abs(vec2(dFdx(uv.x),dFdy(uv.y)));
#if BLOOM_HIGH_PASS
  return highPassDfDownsample(s_HDRi, s_DepthTexture, uv, 1.5 * uvOffset, BloomParams1.y);
#elif DFDOWN_SAMPLE
  return dualFilterDownsample(s_BlurPyramidTexture, uv, 1.5 * uvOffset);
#elif DFDOWN_SAMPLE_WITH_DEPTH_EROSION
  return dualFilterDownsampleWithDepthErosion(s_BlurPyramidTexture, uv, 1.5 * uvOffset);
#elif DFUP_SAMPLE
  return dualFilterUpsample(s_BlurPyramidTexture, uv, 4.0 * uvOffset);
#elif BLOOM_BLEND
  vec3 color = texture2D(s_HDRi, uv).rgb;
  vec4 bloom = dualFilterUpsample(s_BlurPyramidTexture, uv, 4.0 * uvOffset);
  color += 2.0 * BloomParams1.x * bloom.rgb;
  return vec4(color, 1.0);
#endif
}

#endif
