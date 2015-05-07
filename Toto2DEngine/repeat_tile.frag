
uniform vec2 position[40];
uniform vec2 atlasSize;
uniform sampler2D atlas;
//uniform vec3 distortionA[40];
//uniform vec3 distortionB[40];
//uniform float distortionType[40];

varying float indexTile;
varying vec2 modUV;
varying vec2 startUV;
varying vec2 interpolatedUV;


void main()
{
	vec2 tiledUV;
	tiledUV.x = startUV.x + mod(interpolatedUV.x, modUV.x);
	tiledUV.y = startUV.y + mod(interpolatedUV.y, modUV.y);
	gl_FragColor = texture2D(atlas, tiledUV);

	/*
	vec2 tiledUV;
	vec4 fragColor;
	float colorMult = 1.0;
	if (distortionType[int(indexTile)] == 0.0)
	{
		tiledUV.x = startUV.x + mod(interpolatedUV.x, modUV.x);
		tiledUV.y = startUV.y - mod(interpolatedUV.y, modUV.y);
	}
	else if (distortionType[int(indexTile)] == 1.0)
	{
		vec3 distort = distortionA[int(indexTile)];
		tiledUV.x = startUV.x + mod(distort[0]*sin((interpolatedUV.y + distort[2])*distort[1]) + interpolatedUV.x, modUV.x);
		distort = distortionB[int(indexTile)];
		tiledUV.y = startUV.y - mod(distort[0]*sin((interpolatedUV.x + distort[2])*distort[1]) + interpolatedUV.y, modUV.y);
	}
	else if (distortionType[int(indexTile)] == 2.0)
	{
		vec3 distort = distortionA[int(indexTile)];
		tiledUV.x = startUV.x + mod(distort[0]*(sin((interpolatedUV.x + distort[2])*distort[1])+1.0) + interpolatedUV.x, modUV.x);
		distort = distortionB[int(indexTile)];
		tiledUV.y = startUV.y - mod(distort[0]*(sin((interpolatedUV.y + distort[2])*distort[1])+1.0) + interpolatedUV.y, modUV.y);
	}
	else if (distortionType[int(indexTile)] == 3.0)
	{
		vec3 distortA = distortionA[int(indexTile)];
		vec3 distortB = distortionB[int(indexTile)];
		float sqrdDist = ((interpolatedUV.x - distortA[0])*(interpolatedUV.x - distortA[0]) + (interpolatedUV.y - distortA[1])*(interpolatedUV.y - distortA[1]));
		tiledUV.x = startUV.x + mod(distortB[0] + (1.0/distortB[2]) / sqrdDist, modUV.x);
		tiledUV.y = startUV.y + mod(atan(interpolatedUV.y - distortA[1], interpolatedUV.x - distortA[0]) + distortB[1], modUV.y);
		colorMult = min(1.0, sqrdDist/distortA[2]);
	}
	else if (distortionType[int(indexTile)] == 4.0)
	{
		vec4 tilePosition = position[int(indexTile)];
		vec3 distortA = distortionA[int(indexTile)];
		vec3 distortB = distortionB[int(indexTile)];
		float circUV = mod(2.0*interpolatedUV.x/distortA[0], 2.0);
		colorMult = min(1.0, circUV * distortA[2]);
		circUV = asin(circUV - 1.0);
		tiledUV.x = startUV.x + mod((circUV/((tilePosition[2]/(atlasSize[0] - 1.0))/distortA[0]) + distortA[1]) * 2.0, modUV.x);
		if(distortB[0] == 1.0)
			tiledUV.y = startUV.y - mod(interpolatedUV.y * (cos(circUV/2.0)), modUV.y);
		else
			tiledUV.y = startUV.y - mod(interpolatedUV.y / (cos(circUV/2.0)), modUV.y);
	}
	else if (distortionType[int(indexTile)] == 5.0)
	{
		vec4 tilePosition = position[int(indexTile)];
		vec3 distortA = distortionA[int(indexTile)];
		vec3 distortB = distortionB[int(indexTile)];
		float circUV = mod(2.0*interpolatedUV.y/distortA[0], 2.0);
		colorMult = min(1.0, circUV * distortA[2]);
		circUV = asin(circUV - 1.0);
		tiledUV.y = startUV.y - mod((circUV/((tilePosition[3]/(atlasSize[1] - 1.0))/distortA[0]) + distortA[1]) * 2.0, modUV.y);
		if(distortB[0] == 1.0)
			tiledUV.x = startUV.x + mod(interpolatedUV.x * (cos(circUV/2.0)), modUV.x);
		else
			tiledUV.x = startUV.x + mod(interpolatedUV.x / (cos(circUV/2.0)), modUV.x);
	}
	else if (distortionType[int(indexTile)] == 6.0)
	{
		vec3 distortA = distortionA[int(indexTile)];
		vec3 distortB = distortionB[int(indexTile)];
		float sqrdDist = ((interpolatedUV.x - distortA[0])*(interpolatedUV.x - distortA[0]) + (interpolatedUV.y - distortA[1])*(interpolatedUV.y - distortA[1]));
		if (sqrdDist > distortB[0])
			discard;
		tiledUV.x = startUV.x + mod(asin(sqrdDist/distortB[0]) / distortB[2], modUV.x);
		tiledUV.y = startUV.y + mod(atan(interpolatedUV.y - distortA[1], interpolatedUV.x - distortA[0]) + distortB[1], modUV.y);
		colorMult = min(1.0, distortA[2]/sqrdDist);
	}
	
	fragColor = texture2D(atlas, tiledUV);
	if (fragColor.x >= 1.0 && fragColor.y <= 0.0 && fragColor.z >= 1.0)
		discard;
	gl_FragColor = fragColor*colorMult;
	*/
}
