#ifndef HELPERS_H
#define HELPERS_H

class Mesh {
public:
	const char* meshName;

	Mesh(const char* pMeshName) : meshName(pMeshName) {}
	~Mesh() {}
};

class Texture2D {
public:
	const char* textureName;

	Texture2D(const char* pTextureName) : textureName(pTextureName) {}
	~Texture2D() {}
};

class Material {
public:
	const char* materialName;

	Material(const char* pMaterialName) : materialName(pMaterialName) {}
	~Material() {}
};

class Vector3 {
public:
	float x, y, z;

	Vector3(float pX, float pY, float pZ) : x(pX), y(pY), z(pZ) {}
	Vector3() {}
	~Vector3() {}
};

class Vector2 {
public:
	float x, y;

	Vector2(float pX, float pY) : x(pX), y(pY) {}
	~Vector2() {}
};

enum EnumSettlerType {
	Builder = 0,
	Civilian,
	Fighter,
	Trader
};

enum Profession {
	WoodChopper = 0,
	Fisherman
};

#endif
