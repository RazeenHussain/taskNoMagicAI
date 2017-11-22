#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "OBJ_Loader.h"

int main(int argc, char* argv[])
{
	float cellSize = 0.1f;
	// Initialize loader
	objl::Loader Loader;
	// Load .obj file
	bool loadout = Loader.LoadFile("Dataset/tetrahedron.obj");
	// Check for successful load
	if (loadout)
	{
		// Create output file
		std::ofstream file("out.txt");
		std::cout << "File loaded successfully\n";
		file << "File loaded\n";
		// Process each object mesh individually
		for (unsigned int i = 0; i < Loader.LoadedMeshes.size(); i++)
		{
			file << "Mesh " << i << "\n";
			// Load current mesh
			objl::Mesh curMesh = Loader.LoadedMeshes[i];
			unsigned int numTriangle = 0;
			numTriangle = curMesh.Indices.size() / 3;
			// Calculate centre of mass for all triangles
			std::vector<objl::Vector3> centreTriangle;
			objl::Vector3 tempCentre;
			for (unsigned int j = 0; j < curMesh.Indices.size(); j += 3)
			{
				tempCentre.X = (curMesh.Vertices[curMesh.Indices[j]].Position.X + curMesh.Vertices[curMesh.Indices[j + 1]].Position.X + curMesh.Vertices[curMesh.Indices[j + 2]].Position.X) / 3;
				tempCentre.Y = (curMesh.Vertices[curMesh.Indices[j]].Position.Y + curMesh.Vertices[curMesh.Indices[j + 1]].Position.Y + curMesh.Vertices[curMesh.Indices[j + 2]].Position.Y) / 3;
				tempCentre.Z = (curMesh.Vertices[curMesh.Indices[j]].Position.Z + curMesh.Vertices[curMesh.Indices[j + 1]].Position.Z + curMesh.Vertices[curMesh.Indices[j + 2]].Position.Z) / 3;
				centreTriangle.push_back(tempCentre);
			}
			// Compute range for the grid based on object mesh size
			auto minmaxX = std::minmax_element(curMesh.Vertices.begin(), curMesh.Vertices.end(), [](const objl::Vertex& pLeft, const objl::Vertex& pRight) {
				return pLeft.Position.X < pRight.Position.X;
			});
			auto minmaxY = std::minmax_element(curMesh.Vertices.begin(), curMesh.Vertices.end(), [](const objl::Vertex& pLeft, const objl::Vertex& pRight) {
				return pLeft.Position.Y < pRight.Position.Y;
			});
			auto minmaxZ = std::minmax_element(curMesh.Vertices.begin(), curMesh.Vertices.end(), [](const objl::Vertex& pLeft, const objl::Vertex& pRight) {
				return pLeft.Position.Z < pRight.Position.Z;
			});
			objl::Vector3 minGrid = { std::min(0.0f,minmaxX.first->Position.X),std::min(0.0f,minmaxY.first->Position.Y),std::min(0.0f,minmaxZ.first->Position.Z) };
			objl::Vector3 maxGrid = { std::max(0.0f,minmaxX.second->Position.X),std::max(0.0f,minmaxY.second->Position.Y),std::max(0.0f,minmaxZ.second->Position.Z) };
			objl::Vector3 rangeGrid = { (maxGrid.X - minGrid.X),(maxGrid.Y - minGrid.Y),(maxGrid.Z - minGrid.Z) };
			objl::Vector3 curCell = {(ceil(minGrid.X)+0.05f),(ceil(minGrid.Y) + 0.05f),(ceil(minGrid.Z) + 0.05f) };
			curCell.X = ceil(minGrid.X) + 0.05f;
			// Generate grid and locate the corresponding furthest triangle
			float curDistance = 0;
			float maxDistance = 0;
			unsigned int furthestTriangle = 0;
			std::cout << "Computing distances\n";
			for (unsigned int j = 0; j < ceil(rangeGrid.X / cellSize); j++)
			{
				curCell.Y = ceil(minGrid.Y) + 0.05f;
				for (unsigned int k = 0; k < ceil(rangeGrid.Y / cellSize); k++)
				{
					curCell.Z = ceil(minGrid.Z) + 0.05f;
					for (unsigned int l = 0; l < ceil(rangeGrid.Z / cellSize); l++)
					{
						furthestTriangle = 0;
						maxDistance = 0;
						curDistance = 0;
						for (unsigned int m = 0; m < numTriangle; m++)
						{
							curDistance = sqrtf(powf((centreTriangle[m].X - curCell.X), 2) + powf((centreTriangle[m].Y - curCell.Y), 2) + powf((centreTriangle[m].Z - curCell.Z), 2));
							if (curDistance > maxDistance)
							{	
								maxDistance = curDistance;
								furthestTriangle = m;
							}
						}
						file << "Cell centre: {" << curCell.X << "," << curCell.Y << "," << curCell.Z << "} Furthest Triangle: " << (furthestTriangle+1) << " Triangle centre: {" << centreTriangle[furthestTriangle].X << "," << centreTriangle[furthestTriangle].Y << "," << centreTriangle[furthestTriangle].Z << "} Distance: " << maxDistance << "\n";
						curCell.Z += cellSize;
					}
					curCell.Y += cellSize;
				}
				curCell.X += cellSize;
			}
		}
		std::cout << "Calculation completed\n";
		// Close output file
		file.close();
	}
	else
	{
		// Create output file and output the error
		std::ofstream file("e1Out.txt");
		std::cout << "Failed to load file\n";
		file << "Failed to Load File. May have failed to find it or it was not an .obj file.\n";
		file.close();
	}
	return 0;
}