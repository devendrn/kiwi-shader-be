$input v_texcoord0

#include <bgfx_shader.sh>
#include <kiwi/bloom_f.h>

void main() {
  gl_FragColor = kBloomPasses(v_texcoord0);
}
