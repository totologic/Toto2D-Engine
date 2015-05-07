
uniform mat4 screenMat;
uniform mat4 cameraMat;
uniform vec2 atlasSize;
uniform vec4 uv[10];
uniform float uvIndex[40];
uniform vec2 position[40];

attribute float index;

varying vec2 interpolatedUV;

void main()
{
	int tileIndex = int(index*0.25);
	int vertexIndex = int(index-floor(index*0.25)*4.0);

	vec2 tilePosition = position[tileIndex];
	vec4 uvData = uv[int(uvIndex[tileIndex])];
	vec4 vertex = vec4(0.0, 0.0, 0.0, 1.0);

	if (vertexIndex == 0)
	{
		vertex.x = tilePosition.x;
		vertex.y = tilePosition.y;
		interpolatedUV.x = uvData[0] / atlasSize[0];
		interpolatedUV.y = uvData[1] / atlasSize[1];
	}
	else if (vertexIndex == 1)
	{
		vertex.x = tilePosition.x;
		vertex.y = uvData[3] + tilePosition.y;
		interpolatedUV.x = uvData[0] / atlasSize[0];
		interpolatedUV.y = (uvData[1] + uvData[3]) / atlasSize[1];
	}
	else if (vertexIndex == 2)
	{
		vertex.x = uvData[2] + tilePosition.x;
		vertex.y = uvData[3] + tilePosition.y;
		interpolatedUV.x = (uvData[0] + uvData[2]) / atlasSize[0];
		interpolatedUV.y = (uvData[1] + uvData[3]) / atlasSize[1];
	}
	else if (vertexIndex == 3)
	{
		vertex.x = uvData[2] + tilePosition.x;
		vertex.y = tilePosition.y;
		interpolatedUV.x = (uvData[0] + uvData[2]) / atlasSize[0];
		interpolatedUV.y = uvData[1] / atlasSize[1];
	}

	gl_Position = screenMat * cameraMat * vertex;
}
