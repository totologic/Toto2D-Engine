
uniform sampler2D atlas;

varying vec2 interpolatedUV;

void main()
{
	gl_FragColor = texture2D( atlas, interpolatedUV);
}
