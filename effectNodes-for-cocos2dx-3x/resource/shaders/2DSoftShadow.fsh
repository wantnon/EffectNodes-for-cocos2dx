
#ifdef GL_ES
precision highp float;//mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
//uniform sampler2D CC_Texture0;
uniform float u_oneDivObjCount;
void main()
{
    float texLightStrength=texture2D(CC_Texture0, v_texCoord).r;
    float lightStrength_start=v_fragmentColor.r;
    float lightStrength_end=v_fragmentColor.g;
    float finalLightStrength=texLightStrength*(lightStrength_start-lightStrength_end)+lightStrength_end;
    finalLightStrength=finalLightStrength*u_oneDivObjCount;
    gl_FragColor=vec4(vec3(1,1,1),finalLightStrength);
    ////gl_FragColor=vec4(1.0,0.0,0.0,1.0);
}