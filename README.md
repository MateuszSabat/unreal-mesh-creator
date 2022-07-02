# Unreal Mesh Creator
Simple and clean mesh creation for Unreal Engine

### Proxy for FMeshDescription
Provides you with with more compact syntax along with fluent and intuitive interface for creating mesh

All you need to do is fill in data you want
```cpp
FMeshDescription SimpleQuad()
{
	FMeshDescription description;
	FStaticMeshAttributes(description).Register();

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

	positions[0] = FVector3f(-1, -1, 0);
	positions[1] = FVector3f(1, -1, 0);
	positions[2] = FVector3f(-1, 1, 0);
	positions[3] = FVector3f(1, 1, 0);
  
	normals[0] = FVector3f(0, 0, 1);
	normals[1] = FVector3f(0, 0, 1);
	normals[2] = FVector3f(0, 0, 1);
	normals[3] = FVector3f(0, 0, 1);
  
	uvs[0] = FVector2f(0, 0);
	uvs[1] = FVector2f(1, 0);
	uvs[2] = FVector2f(0, 1);
	uvs[3] = FVector2f(1, 1);

	triangles.AddQuad(0, 1, 2, 3);

	// the same as:
  
	// triangles.AddTriangle(0, 1, 2);
	// triangles.AddTriangle(2, 3, 0);
}
```

### Usage with UStaticMeshComponetn with Nanite enabled

```cpp
_meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

UStaticMesh::FBuildMeshDescriptionsParams mdParams;
mdParams.bBuildSimpleCollision = true;

FMeshDescription description = StaticMeshes::SteinerGrid(20, 20, 50); // or your custom function

UStaticMesh* mesh = NewObject<UStaticMesh>(this, FName(TEXT("mesh")));
mesh->GetStaticMaterials().Add(FStaticMaterial());
mesh->NaniteSettings.bEnabled = true;
mesh->BuildFromMeshDescriptions({&description}, mdParams);

_meshComponent->SetStaticMesh(mesh);
```
