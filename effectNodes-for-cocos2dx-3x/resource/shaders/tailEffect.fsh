

#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;
varying vec4 v_color;

//uniform sampler2D CC_Texture0;


void main() {

    vec4 texColor=texture2D(CC_Texture0, v_texCoord);
    gl_FragColor=texColor*v_color;




}

