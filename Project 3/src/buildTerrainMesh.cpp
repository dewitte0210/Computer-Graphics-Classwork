#include "buildTerrainMesh.h"

void buildTerrainMesh(ofMesh& terrainMesh, const ofShortPixels& heightmap,
	unsigned int xStart, unsigned int yStart, unsigned int xEnd, unsigned int yEnd, glm::vec3 scale) {
	
	//Create verticies
	for (int x = xStart; x < xEnd; x++) {
		for (int y = yStart; y < yEnd; y++) {
			int	height = heightmap.getColor(x, y).r;
			terrainMesh.addVertex(glm::vec3(x, height / USHRT_MAX, y));
		}
	}

	// Index Buffer
	for (int y = xStart; y < yEnd - 1; y++) {
		for (int x = xStart; x < xEnd - 1; x++) {
			int a = x + y * xEnd;
			int b = (x + 1) + y * xEnd;
			int c = x + (y + 1) * xEnd;
			int d = (x + 1) + (y + 1) * xEnd;

			terrainMesh.addIndex(a);
			terrainMesh.addIndex(b);
			terrainMesh.addIndex(c);

			terrainMesh.addIndex(b);
			terrainMesh.addIndex(d);
			terrainMesh.addIndex(c);
		}
	}
}