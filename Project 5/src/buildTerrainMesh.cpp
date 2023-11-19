#include "buildTerrainMesh.h"
using namespace glm;

void buildTerrainMesh(ofMesh& terrainMesh, const ofShortPixels& heightmap,
    unsigned int xStart, unsigned int yStart, unsigned int xEnd, unsigned int yEnd, vec3 scale)
{
    // Initialize vertex positions
    for (unsigned int x{ xStart }; x <= xEnd; x++)
    {
        for (unsigned int y{ yStart }; y <= yEnd; y++)
        {
            // Vertex position
            terrainMesh.addVertex(scale * (vec3(x, static_cast<float>(heightmap.getColor(x, y).r) / static_cast<float>(USHRT_MAX), y)));

            // UV coordinates
            terrainMesh.addTexCoord(vec2(x / 128.0, y / 128.0));

            // Calculate normal

            // Decide which heightmap pixels to sample
            // Make sure the indices aren't out of bounds
            int x1 = glm::max(1u, x) - 1;
            int x2 = glm::min(heightmap.getWidth() - 1, x + 1ull);
            int y1 = glm::max(1u, y) - 1;
            int y2 = glm::min(heightmap.getHeight() - 1, y + 1ull);

            // Generate vectors roughly parallel to the ground
            vec3 v1{ scale * vec3(x - x1, static_cast<float>(heightmap.getColor(x, y).r - heightmap.getColor(x1, y).r) / static_cast<float>(USHRT_MAX), 0) };
            vec3 v2{ scale * vec3(x2 - x, static_cast<float>(heightmap.getColor(x2, y).r - heightmap.getColor(x, y).r) / static_cast<float>(USHRT_MAX), 0) };

            vec3 w1{ scale * vec3(0, static_cast<float>(heightmap.getColor(x, y1).r - heightmap.getColor(x, y).r) / static_cast<float>(USHRT_MAX), y1 - y) };
            vec3 w2{ scale * vec3(0, static_cast<float>(heightmap.getColor(x, y).r - heightmap.getColor(x, y2).r) / static_cast<float>(USHRT_MAX), y - y2) };

            // Take a cross product to get the normal vector
            terrainMesh.addNormal(normalize(cross(normalize(w1 + w2), normalize(v1 + v2))));
        }
    }

    // Initialize index buffer
    int k{ 0 }; // k stores the index of the first corner of the quad.
    for (unsigned int x{ xStart }; x < xEnd; x++)
    {
        for (unsigned int y{ yStart }; y < yEnd; y++)
        {
            if (k % 2)
            {
                // Triangle 1
                terrainMesh.addIndex(k);                           // SW
                terrainMesh.addIndex(k + 1);                       // NW
                terrainMesh.addIndex(k + (1 + yEnd - yStart));     // SE

                // Triangle 2
                terrainMesh.addIndex(k + (1 + yEnd - yStart));     // SE
                terrainMesh.addIndex(k + 1);                       // NW
                terrainMesh.addIndex(k + (1 + yEnd - yStart) + 1); // NE
            }
            else
            {
                // Triangle 1
                terrainMesh.addIndex(k + 1);                       // NW
                terrainMesh.addIndex(k + (1 + yEnd - yStart) + 1); // NE
                terrainMesh.addIndex(k + (1 + yEnd - yStart));     // SE

                // Triangle 2
                terrainMesh.addIndex(k + (1 + yEnd - yStart));     // SE
                terrainMesh.addIndex(k);                           // SW
                terrainMesh.addIndex(k + 1);                       // NW
            }

            k++; // Advance k to the next vertex in the "column."
        }

        // We've reached the end of the column, advance k an extra time past the final vertex of the column, to the start of the next column.
        k++;
    }
    
    for (size_t i{ 0 }; i < terrainMesh.getNumNormals(); i++)
    {
        terrainMesh.setNormal(i, -terrainMesh.getNormal(i));
    }
}

void buildWaterMesh(ofMesh& waterMesh, unsigned int xStart, unsigned int yStart, unsigned int xEnd, unsigned int yEnd, int planeHeight, vec3 scale) {

    for (unsigned int x{ xStart }; x <= xEnd; x++)
    {
        for (unsigned int y{ yStart }; y <= yEnd; y++)
        {
            waterMesh.addVertex(scale * vec3(x, planeHeight, y));
            waterMesh.addTexCoord(vec2(x / 8.0, y / 8.0));
        }
    }

    // Initialize index buffer
    int k{ 0 }; // k stores the index of the first corner of the quad.
    for (unsigned int x{ xStart }; x < xEnd; x++)
    {
        for (unsigned int y{ yStart }; y < yEnd; y++)
        {
            if (k % 2)
            {
                // Triangle 1
                waterMesh.addIndex(k);                           // SW
                waterMesh.addIndex(k + 1);                       // NW
                waterMesh.addIndex(k + (1 + yEnd - yStart));     // SE

                // Triangle 2
                waterMesh.addIndex(k + (1 + yEnd - yStart));     // SE
                waterMesh.addIndex(k + 1);                       // NW
                waterMesh.addIndex(k + (1 + yEnd - yStart) + 1); // NE
            }
            else
            {
                // Triangle 1
                waterMesh.addIndex(k + 1);                       // NW
                waterMesh.addIndex(k + (1 + yEnd - yStart) + 1); // NE
                waterMesh.addIndex(k + (1 + yEnd - yStart));     // SE

                // Triangle 2
                waterMesh.addIndex(k + (1 + yEnd - yStart));     // SE
                waterMesh.addIndex(k);                           // SW
                waterMesh.addIndex(k + 1);                       // NW
            }

            k++; // Advance k to the next vertex in the "column."
        }

        // We've reached the end of the column, advance k an extra time past the final vertex of the column, to the start of the next column.
        k++;
    }
    waterMesh.flatNormals();
}
