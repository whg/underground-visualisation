//#version 120
uniform float time;
uniform vec2 resolution;

uniform sampler2DRect tex0, tex1, tex2, tex3, tex4, tex5, tex6, tex7, tex8, tex9, tex10, tex11;


vec2 linterp(vec2 coord) {

    vec4 c;
    c = texture2DRect(tex1, coord);
    vec4 d = texture2DRect(tex0, coord);
    if(time < d.z || c.z < 199.0) return vec2(0);
    
    if(time < c.z) {
        return d.xy + ((time - d.z) / (c.z - d.z)) * (c.xy - d.xy);
    }
    
//    return vec2(0.0);

    c = texture2DRect(tex2, coord);
    if(time < c.z) {
        vec4 d = texture2DRect(tex1, coord);
        return d.xy + ((time - d.z) / (c.z - d.z)) * (c.xy - d.xy);
    }

    c = texture2DRect(tex3, coord);
    if(time < c.z) {
        vec4 d = texture2DRect(tex2, coord);
        return d.xy + ((time - d.z) / (c.z - d.z)) * (c.xy - d.xy);
    }

    c = texture2DRect(tex4, coord);
    if(time < c.z) {
        vec4 d = texture2DRect(tex3, coord);
        return d.xy + ((time - d.z) / (c.z - d.z)) * (c.xy - d.xy);
    }

    c = texture2DRect(tex5, coord);
    if(time < c.z) {
        vec4 d = texture2DRect(tex4, coord);
        return d.xy + ((time - d.z) / (c.z - d.z)) * (c.xy - d.xy);
    }

    c = texture2DRect(tex6, coord);
    if(time < c.z) {
        vec4 d = texture2DRect(tex5, coord);
        return d.xy + ((time - d.z) / (c.z - d.z)) * (c.xy - d.xy);
    }

    c = texture2DRect(tex7, coord);
    if(time < c.z) {
        vec4 d = texture2DRect(tex6, coord);
        return d.xy + ((time - d.z) / (c.z - d.z)) * (c.xy - d.xy);
    }
    
    c = texture2DRect(tex8, coord);
    if(time < c.z) {
        vec4 d = texture2DRect(tex7, coord);
        return d.xy + ((time - d.z) / (c.z - d.z)) * (c.xy - d.xy);
    }
    
    c = texture2DRect(tex9, coord);
    if(time < c.z) {
        vec4 d = texture2DRect(tex8, coord);
        return d.xy + ((time - d.z) / (c.z - d.z)) * (c.xy - d.xy);
    }
    
    c = texture2DRect(tex10, coord);
    if(time < c.z) {
        vec4 d = texture2DRect(tex9, coord);
        return d.xy + ((time - d.z) / (c.z - d.z)) * (c.xy - d.xy);
    }
    
    c = texture2DRect(tex11, coord);
    if(time < c.z) {
        vec4 d = texture2DRect(tex10, coord);
        return d.xy + ((time - d.z) / (c.z - d.z)) * (c.xy - d.xy);
    }
    
    return vec2(0);

//    c = texture2DRect(tex8, coord);
//    if(time < c.z) {
//        vec4 d = texture2DRect(tex7, coord);
//        return d.xy + ((time - d.z) / (c.z - d.z)) * (c.xy - d.xy);
//    }

}

void main() {

//    const int NUM_TEX = 8;
//    sampler2DRect tex[NUM_TEX];
//
//    tex[0] = tex0; tex[1] = tex1; tex[2] = tex2;
//    tex[3] = tex3; tex[4] = tex4; tex[5] = tex5;
//    tex[6] = tex6; tex[7] = tex7;
    

    vec2 p = vec2(gl_FragCoord.x, gl_FragCoord.y);
    vec2 r = linterp(p);
//    vec2 r = texture2DRect(tex0, p).xy;
    gl_FragColor = vec4(r.x, r.y, 1.0, 1.0);
//    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    
//    vec2 texition = (( gl_FragCoord.xy / resolution.xy )*2.0 - 1.0);
//    vec2 uv = gl_FragCoord.xy / resolution;
//    vec4 texcol = texture2DRect(tex, vec2(gl_FragCoord.x, resolution.y - gl_FragCoord.y));
//    
////    gl_FragColor = vec4(gl_FragCoord.x / resolution.x, gl_TexCoord[1].t, 1.0, 1.0); //texcol; //vec4(vec3(texcol.x), 1.0);
//    gl_FragColor = texcol;
////    float v = sin(uv.x*2.0*3.14) + cos(uv.y*6.3);
//    gl_FragColor = vec4(vec3(v*0.5), 1.0);
//    gl_FragColor = gl_Color;
//    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}