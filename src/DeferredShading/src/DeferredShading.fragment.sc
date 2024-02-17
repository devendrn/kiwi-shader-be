$input v_projPosition, v_texcoord0

#include <bgfx_shader.sh>
#include <bgfx_compute.sh>

// For syntax checking through fallback.
// Should be commented out for release.
// TOOD - pass this through MBT instead
#define DEBUG

// DISABLE_DEFERRED is also used to disable unsuported features in fallback
#if defined(FALLBACK) || !(BGFX_SHADER_LANGUAGE_GLSL >= 310 || BGFX_SHADER_LANGUAGE_HLSL >= 500 || BGFX_SHADER_LANGUAGE_PSSL || BGFX_SHADER_LANGUAGE_SPIRV || BGFX_SHADER_LANGUAGE_METAL)
  #define DISABLE_DEFERRED 
#endif

#if !defined(DISABLE_DEFERRED) || defined(DEBUG)
#include <kiwi/deferred_f.h>
#endif

void main() {
#if !defined(DISABLE_DEFERRED) || defined(DEBUG)
  gl_FragColor = kDeferredLighting(v_texcoord0, v_projPosition);
#else
  gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
#endif
}
