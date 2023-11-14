#include "buildTerrainMesh.h"
#include <cstdlib>
void buildTerrainMesh(ofMesh& terrainMesh, const ofShortPixels& heightmap,
	unsigned int xStart, unsigned int yStart, unsigned int xEnd, unsigned int yEnd, glm::vec3 scale) {
	
	terrainMesh.clear();

	//Create verticies
	for (unsigned int y = yStart; y <= yEnd; y++) {
		for (unsigned int x = xStart; x <= xEnd; x++) {
			int	height = heightmap.getColor(x, y).r;
			terrainMesh.addVertex(glm::vec3(x, static_cast<float>(height) / USHRT_MAX, y) * scale);

			//The following are additions for project 5 
			//add UV coordinates
			terrainMesh.addTexCoord(glm::vec2(x / 128.0, y / 128.0));
			
			//Calculate Normals
			//Decide which heightmpa pixels to sample
			//Make sure the indicies aren't out of bounds	
			int x1 = glm::max(1u, x) - 1;
			int x2 = glm::min(heightmap.getWidth() - 1, x + 1ull);
			int y1 = glm::max(1u, y) - 1;
			int y2 = glm::min(heightmap.getHeight() - 1, y + 1ull);

			//Generate vectors roughtly parrallel to the ground
			glm::vec3 v1{ scale * glm::vec3(x - x1, static_cast<float>(heightmap.getColor(x,y).r - heightmap.getColor(x1,y).r) / static_cast<float>(USHRT_MAX), 0) };
			glm::vec3 v2{ scale * glm::vec3(x2 - x, static_cast<float>(heightmap.getColor(x2,y).r - heightmap.getColor(x,y).r) / static_cast<float>(USHRT_MAX), 0) };
			glm::vec3 w1{ scale * glm::vec3(0, static_cast<float>(heightmap.getColor(x,y1).r - heightmap.getColor(x,y).r) / static_cast<float>(USHRT_MAX), 0) };
			glm::vec3 w2{ scale * glm::vec3(0, static_cast<float>(heightmap.getColor(x,y).r - heightmap.getColor(x,y2).r) / static_cast<float>(USHRT_MAX), 0) };

			//Take a cross product to get the normal vector
			terrainMesh.addNormal(glm::normalize(glm::cross(glm::normalize(w1 + w2), glm::normalize(v1 + v2))));
		}
	}


	int width = (xEnd - xStart) + 1;
	int height = (yEnd - yStart) + 1;
	// Index Buffer
	for (int y = 0; y < height -1; y++) {
		for (int x = 0; x < width -1; x++) {
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
	//terrainMesh.flatNormals();
}