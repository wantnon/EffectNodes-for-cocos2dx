
#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
uniform sampler2D CC_Texture0;
void main()
{
    vec2 texCoord=vec2(v_texCoord.x,v_texCoord.y);
    float grey=texture2D(CC_Texture0, texCoord).r;

    float alpha=1.0-grey;
    alpha=min(1.0,alpha*3.0);
    alpha*=0.5;
    gl_FragColor=vec4(vec3(0,0,0),alpha)  ;
}