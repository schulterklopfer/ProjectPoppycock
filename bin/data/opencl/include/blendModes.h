/* based on https://github.com/jamieowen/glsl-blend */

static float blendAdd(float base, float blend) {
	return min(base+blend,1.0f);
}

static float blendSubtract(float base, float blend) {
	return max(base+blend-1.0f,0.0f);
}

static float blendLighten(float base, float blend) {
	return max(blend,base);
}

static float blendDarken(float base, float blend) {
	return min(blend,base);
}

static float blendLinearBurn(float base, float blend) {
    // Note : Same implementation as BlendSubtractf
    return max(base+blend-1.0f,0.0f);
}

static float blendLinearDodge(float base, float blend) {
    // Note : Same implementation as BlendAddf
    return min(base+blend,1.0f);
}

static float blendLinearLight(float base, float blend) {
	return blend<0.5f?blendLinearBurn(base,(2.0f*blend)):blendLinearDodge(base,(2.0f*(blend-0.5f)));
}


static float blendScreen(float base, float blend) {
	return 1.0f-((1.0f-base)*(1.0f-blend));
}


static float blendOverlay(float base, float blend) {
	return base<0.5f?(2.0f*base*blend):(1.0f-2.0f*(1.0f-base)*(1.0f-blend));
}


static float blendSoftLight(float base, float blend) {
	return (blend<0.5f)?(2.0f*base*blend+base*base*(1.0f-2.0f*blend)):(sqrt(base)*(2.0f*blend-1.0f)+2.0f*base*(1.0f-blend));
}


static float blendColorDodge(float base, float blend) {
	return (blend==1.0f)?blend:min(base/(1.0f-blend),1.0f);
}


static float blendColorBurn(float base, float blend) {
	return (blend==0.0f)?blend:max((1.0f-((1.0f-base)/blend)),0.0f);
}


static float blendVividLight(float base, float blend) {
	return (blend<0.5f)?blendColorBurn(base,(2.0f*blend)):blendColorDodge(base,(2.0f*(blend-0.5f)));
}


static float blendPinLight(float base, float blend) {
	return (blend<0.5f)?blendDarken(base,(2.0f*blend)):blendLighten(base,(2.0f*(blend-0.5f)));
}


static float blendHardMix(float base, float blend) {
	return (blendVividLight(base,blend)<0.5f)?0.0f:1.0f;
}


static float blendReflect(float base, float blend) {
	return (blend==1.0f)?blend:min(base*base/(1.0f-blend),1.0f);
}

static float4 blendNormal4(float4 base, float4 blend) {
	return blend;
}


static float4 blendLighten4(float4 base, float4 blend) {
	return (float4)(blendLighten(base.r,blend.r),blendLighten(base.g,blend.g),blendLighten(base.b,blend.b),1.0f);
}


static float4 blendDarken4(float4 base, float4 blend) {
	return (float4)(blendDarken(base.r,blend.r),blendDarken(base.g,blend.g),blendDarken(base.b,blend.b),1.0f);
}


static float4 blendMultiply4(float4 base, float4 blend) {
	return base*blend;
}


static float4 blendAverage4(float4 base, float4 blend) {
	return (base+blend)/2.0f;
}


static float4 blendAdd4(float4 base, float4 blend) {
	return min(base+blend,float4(1.0f));
}


static float4 blendSubtract4(float4 base, float4 blend) {
	return max(base+blend-float4(1.0f),float4(0.0f));
}


static float4 blendDifference4(float4 base, float4 blend) {
	return fabs(base-blend);
}


static float4 blendNegation4(float4 base, float4 blend) {
	return (float4)(1.0f)-fabs(float4(1.0f)-base-blend);
}


static float4 blendExclusion4(float4 base, float4 blend) {
	return base+blend-2.0f*base*blend;
}


static float4 blendScreen4(float4 base, float4 blend) {
	return (float4)(blendScreen(base.r,blend.r),blendScreen(base.g,blend.g),blendScreen(base.b,blend.b),1.0f);
}


static float4 blendOverlay4(float4 base, float4 blend) {
	return (float4)(blendOverlay(base.r,blend.r),blendOverlay(base.g,blend.g),blendOverlay(base.b,blend.b),1.0f);
}


static float4 blendSoftLight4(float4 base, float4 blend) {
	return (float4)(blendSoftLight(base.r,blend.r),blendSoftLight(base.g,blend.g),blendSoftLight(base.b,blend.b),1.0f);
}


