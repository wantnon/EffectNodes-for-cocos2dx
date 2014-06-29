
#ifdef GL_ES
precision mediump float;
#endif

//varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
//uniform sampler2D CC_Texture0;
uniform float u_texNonlinearFactor;
uniform vec4 u_colors[3];
uniform float u_cosA;
uniform float u_sinA;
uniform float u_ribbonRepeat;
void main()
{
    
   vec2 texCoord=vec2(v_texCoord.x,pow(v_texCoord.y,u_texNonlinearFactor));

    vec2 texCoord2;
    texCoord2.x=texCoord.x*u_cosA-texCoord.y*u_sinA;
    texCoord2.y=texCoord.x*u_sinA+texCoord.y*u_cosA;
    
    
    int colorIndex = int(mod((texCoord2.y*u_ribbonRepeat),3.0));
    
    vec4 ribbonColor=u_colors[colorIndex];
    
    vec4 gradualColor=vec4(vec3((1.0-v_texCoord.y)*(1.0-v_texCoord.y)*0.7+0.3),1.0);
    
    gl_FragColor=ribbonColor*gradualColor*1.5;
}