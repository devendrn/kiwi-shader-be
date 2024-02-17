$input v_ndcPosition, v_texcoord0

#include <bgfx_shader.sh>

#ifndef FALLBACK
#include <kiwi/sunmoon_forward_f.h>
#endif

void main() {
#if FALLBACK
  gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
#else
  gl_FragColor = vec4(kSunMoon(v_texcoord0), 1.0);
#endif
}