static float4 blendHardLight4(float4 base, float4 blend) {
	return blendOverlay4(blend,base);
}


static float4 blendColorDodge4(float4 base, float4 blend) {
	return (float4)(blendColorDodge(base.r,blend.r),blendColorDodge(base.g,blend.g),blendColorDodge(base.b,blend.b),1.0f);
}


static float4 blendColorBurn4(float4 base, float4 blend) {
	return (float4)(blendColorBurn(base.r,blend.r),blendColorBurn(base.g,blend.g),blendColorBurn(base.b,blend.b),1.0f);
}


static float4 blendLinearLight4(float4 base, float4 blend) {
	return (float4)(blendLinearLight(base.r,blend.r),blendLinearLight(base.g,blend.g),blendLinearLight(base.b,blend.b),1.0f);
}


static float4 blendVividLight4(float4 base, float4 blend) {
	return (float4)(blendVividLight(base.r,blend.r),blendVividLight(base.g,blend.g),blendVividLight(base.b,blend.b),1.0f);
}


static float4 blendPinLight4(float4 base, float4 blend) {
	return (float4)(blendPinLight(base.r,blend.r),blendPinLight(base.g,blend.g),blendPinLight(base.b,blend.b),1.0f);
}


static float4 blendHardMix4(float4 base, float4 blend) {
	return (float4)(blendHardMix(base.r,blend.r),blendHardMix(base.g,blend.g),blendHardMix(base.b,blend.b),1.0f);
}


static float4 blendReflect4(float4 base, float4 blend) {
	return (float4)(blendReflect(base.r,blend.r),blendReflect(base.g,blend.g),blendReflect(base.b,blend.b),1.0f);
}


static float4 blendGlow4(float4 base, float4 blend) {
	return blendReflect4(blend,base);
}


static float4 blendPhoenix4(float4 base, float4 blend) {
	return min(base,blend)-max(base,blend)+float4(1.0f);
}

static float4 blendLinearDodge4(float4 base, float4 blend) {
	// Note : Same implementation as BlendAdd
	return min(base+blend,float4(1.0f));
}


static float4 blendLinearBurn4(float4 base, float4 blend) {
	// Note : Same implementation as BlendSubtract
	return max(base+blend-float4(1.0f),float4(0.0f));
}

static float4 blendMaskR4( float4 base, float4 blend ) {
    return (float4)( blend.r * base.r, blend.r * base.g, blend.r * base.b, 1.0f);
}

static float4 blendMaskG4( float4 base, float4 blend ) {
    return (float4)( blend.g * base.r, blend.g * base.g, blend.g * base.b, 1.0f);
}

static float4 blendMaskB4( float4 base, float4 blend ) {
    return (float4)( blend.b * base.r, blend.b * base.g, blend.b * base.b, 1.0f);
}

static float4 blendNormal4_with_opacity(float4 base, float4 blend, float opacity) {
	return (blendNormal4(base, blend) * opacity + base * (1.0f - opacity));
}


static float4 blendLighten4_with_opacity(float4 base, float4 blend, float opacity) {
	return (blendLighten4(base, blend) * opacity + base * (1.0f - opacity));
}


static float4 blendDarken4_with_opacity(float4 base, float4 blend, float opacity) {
	return (blendDarken4(base, blend) * opacity + base * (1.0f - opacity));
}


static float4 blendMultiply4_with_opacity(float4 base, float4 blend, float opacity) {
	return (blendMultiply4(base, blend) * opacity + base * (1.0f - opacity));
}


static float4 blendAverage4_with_opacity(float4 base, float4 blend, float opacity) {
	return (blendAverage4(base, blend) * opacity + base * (1.0f - opacity));
}


static float4 blendAdd4_with_opacity(float4 base, float4 blend, float opacity) {
	return (blendAdd4(base, blend) * opacity + base * (1.0f - opacity));
}


static float4 blendSubtract4_with_opacity(float4 base, float4 blend, float opacity) {
	return (blendSubtract4(base, blend) * opacity + base * (1.0f - opacity));
}


static float4 blendDifference4_with_opacity(float4 base, float4 blend, float opacity) {
	return (blendDifference4(base, blend) * opacity + base * (1.0f - opacity));
}


static float4 blendNegation4_with_opacity(float4 base, float4 blend, float opacity) {
	return (blendNegation4(base, blend) * opacity + base * (1.0f - opacity));
}


