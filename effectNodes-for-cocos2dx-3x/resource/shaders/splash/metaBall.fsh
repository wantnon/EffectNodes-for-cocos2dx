
#ifdef GL_ES
precision mediump float;
#endif

//this shader is ref to ccShader_PositionTextureColor_frag.h
//v_fragmentColor is corresponding to sprite's opacity

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
////uniform sampler2D CC_Texture0;
uniform float u_highlight;
void main()
{
    vec4 texColor=texture2D(CC_Texture0,v_texCoord);
    
    

    
    
    
    vec4 color=texColor*v_fragmentColor;
    
    vec4 fragColor;
    if(color.r>0.5){
        ////fragColor=vec4(1.0,1.0,1.0,1.0);
        fragColor=vec4(0.0,0.0,1.0,(color.r-0.5+0.4))*u_highlight;
    }else{
        ////fragColor=vec4(0.0,0.0,0.0,1.0);
        discard;
    }
    
    gl_FragColor = fragColor;//vec4(color.r,color.r,color.r,1.0);//
    
    
}
