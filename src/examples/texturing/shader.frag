varying vec2 v_TexCoord;

uniform sampler2D uTex;

void main()
{
	vec4 tex = texture2D(uTex,v_TexCoord);
	gl_FragColor = tex;
}