static float4 blendExclusion4_with_opacity(float4 base, float4 blend, float opacity) {
	return (blendExclusion4(base, blend) * opacity + base * (1.0f - opacity));
}


static float4 blendScreen4_with_opacity(float4 base, float4 blend, float opacity) {
	return (blendScreen4(base, blend) * opacity + base * (1.0f - opacity));
}


static float4 blendOverlay4_with_opacity(float4 base, float4 blend, float opacity) {
	return (blendOverlay4(base, blend) * opacity + base * (1.0f - opacity));
}


static float4 blendSoftLight4_with_opacity(float4 base, float4 blend, float opacity) {
	return (blendSoftLight4(base, blend) * opacity + base * (1.0f - opacity));
}


static float4 blendHardLight4_with_opacity(float4 base, float4 blend, float opacity) {
	return (blendHardLight4(base, blend) * opacity + base * (1.0f - opacity));
}


static float4 blendColorDodge4_with_opacity(float4 base, float4 blend, float opacity) {
	return (blendColorDodge4(base, blend) * opacity + base * (1.0f - opacity));
}


static float4 blendColorBurn4_with_opacity(float4 base, float4 blend, float opacity) {
	return (blendColorBurn4(base, blend) * opacity + base * (1.0f - opacity));
}


static float4 blendLinearLight4_with_opacity(float4 base, float4 blend, float opacity) {
	return (blendLinearLight4(base, blend) * opacity + base * (1.0f - opacity));
}


static float4 blendVividLight4_with_opacity(float4 base, float4 blend, float opacity) {
	return (blendVividLight4(base, blend) * opacity + base * (1.0f - opacity));
}


static float4 blendPinLight4_with_opacity(float4 base, float4 blend, float opacity) {
	return (blendPinLight4(base, blend) * opacity + base * (1.0f - opacity));
}


static float4 blendHardMix4_with_opacity(float4 base, float4 blend, float opacity) {
	return (blendHardMix4(base, blend) * opacity + base * (1.0f - opacity));
}


static float4 blendReflect4_with_opacity(float4 base, float4 blend, float opacity) {
	return (blendReflect4(base, blend) * opacity + base * (1.0f - opacity));
}


static float4 blendGlow4_with_opacity(float4 base, float4 blend, float opacity) {
	return (blendGlow4(base, blend) * opacity + base * (1.0f - opacity));
}


static float4 blendPhoenix4_with_opacity(float4 base, float4 blend, float opacity) {
	return (blendPhoenix4(base, blend) * opacity + base * (1.0f - opacity));
}


static float4 blendLinearDodge4_with_opacity(float4 base, float4 blend, float opacity) {
	return (blendLinearDodge4(base, blend) * opacity + base * (1.0f - opacity));
}


static float4 blendLinearBurn4_with_opacity(float4 base, float4 blend, float opacity) {
	return (blendLinearBurn4(base, blend) * opacity + base * (1.0f - opacity));
}

static float4 blendMaskR4_with_opacity( float4 base, float4 blend, float opacity ) {
    return (blendMaskR4(base, blend) * opacity + base * (1.0f - opacity));
}

static float4 blendMaskG4_with_opacity( float4 base, float4 blend, float opacity ) {
    return (blendMaskG4(base, blend) * opacity + base * (1.0f - opacity));
}

static float4 blendMaskB4_with_opacity( float4 base, float4 blend, float opacity ) {
    return (blendMaskB4(base, blend) * opacity + base * (1.0f - opacity));
}

