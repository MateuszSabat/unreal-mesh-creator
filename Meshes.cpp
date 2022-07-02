#include "Meshes.h"

#include "GenericPlatform/GenericPlatformMath.h"
#include "MathUtil.h"

void Meshes::Triangles::Init(FMeshDescription* description, FPolygonGroupID id)
{
	_description = description;
	_id = id;
}

void Meshes::Triangles::AddTriangle(FVertexInstanceID v0, FVertexInstanceID v1, FVertexInstanceID v2)
{
	_description->CreateTriangle(_id, { v0, v1, v2 });
}

void Meshes::Triangles::AddQuad(FVertexInstanceID v0, FVertexInstanceID v1, FVertexInstanceID v2, FVertexInstanceID v3)
{
	AddTriangle(v0, v1, v2);
	AddTriangle(v2, v3, v0);
}

void Meshes::Triangles::AddHex(FVertexInstanceID v0, FVertexInstanceID v1, FVertexInstanceID v2, FVertexInstanceID v3, FVertexInstanceID v4, FVertexInstanceID v5)
{
	AddQuad(v0, v1, v2, v3);
	AddQuad(v3, v4, v5, v0);
}

Meshes::InitializedDescription Meshes::Init(FMeshDescription& description, int vertices, int triangles)
{
	FStaticMeshAttributes(description).Register();

	description.ReserveNewVertices(vertices);
	description.ReserveNewVertexInstances(vertices);

	description.CreatePolygonGroup();
	description.ReserveNewPolygons(triangles);
	description.ReserveNewTriangles(triangles);
	description.ReserveNewEdges(triangles * 3);

	for (int v = 0; v < vertices; ++v)
	{
		description.CreateVertex();
		description.CreateVertexInstance(v);
	}

	description.BuildVertexIndexers();

	return InitializedDescription(description);
}

Meshes::InitializedDescription& Meshes::InitializedDescription::WithPositions(TArrayView<FVector3f>& positions)
{
	positions = _description.GetVertexPositions().GetRawArray();
	return *this;
}

Meshes::InitializedDescription& Meshes::InitializedDescription::WithUVs(TArrayView<FVector2f>& uv)
{
	return WithVertexInstanceAttributes<FVector2f>(uv, MeshAttribute::VertexInstance::TextureCoordinate);
}

Meshes::InitializedDescription& Meshes::InitializedDescription::WithNormals(TArrayView<FVector3f>& normals)
{
	return WithVertexInstanceAttributes<FVector3f>(normals, MeshAttribute::VertexInstance::Normal);
}

Meshes::InitializedDescription& Meshes::InitializedDescription::WithTangents(TArrayView<FVector3f>& tangents)
{
	return WithVertexInstanceAttributes<FVector3f>(tangents, MeshAttribute::VertexInstance::Tangent);
}

Meshes::InitializedDescription& Meshes::InitializedDescription::WithColors(TArrayView<FVector4f>& colors)
{
	return WithVertexInstanceAttributes<FVector4f>(colors, MeshAttribute::VertexInstance::Color);
}

Meshes::InitializedDescription& Meshes::InitializedDescription::WithBinormalSigns(TArrayView<float>& binormalSigns)
{
	return WithVertexInstanceAttributes<float>(binormalSigns, MeshAttribute::VertexInstance::BinormalSign);
}

Meshes::InitializedDescription& Meshes::InitializedDescription::WithTriangles(Triangles& triangles, FPolygonGroupID id)
{
	triangles.Init(&_description, id);
	return *this;
}


