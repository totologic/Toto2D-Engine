
uniform mat4 screenMat;
uniform mat4 cameraMat;
uniform vec2 atlasSize;
uniform vec4 uv[10];
uniform float uvIndex[40];
uniform vec2 position[40];
uniform vec2 dimension[40];
uniform vec2 scroll[40];

attribute float index;

varying float indexTile;
varying vec2 modUV;
varying vec2 startUV;
varying vec2 interpolatedUV;

void main()
{
	
	indexTile = index*0.25;
	int tileIndex = int(index*0.25);
	int vertexIndex = int(index-floor(index*0.25)*4.0);
	
	vec2 tilePosition = position[tileIndex];
	vec2 tileDimension = dimension[tileIndex];
	vec4 uvData = uv[int(uvIndex[tileIndex])];
	vec4 vertex = vec4(0.0, 0.0, 0.0, 1.0);
	
	modUV.x = uvData[2] / atlasSize[0];
	modUV.y = uvData[3] / atlasSize[1];
	startUV.x = uvData[0] / atlasSize[0];
	startUV.y = uvData[1] / atlasSize[1];

	if (vertexIndex == 0)
	{
		vertex.x = tilePosition[0];
		vertex.y = tilePosition[1];
		interpolatedUV.x = scroll[tileIndex].x / atlasSize[0];
		interpolatedUV.y = scroll[tileIndex].y / atlasSize[0];
	}
	else if (vertexIndex == 1)
	{
		vertex.x = tilePosition[0];
		vertex.y = tilePosition[1] + tileDimension[1];
		interpolatedUV.x = scroll[tileIndex].x / atlasSize[0];
		interpolatedUV.y = (tileDimension[1] + scroll[tileIndex].y) / atlasSize[1];
	}
	else if (vertexIndex == 2)
	{
		vertex.x = tilePosition[0] + tileDimension[0];
		vertex.y = tilePosition[1] + tileDimension[1];
		interpolatedUV.x = (tileDimension[0] + scroll[tileIndex].x) / atlasSize[0];
		interpolatedUV.y = (tileDimension[1] + scroll[tileIndex].y) / atlasSize[1];
	}
	else if (vertexIndex == 3)
	{
		vertex.x = tilePosition[0] + tileDimension[0];
		vertex.y = tilePosition[1];
		interpolatedUV.x = (tileDimension[0] + scroll[tileIndex].x) / atlasSize[0];
		interpolatedUV.y = scroll[tileIndex].y / atlasSize[0];
	}

	gl_Position = screenMat * cameraMat * vertex;

}
