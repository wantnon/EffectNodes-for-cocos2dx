
#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
//uniform sampler2D CC_Texture0;

void main()
{
    if(v_texCoord.x<0.0||v_texCoord.x>1.0||v_texCoord.y<0.0||v_texCoord.y>1.0){
        gl_FragColor=vec4(0.0,0.0,0.0,0.0);
    }else{
        gl_FragColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord);
    }
}