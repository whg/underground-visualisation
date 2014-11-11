#version 120
uniform float time = 1.0;
uniform float q = 1.0;

vec4 linterp();

void main() {

//    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}

//vec4 linterp() {
//    vec4 res = vec4(0, 0, 0, 0);
//    //this is what moves the point
//    for (int i = 0; i < NUM_POS; i++) {
//        
//        if (time < pos[i].z) {
//            
//            //			if (i > 0) {
//            //				vpos.xy = pos[i-1].xy;
//            //				lasttime = pos[i-1].z;
//            //			}
//            //
//            //			vpos.xy+= ((time-lasttime)/(pos[i].z-lasttime)) * (pos[i].xy - vpos.xy);
//            //			break;
//            
//            //			if (i == 0) {
//            //				vpos.xy+= ((time-starttime.z)/(pos[i].z-starttime.z)) * (pos[i].xy - vpos.xy);
//            //			}
//            //			else {
//            res.xy = pos[i-1].xy + ((time-pos[i-1].z)/(pos[i].z-pos[i-1].z))*(pos[i].xy - pos[i-1].xy);
//            //}
//            break;
//        }
//    }
//    return res;
//}
