$input a_position, a_normal

#include <../common.sh>

void main() {
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
}