FMeshDescription Meshes::SimpleQuad()
{
	FMeshDescription description;

	TArrayView<FVector3f> positions;
	TArrayView<FVector2f> uvs;
	TArrayView<FVector3f> normals;
	Meshes::Triangles triangles;

	int vertexCount = 4;
	int trianglesCount = 2;
	Meshes::Init(description, vertexCount, trianglesCount)
		.WithPositions(positions)
		.WithUVs(uvs)
		.WithNormals(normals)
		.WithTriangles(triangles, 0);

	positions[0] = FVector3f(-100, -100, 0);
	positions[1] = FVector3f(-100, 100, 0);
	positions[2] = FVector3f(100, -100, 0);
	positions[3] = FVector3f(100, 100, 0);

	normals[0] = FVector3f(0, 0, 1);
	normals[1] = FVector3f(0, 0, 1);
	normals[2] = FVector3f(0, 0, 1);
	normals[3] = FVector3f(0, 0, 1);

	uvs[0] = FVector2f(0, 0);
	uvs[1] = FVector2f(0, 1);
	uvs[2] = FVector2f(1, 0);
	uvs[3] = FVector2f(1, 1);

	triangles.AddQuad(0, 1, 3, 2);

	// the same as:
	
	// triangles.AddTriangle(0, 1, 2);
	// triangles.AddTriangle(2, 3, 0);

	return description;
}

FMeshDescription Meshes::Grid(int x, int y, float cellSize)
{
	FMeshDescription description;

	TArrayView<FVector3f> positions;
	TArrayView<FVector2f> uvs;
	TArrayView<FVector3f> normals;
	Meshes::Triangles triangles;

	Meshes::Init(description, (x + 1) * (y + 1), x * y * 2)
		.WithPositions(positions)
		.WithUVs(uvs)
		.WithNormals(normals)
		.WithTriangles(triangles, 0);

	int i = 0;
	for (int ix = 0; ix <= x; ++ix)
	{
		for (int iy = 0; iy <= y; ++iy)
		{
			if (ix < x && iy < y) 
			{
				int v0 = i;
				int v1 = v0 + 1;
				int v3 = i + y + 1;
				int v2 = v3 + 1;

				triangles.AddQuad(v0, v1, v2, v3);
			}

			float u = (float)ix / (x - 1);
			float v = (float)iy / (y - 1);

			uvs[i] = FVector2f(u, v);
			positions[i] = FVector3f(u * x * cellSize, v * y * cellSize, 0);
			normals[i] = FVector3f(0, 0, 1);

			i++;
		}
	}

	return description;
}

FMeshDescription Meshes::SteinerGrid(int x, int y, float cellSize)
{
	FMeshDescription description;

	TArrayView<FVector3f> positions;
	TArrayView<FVector2f> uvs;
	TArrayView<FVector3f> normals;
	Meshes::Triangles triangles;

	Meshes::Init(description, (x + 1) * (y + 1) + x * y, x * y * 4)
		.WithPositions(positions)
		.WithUVs(uvs)
		.WithNormals(normals)
		.WithTriangles(triangles, 0);

	float du = 1.0f / x;
	float dv = 1.0f / y;

	for (int ix = 0; ix <= x; ++ix)
	{
		for (int iy = 0; iy <= y; ++iy)
		{
			int v1 = ix * (y + y + 1) + iy;

			if (ix < x && iy < y)
			{
				int v0 = v1 + (y + 1);

				int v2 = v1 + 1;
				int v4 = v0 + y;
				int v3 = v4 + 1;

				float cx = (float)ix + 0.5f;
				float cy = (float)iy + 0.5f;

				float uc = cx * du;
				float vc = cy * dv;

				uvs[v0] = FVector2f(uc, vc);
				positions[v0] = FVector3f(cx * cellSize, cy * cellSize, 0);
				normals[v0] = FVector3f(0, 0, 1);

				triangles.AddTriangle(v0, v1, v2);
				triangles.AddTriangle(v0, v2, v3);
				triangles.AddTriangle(v0, v3, v4);
				triangles.AddTriangle(v0, v4, v1);
			}

			float u = ix * du;
			float v = iy * dv;

			uvs[v1] = FVector2f(u, v);
			positions[v1] = FVector3f(ix * cellSize, iy * cellSize, 0);
			normals[v1] = FVector3f(0, 0, 1);
		}
	}

	return description;
}

