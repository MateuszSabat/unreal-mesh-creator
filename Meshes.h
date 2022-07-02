#pragma once

#include "CoreMinimal.h"

#include "MeshDescription.h"
#include "StaticMeshDescription.h"
#include "StaticMeshAttributes.h"

namespace Meshes
{
	class Triangles
	{
	private:
		FMeshDescription* _description;
		FPolygonGroupID _id;

	public:
		Triangles() : _description(nullptr), _id(0) {}

		void Init(FMeshDescription* description, FPolygonGroupID id);

		void AddTriangle(FVertexInstanceID v0, FVertexInstanceID v1, FVertexInstanceID v2);
		void AddQuad(FVertexInstanceID v0, FVertexInstanceID v1, FVertexInstanceID v2, FVertexInstanceID v3);
		void AddHex(FVertexInstanceID v0, FVertexInstanceID v1, FVertexInstanceID v2, FVertexInstanceID v3, FVertexInstanceID v4, FVertexInstanceID v5);
	};

	class InitializedDescription
	{
	private:
		FMeshDescription& _description;

	public:
		InitializedDescription(FMeshDescription& description) : _description(description) { }

		InitializedDescription& WithPositions(TArrayView<FVector3f>& positions);
		InitializedDescription& WithUVs(TArrayView<FVector2f>& uv);
		InitializedDescription& WithNormals(TArrayView<FVector3f>& normals);
		InitializedDescription& WithTriangles(Triangles& triangles, FPolygonGroupID id);
	};

	InitializedDescription Init(FMeshDescription& description, int vertices, int triangles);
}

namespace StaticMeshes
{
	FMeshDescription Grid(int x, int y, float cellSize);
	FMeshDescription SteinerGrid(int x, int y, float cellSize);
	FMeshDescription Cube(float size);
	FMeshDescription Sphere(float radius, int density);
}