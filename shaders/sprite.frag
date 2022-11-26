#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;
uniform float demotime;

#define pi 3.145678

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
	
	vec2 pos = TexCoords - vec2(0.5,0.5);
	//float dis = length(abs(pos)-0.1);
	float dis = length(pos);
	

	color = vec4(0.003/dis);

}