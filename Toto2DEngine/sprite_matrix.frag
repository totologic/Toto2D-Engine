
uniform sampler2D atlas;

varying vec2 interpolatedUV;
varying vec2 spriteEffect;

void main()
{
	gl_FragColor = texture2D( atlas, interpolatedUV);
	
	if (spriteEffect[0] != 0.0)
	{
		float effectType = mod(spriteEffect[0], 10.0);
		//float effectType = spriteEffect[0]-floor(spriteEffect[0]*0.1)*10.0;
		if (effectType < 1.1) // TINT
		{
			float intensity = ((spriteEffect[0] - effectType) / 10.0) * 0.003921568627451; // 1/255
			float invIntensity = 1.0 - intensity;
			float colorEffect = spriteEffect[1];
			float canal = colorEffect - 256.0 * floor(colorEffect * 0.00390625);
			gl_FragColor.x = invIntensity * gl_FragColor.x + intensity * canal * 0.003921568627451; // 1/255
			colorEffect = (colorEffect - canal) * 0.00390625; // 1/256;
			canal = colorEffect - 256.0 * floor(colorEffect * 0.00390625);
			gl_FragColor.y = invIntensity * gl_FragColor.y + intensity * canal * 0.003921568627451; // 1/255
			colorEffect = (colorEffect - canal) * 0.00390625; // 1/256;
			canal = colorEffect - 256.0 * floor(colorEffect * 0.00390625);
			gl_FragColor.z = invIntensity * gl_FragColor.z + intensity * canal * 0.003921568627451; // 1/255
		}
		else if (effectType < 2.1) // UNSATURATE
		{
			float intensity = ((spriteEffect[0] - effectType) / 10.0) / 255.0;
			float invIntensity = 1.0 - intensity;
			float saturated = (gl_FragColor.x * 0.6 + gl_FragColor.y * 0.3 + gl_FragColor.z * 0.1) * intensity;
			gl_FragColor.x = gl_FragColor.x * invIntensity + saturated;
			gl_FragColor.y = gl_FragColor.y * invIntensity + saturated;
			gl_FragColor.z = gl_FragColor.z * invIntensity + saturated;
		}
	}
}
