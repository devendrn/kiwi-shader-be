$input v_texcoord0

#include <bgfx_shader.sh>
#include <kiwi/tonemap_f.h>

void main() {
  gl_FragColor = vec4(kColorProcessing(v_texcoord0), 1.0);
}
