

#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;

//uniform sampler2D CC_Texture0;//m_biggerRT's texture

uniform float u_filterR;
uniform float u_ds;
uniform float u_s_step;
void main() {
    float glowAlpha;
    {
        float s_cur=v_texCoord.x;
        float t_cur=v_texCoord.y;
        float s_halfWidth=u_filterR*u_ds;
        float s_begin=s_cur-s_halfWidth;
        float s_end=s_cur+s_halfWidth;
        float alpha=0.0;
        int count=0;
        //i do not know why, if use s<=s_end, will stuck in cocos2dx 3.x, investigate it later.
        ////for(float s=s_begin;s<=s_end;s+=u_s_step){
        for(float s=s_begin;s<s_end;s+=u_s_step){
            alpha+=texture2D(CC_Texture0, vec2(s,t_cur)).a;
            count+=1;
        }
        alpha/=float(count);
        glowAlpha=alpha;
    }
    
    vec4 finalColor=vec4(vec3(1.0),glowAlpha);
    
    gl_FragColor=finalColor;
    
}

