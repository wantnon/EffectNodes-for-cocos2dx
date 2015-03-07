
#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
//uniform sampler2D CC_Texture0;
uniform int u_objCount;
uniform float u_shadowDarkness;//range from 0 to 1.0 ; 1.0 is full darkness
void main()
{
    vec2 texCoord=vec2(v_texCoord.x,v_texCoord.y);
    float grey=texture2D(CC_Texture0, texCoord).r;

    float alpha=1.0-grey;
    alpha=min(1.0,alpha*float(u_objCount));
    const float brightFactor=0.5;
    alpha*=u_shadowDarkness;
    gl_FragColor=vec4(vec3(0,0,0),alpha)  ;
}