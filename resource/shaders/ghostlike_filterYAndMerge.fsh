

#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;

uniform sampler2D CC_Texture0;//m_filterXRT's texture
uniform sampler2D u_refTex;//m_refSprite's texture

uniform float u_filterR;
uniform float u_dt;
uniform float u_t_step;
void main() {
    vec4 refTexColor=texture2D(u_refTex,v_texCoord);;
    float glowAlpha;
    {
        float s_cur=v_texCoord.x;
        float t_cur=v_texCoord.y;
        float t_halfHeight=u_filterR*u_dt;
        float t_begin=t_cur-t_halfHeight;
        float t_end=t_cur+t_halfHeight;
        float alpha=0.0;
        int count=0;
        for(float t=t_begin;t<=t_end;t+=u_t_step){
            alpha+=texture2D(CC_Texture0, vec2(s_cur,t)).a;
            count+=1;
        }
        alpha/=float(count);
        glowAlpha=alpha;
        glowAlpha+=refTexColor.a;//this extra line makes the outline more clear
    }
    float ghostAlpha;
    {
        float grey = dot(refTexColor.rgb, vec3(0.299, 0.587, 0.114));
        ghostAlpha=grey*refTexColor.a;
    }
    
    
    vec4 finalColor=vec4(vec3(1.0),glowAlpha*(1.0-refTexColor.a)+ghostAlpha);
    //if you want to add a extra blue color, open below block
   /*
    //add blue
    finalColor=vec4(finalColor.rgb*finalColor.a+(1.0-finalColor.a)*vec3(0.5,1.0,1.0),finalColor.a);
    */
    gl_FragColor=finalColor;
    
}