static float4 blend4( int mode, float4 base, float4 blend ){
	if( mode == 1 ){
		return blendAdd4( base, blend );
	}else
	if( mode == 2 ){
		return blendAverage4( base, blend );
	}else
	if( mode == 3 ){
		return blendColorBurn4( base, blend );
	}else
	if( mode == 4 ){
		return blendColorDodge4( base, blend );
	}else
	if( mode == 5 ){
		return blendDarken4( base, blend );
	}else
	if( mode == 6 ){
		return blendDifference4( base, blend );
	}else
	if( mode == 7 ){
		return blendExclusion4( base, blend );
	}else
	if( mode == 8 ){
		return blendGlow4( base, blend );
	}else
	if( mode == 9 ){
		return blendHardLight4( base, blend );
	}else
	if( mode == 10 ){
		return blendHardMix4( base, blend );
	}else
	if( mode == 11 ){
		return blendLighten4( base, blend );
	}else
	if( mode == 12 ){
		return blendLinearBurn4( base, blend );
	}else
	if( mode == 13 ){
		return blendLinearDodge4( base, blend );
	}else
	if( mode == 14 ){
		return blendLinearLight4( base, blend );
	}else
	if( mode == 15 ){
		return blendMultiply4( base, blend );
	}else
	if( mode == 16 ){
		return blendNegation4( base, blend );
	}else
	if( mode == 17 ){
		return blendNormal4( base, blend );
	}else
	if( mode == 18 ){
		return blendOverlay4( base, blend );
	}else
	if( mode == 19 ){
		return blendPhoenix4( base, blend );
	}else
	if( mode == 20 ){
		return blendPinLight4( base, blend );
	}else
	if( mode == 21 ){
		return blendReflect4( base, blend );
	}else
	if( mode == 22 ){
		return blendScreen4( base, blend );
	}else
	if( mode == 23 ){
		return blendSoftLight4( base, blend );
	}else
	if( mode == 24 ){
		return blendSubtract4( base, blend );
	}else
	if( mode == 25 ){
		return blendVividLight4( base, blend );
    }else
    if( mode == 26 ){
        return blendMaskR4( base, blend );
    }else
    if( mode == 27 ){
        return blendMaskG4( base, blend );
    }else
    if( mode == 28 ){
        return blendMaskB4( base, blend );
    }
}

static float4 blend4_with_opacity( int mode, float4 base, float4 blend, float opacity ){
	if( mode == 1 ){
		return blendAdd4_with_opacity( base, blend, opacity );
	}else
	if( mode == 2 ){
		return blendAverage4_with_opacity( base, blend, opacity );
	}else
	if( mode == 3 ){
		return blendColorBurn4_with_opacity( base, blend, opacity );
	}else
	if( mode == 4 ){
		return blendColorDodge4_with_opacity( base, blend, opacity );
	}else
	if( mode == 5 ){
		return blendDarken4_with_opacity( base, blend, opacity );
	}else
	if( mode == 6 ){
		return blendDifference4_with_opacity( base, blend, opacity );
	}else
	if( mode == 7 ){
		return blendExclusion4_with_opacity( base, blend, opacity );
	}else
	if( mode == 8 ){
		return blendGlow4_with_opacity( base, blend, opacity );
	}else
	if( mode == 9 ){
		return blendHardLight4_with_opacity( base, blend, opacity );
	}else
	if( mode == 10 ){
		return blendHardMix4_with_opacity( base, blend, opacity );
	}else
	if( mode == 11 ){
		return blendLighten4_with_opacity( base, blend, opacity );
	}else
	if( mode == 12 ){
		return blendLinearBurn4_with_opacity( base, blend, opacity );
	}else
	if( mode == 13 ){
		return blendLinearDodge4_with_opacity( base, blend, opacity );
	}else
	if( mode == 14 ){
		return blendLinearLight4_with_opacity( base, blend, opacity );
	}else
	if( mode == 15 ){
		return blendMultiply4_with_opacity( base, blend, opacity );
	}else
	if( mode == 16 ){
		return blendNegation4_with_opacity( base, blend, opacity );
	}else
	if( mode == 17 ){
		return blendNormal4_with_opacity( base, blend, opacity );
	}else
	if( mode == 18 ){
		return blendOverlay4_with_opacity( base, blend, opacity );
	}else
	if( mode == 19 ){
		return blendPhoenix4_with_opacity( base, blend, opacity );
	}else
	if( mode == 20 ){
		return blendPinLight4_with_opacity( base, blend, opacity );
	}else
	if( mode == 21 ){
		return blendReflect4_with_opacity( base, blend, opacity );
	}else
	if( mode == 22 ){
		return blendScreen4_with_opacity( base, blend, opacity );
	}else
	if( mode == 23 ){
		return blendSoftLight4_with_opacity( base, blend, opacity );
	}else
	if( mode == 24 ){
		return blendSubtract4_with_opacity( base, blend, opacity );
	}else
	if( mode == 25 ){
		return blendVividLight4_with_opacity( base, blend, opacity );
    }else
    if( mode == 26 ){
        return blendMaskR4_with_opacity( base, blend, opacity );
    }else
    if( mode == 27 ){
        return blendMaskG4_with_opacity( base, blend, opacity );
    }else
    if( mode == 28 ){
        return blendMaskB4_with_opacity( base, blend, opacity );
    }
}

