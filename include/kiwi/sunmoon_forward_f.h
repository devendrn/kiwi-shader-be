#ifndef SUNMOON_FORWARD_H
#define SUNMOON_FORWARD_H

uniform vec4 SunMoonColor;

SAMPLER2D(s_SunMoonTexture, 0);

vec3 kSunMoon(vec2 texcoord) {
  vec4 spriteColor = texture2D(s_SunMoonTexture, texcoord);
  vec3 sunMoonColor = SunMoonColor.rgb * spriteColor.rgb * SunMoonColor.a;
  return sunMoonColor;
}

#endif
