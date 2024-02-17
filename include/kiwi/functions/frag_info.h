#ifndef K_FRAG_INFO_H
#define K_FRAG_INFO_H

struct FragmentInfo {
  vec2 uv;
  vec3 worldPos;
  vec3 viewPos;
  vec3 worldNrm;
  vec3 viewNrm;
  vec3 worldDir;
  vec3 ndcPos;
  vec3 albedo;
  float metalness;
  float roughness;
  float emissive;
  float skyAmbient;
  float blockAmbient;
};

vec3 colorDegamma(vec3 col) {
  return pow(max(col, vec3_splat(0.0)), vec3_splat(2.2));
}

vec2 octWrap(vec2 v) {
  return (1.0 - abs(v.yx)) * ((2.0 * step(0.0, v)) - 1.0);
}

vec3 octToNdirSnrm(vec2 p) {
  vec3 n = vec3(p.xy, 1.0 - abs(p.x) - abs(p.y));
  n.xy = (n.z < 0.0) ? octWrap(n.xy) : n.xy;
  return normalize(n);
}

vec4 projToView(vec4 p, mat4 inverseProj) {
#if BGFX_SHADER_LANGUAGE_GLSL
  p = vec4(
    p.x * inverseProj[0][0],
    p.y * inverseProj[1][1],
    p.w * inverseProj[3][2],
    p.z * inverseProj[2][3] + p.w * inverseProj[3][3]
  );
#else
  p = vec4(
    p.x * inverseProj[0][0],
    p.y * inverseProj[1][1],
    p.w * inverseProj[2][3],
    p.z * inverseProj[3][2] + p.w * inverseProj[3][3]
  );
#endif
  return p / p.w;
}

FragmentInfo getFragmentInfo(
  vec3 projPos,
  vec2 texcoord0,
  sampler2D s_ColorMetalness,
  sampler2D s_Normal,
  sampler2D s_EmissiveAmbientLinearRoughness,
  sampler2D s_SceneDepth
) {
  vec2 uv = texcoord0;
  float z = texture2D(s_SceneDepth, uv).r;
  vec4 cm = texture2D(s_ColorMetalness, uv);
  vec4 ear = texture2D(s_EmissiveAmbientLinearRoughness, uv);
  vec2 n = texture2D(s_Normal, uv).xy;

#if BGFX_SHADER_LANGUAGE_GLSL
  z = z * 2.0 - 1.0;
#endif

  vec4 viewPos = projToView(vec4(projPos.xy, z, 1.0), u_invProj);
  vec4 worldPos = mul(u_invView, vec4(viewPos.xyz, 1.0));
  vec3 worldNrm = normalize(octToNdirSnrm(n.xy));
  vec3 viewNrm = normalize(mul(u_view, vec4(worldNrm, 0.0)).xyz);

  FragmentInfo result;
  result.uv = uv;
  result.worldPos = worldPos.xyz;
  result.viewPos = viewPos.xyz;
  result.ndcPos = vec3(projPos.xy, z);
  result.worldNrm = worldNrm;
  result.viewNrm = viewNrm;
  result.worldDir = normalize(worldPos.xyz);
  result.albedo = colorDegamma(cm.rgb);
  result.metalness = cm.a;
  result.emissive = ear.r;
  result.blockAmbient = ear.g;
  result.skyAmbient = ear.b;
  result.roughness = ear.a;
  return result;
}

#endif
