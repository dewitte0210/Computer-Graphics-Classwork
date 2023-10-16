#include "buildTerrainMesh.h"
#include <cstdlib>
void buildTerrainMesh(ofMesh& terrainMesh, const ofShortPixels& heightmap,
	unsigned int xStart, unsigned int yStart, unsigned int xEnd, unsigned int yEnd, glm::vec3 scale) {
	
	//Create verticies
	for (int y = yStart; y <= yEnd; y++) {
		for (int x = xStart; x <= xEnd; x++) {
			int	height = heightmap.getColor(x, y).r;
			terrainMesh.addVertex(glm::vec3(x, static_cast<float>(height) / USHRT_MAX, y) * scale);
		}
	}
	int width = xEnd + 1;
	// Index Buffer
	for (int y = xStart; y < yEnd; y++) {
		for (int x = xStart; x < xEnd; x++) {
			int a = x + y * width;
			int b = (x + 1) + y * width;
			int c = x + (y + 1) * width;
			int d = (x + 1) + (y + 1) * width;

			terrainMesh.addIndex(a);
			terrainMesh.addIndex(c);
			terrainMesh.addIndex(b);

			terrainMesh.addIndex(b);
			terrainMesh.addIndex(c);
			terrainMesh.addIndex(d);
		}
	}
}