FMeshDescription Meshes::Cube(float size)
{
	FMeshDescription description;

	TArrayView<FVector3f> positions;
	TArrayView<FVector3f> normals;
	Meshes::Triangles triangles;

	Meshes::Init(description, 24, 8)
		.WithPositions(positions)
		.WithNormals(normals)
		.WithTriangles(triangles, 0);

	int index = 0;
	auto addSide = [&index, &triangles, &positions, &normals, size](FVector3f n, FVector3f u, FVector3f v) {

		int v0 = index;
		int v1 = index + 1;
		int v2 = index + 2;
		int v3 = index + 3;
		index += 4;

		triangles.AddQuad(v0, v1, v2, v3);

		positions[v0] = (n - u - v) * size;
		normals[v0] = n;

		positions[v1] = (n - u + v) * size;
		normals[v1] = n;

		positions[v2] = (n + u + v) * size;
		normals[v2] = n;

		positions[v3] = (n + u - v) * size;
		normals[v3] = n;
	};

	addSide(FVector3f(1, 0, 0), FVector3f(0, 1, 0), FVector3f(0, 0, 1));
	addSide(FVector3f(-1, 0, 0), FVector3f(0, 0, 1), FVector3f(0, 1, 0));
	addSide(FVector3f(0, 1, 0), FVector3f(0, 0, 1), FVector3f(1, 0, 0));
	addSide(FVector3f(0, -1, 0), FVector3f(1, 0, 0), FVector3f(0, 0, 1));
	addSide(FVector3f(0, 0, 1), FVector3f(1, 0, 0), FVector3f(0, 1, 0));
	addSide(FVector3f(0, 0, -1), FVector3f(0, 1, 0), FVector3f(1, 0, 0));

	return description;
}

FMeshDescription Meshes::Sphere(float radius, int density)
{
	density *= 2;
	int parallels = (density - 2) / 2;

	FMeshDescription description;

	TArrayView<FVector3f> positions;
	TArrayView<FVector3f> normals;
	Meshes::Triangles triangles;

	Meshes::Init(description, 2 + parallels * density, 2 * parallels * density)
		.WithPositions(positions)
		.WithNormals(normals)
		.WithTriangles(triangles, 0);

	int vertex = 0;

	float dv = PI / (parallels + 1);
	float fv = -PI / 2 + dv;

	float dt = 2 * PI / density;
	
	for (int v = 0; v < parallels; v++)
	{
		float fz = FGenericPlatformMath::Sin(fv);
		float fu = 0;
		float horizontal = FGenericPlatformMath::Sqrt(1 - fz * fz);

		for (int u = 0; u < density; u++)
		{
			if (v != parallels - 1) 
			{
				int v0 = vertex;
				int v1 = v0 + density;
				int v3 = (u == density - 1) ? v * density : vertex + 1;
				int v2 = v3 + density;

				triangles.AddQuad(v0, v1, v2, v3);
			}

			float fx = horizontal * FGenericPlatformMath::Cos(fu);
			float fy = horizontal * FGenericPlatformMath::Sin(fu);

			positions[vertex] = FVector3f(fx, fy, fz) * radius;
			normals[vertex] = FVector3f(fx, fy, fz);

			fu += dt;
			vertex++;
		}
		fv += dv;
	}

	int s0 = 0;
	int n0 = (parallels - 1) * density;

	int sPole = parallels * density;
	int nPole = sPole + 1;

	positions[sPole] = FVector3f(0, 0, -radius);
	normals[sPole] = FVector3f(0, 0, -1);

	positions[nPole] = FVector3f(0, 0, radius);
	normals[nPole] = FVector3f(0, 0, 1);

	for (int x = 0; x < density; x++, n0++, s0++)
	{
		int s1, n1;
		if (x == density - 1)
		{
			s1 = 0;
			n1 = (parallels - 1) * density;
		}
		else
		{
			s1 = s0 + 1;
			n1 = n0 + 1;
		}

		triangles.AddTriangle(sPole, s0, s1);
		triangles.AddTriangle(nPole, n1, n0);
	}

	return description;
}
