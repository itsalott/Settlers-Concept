// MakePatternsWork.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

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
	~Vector3() {}
};

enum EnumSettlerType {
	Builder = 0,
	Civilian,
	Fighter,
	Trader
};

class SettlerType {
public:
	Mesh model;
	Texture2D texture;
	Material material;
	EnumSettlerType type;
	
	SettlerType() : model(""), texture(""), material(""), type((EnumSettlerType)0) {}
	SettlerType(Mesh pModel, Texture2D pTexture, Material pMaterial, EnumSettlerType pType) : model(pModel), texture(pTexture), material(pMaterial), type(pType) {}
	~SettlerType() {}
};

class Settler {
public:
	Settler(SettlerType* pType) : type(pType), pos(Vector3(0, 0, 0)) {	}
	~Settler() { }
	SettlerType* type;

private:
	Vector3 pos;
};

class World {
public:
	World() {
		types = new SettlerType*[4];
		types[0] = new SettlerType("BuilderModel", "BuilderTexture", "BuilderMaterial", Builder);
		types[1] = new SettlerType("CivilianModel", "CivilianTexture", "CivilianMaterial", Civilian);
		types[2] = new SettlerType("FighterModel", "FighterTexture", "FighterMaterial", Fighter);
		types[3] = new SettlerType("TraderModel", "TraderTexture", "TraderMaterial", Trader);
	}

	Settler CreateNewSettler(EnumSettlerType type) {
		switch(type) {
		case Builder:
			return Settler(types[0]);
			break;
		case Civilian:
			return Settler(types[1]);
			break;
		case Fighter:
			return Settler(types[2]);
			break;
		case Trader:
			return Settler(types[3]);
			break;
		}
	}

private:
	SettlerType** types;
};

int main()
{
	World* world = new World();

	Settler settler = world->CreateNewSettler(Builder);
	Settler settler2 = world->CreateNewSettler(Builder);
	std::cout <<  "I'm a Settler of type " << settler.type << std::endl;
	std::cout <<  "I'm a Settler of type " << settler2.type << std::endl;

	char c;
	std::cin >> c;

    return 0;
}



