# Unreal Mesh Creator
Simple and clean mesh creation for Unreal Engine

### Proxy for FMeshDescription
Provides you with with more compact syntax along with fluent and intuitive interface for creating mesh

All you need to do is fill in data you want
```cpp
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
	
	// triangles.AddTriangle(0, 1, 3);
	// triangles.AddTriangle(1, 3, 2);

	return description;
}
```

### Usage with UStaticMeshComponetn with Nanite enabled

```cpp
// Sets default values
AQuad::AQuad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
}

// Called when the game starts or when spawned
void AQuad::BeginPlay()
{
	Super::BeginPlay();

	UStaticMesh::FBuildMeshDescriptionsParams mdParams;
	mdParams.bBuildSimpleCollision = true;

	FMeshDescription description = Meshes::SimpleQuad(); // replace it with any mesh you want

	UStaticMesh* mesh = NewObject<UStaticMesh>(this, FName(TEXT("mesh")));
	mesh->GetStaticMaterials().Add(FStaticMaterial());
	mesh->NaniteSettings.bEnabled = true;
	mesh->BuildFromMeshDescriptions({ &description }, mdParams);

	_meshComponent->SetStaticMesh(mesh);
}
```
