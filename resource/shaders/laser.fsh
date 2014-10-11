#ifdef GL_ES
//precision mediump float;
precision highp float;//only to avoid u_time overflow
#endif

//this shader is ref to ccShader_PositionTextureColor_frag.h
//v_fragmentColor is corresponding to sprite's opacity

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
uniform sampler2D CC_Texture0;
uniform sampler2D u_maskTex;
uniform sampler2D u_maskTex2;
uniform sampler2D u_noiseTex;
uniform sampler2D u_noiseTex2;
uniform vec2 u_noiseSize;
uniform vec2 u_spriteSize;
uniform vec2 u_LUPos;
uniform float u_highlight;
uniform float u_time;
uniform float u_PorN;
uniform vec2 u_scale;
uniform float u_kLightColor;
uniform float u_kGlowColor;
uniform float u_noiseScale;

void main()
{
    vec2 texCoord_noise;
    
    texCoord_noise.x = (v_texCoord.x*u_spriteSize.x+u_LUPos.x)/u_noiseSize.x;
    texCoord_noise.y = (v_texCoord.y*u_spriteSize.y+u_noiseSize.y-u_LUPos.y)/u_noiseSize.y;

    vec2 texCoord_mask;
    texCoord_mask.x=(v_texCoord.x+u_time*0.05)*u_scale.x;
    texCoord_mask.y=v_texCoord.y;
    
    vec2 texCoord_mask2;
    texCoord_mask2.x=(v_texCoord.x+u_time*0.1)*u_scale.x;
    texCoord_mask2.y=v_texCoord.y;
    
    vec4 maskTexColor=texture2D(u_maskTex, texCoord_mask);
    vec4 maskTexColor2=texture2D(u_maskTex2, texCoord_mask2);

    vec4 lightTexColor= texture2D(CC_Texture0, v_texCoord+vec2(u_time*10.0,0.0));
    
    vec2 texCoord_noise1=texCoord_noise;
    texCoord_noise1*=u_noiseScale;
    texCoord_noise1+=vec2(0.0,u_time*0.2*u_PorN);
    
    vec2 texCoord_noise2=texCoord_noise;
    texCoord_noise2*=u_noiseScale;
    texCoord_noise2+=vec2(0.0, u_time*0.4*u_PorN);
    
    vec4 noiseTexColor1=texture2D(u_noiseTex, texCoord_noise1);
    vec4 noiseTexColor2=texture2D(u_noiseTex2, texCoord_noise2);
 
    
    float deep;
    if(v_texCoord.y<0.5+0.001){
        deep=(u_PorN==-1.0)?0.0:v_texCoord.y*v_texCoord.y*1.0/0.25;
    }
    if(v_texCoord.y>=0.5-0.001){
        deep=(u_PorN==-1.0)?(1.0-v_texCoord.y)*(1.0-v_texCoord.y)*1.0/0.25:0.0;
    
    }
    
   
    float deep2;
    vec2 curPoint=v_texCoord*u_spriteSize;
    float curveX=u_spriteSize.x-0.05*(curPoint.y-0.5*u_spriteSize.y)*(curPoint.y-0.5*u_spriteSize.y)-10.0;//-30.0;
    if(curPoint.x<curveX){
        deep2=1.0;
    }else{//curPoint.x>=curveX
        curPoint.x-curveX;
        deep2=1.0-1.0/(u_spriteSize.x-curveX)*(curPoint.x-curveX);
    }
    

    
    
    vec4 glowColor =u_kGlowColor*(noiseTexColor1+noiseTexColor2)*(maskTexColor+maskTexColor2)/2.0*u_highlight;
    vec4 lightColor =lightTexColor*u_kLightColor;
    vec4 finalColor = glowColor*vec4(1.0,0.5,0.0,1.0)+lightColor;
    
    float Gray = finalColor.r*0.299 + finalColor.g*0.587 + finalColor.b*0.114;
    
    finalColor.a=Gray*2.0;
    finalColor*=deep*deep2;
    gl_FragColor=finalColor;
    
    
}
