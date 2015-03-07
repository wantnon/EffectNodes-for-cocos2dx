
#ifdef GL_ES
precision mediump float;
#endif

//this shader is ref to ccShader_PositionTextureColor_frag.h
//v_fragmentColor is corresponding to sprite's opacity

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
//uniform sampler2D CC_Texture0;
uniform float u_highlight;
void main()
{
    vec4 texColor=texture2D(CC_Texture0,v_texCoord);
    
    

    
    gl_FragColor = texColor*v_fragmentColor*u_highlight;
    
    
    
    
}
