
#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
uniform sampler2D CC_Texture0;
void main()
{
    float texAlpha=texture2D(CC_Texture0, v_texCoord).a;
    float alpha1=v_fragmentColor.r;
    float alpha2=v_fragmentColor.g;
    float finalAlpha=texAlpha*(alpha2-alpha1)+alpha1;
    gl_FragColor=vec4(vec3(0),finalAlpha);
}