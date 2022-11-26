#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;
uniform float demotime;

#define pi 3.145678

vec2 rot(vec2 p, float a)
{
    float c = cos(a);
    float s = sin(a);
    return mat2(c, -s, s, c) * p;
}

void main()
{
    //vec4 texturecolor = vec4(spriteColor, 1.0) * texture(image, TexCoords);
	//vec4 tempcolor = vec4(1.0,85.0/255.0,48.0/255.0,1.0);
	
	//ec4 mutilcolor = mix(vec4(0.1), tempcolor, smoothstep(0.0,0.5, TexCoords.y));
	
	//float area = step(0.45,TexCoords.x) * step(TexCoords.x,0.5);
	
	//color = mutilcolor * texturecolor;
	
	//float dis = length(pos);
	//if (dis >=0.5) {
		//color = vec4(1.0,1.0,1.0,1.0);
	//} else {
		//float p = 0.02/dis;
		//color =  vec4(p) * tempcolor;
	//}
	//float res = cos(pi*2*dis);
	//color = vec4(res) * tempcolor;
	
	vec2 p = TexCoords - vec2(0.5,0.5);
	//color = vec4( .003 / length(abs(pos) - .1) );
	
    float t = demotime;
    float n = 90;
	float s = 0.0;
	vec3 col = vec3(0.);
    for (float i = 0.; i < mix(0.0,n,0.5+0.5*cos(t)); i++)
    {
        float itr = i / n;
 
        /// @note 螺旋线的半径
        vec2 q = abs(p) - mix(1.5,-0.02,itr) * itr;  ///< 测试，对角线上的某个位置
 
        float d = length(q);
        s += mix(0.0001,0.002,itr) / d;
 
        /// @note 螺旋线的角度
        p = rot(p, 0.05 * t); ///< 将对角线上的点旋转到新位置
    }
 
    col = vec3(s);
    color = vec4(col, 1.0);

}