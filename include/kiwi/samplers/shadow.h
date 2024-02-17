#ifndef SHADOW_SAMPLERS_H
#define SHADOW_SAMPLERS_H

// DeferredShading.fragment.sc
// RenderChunkForwardPBR.fragment.sc

SAMPLER2DSHADOW(s_PlayerShadowMap, 7);
#ifndef DISABLE_DEFERRED
// not supported in fallback
SAMPLER2DARRAYSHADOW(s_ShadowCascades, 12);
#endif

#endif
