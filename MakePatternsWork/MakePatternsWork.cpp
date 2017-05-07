// MakePatternsWork.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <list>
#include <cmath>

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

class ResourceGroup{
public:
	ResourceGroup() : pos(0, 0, 0)
	{ 
		resourcesLeft = 10;
	}
	~ResourceGroup() {}
	bool isEmpty(){ return resourcesLeft <= 0; }
	int harvest(){ resourcesLeft--; return 1; }
	Vector3 pos;

private:
	int resourcesLeft;
};

class Forest : public ResourceGroup{
public:
	Forest() {}
	~Forest() {}
};

class FishingSpot : public ResourceGroup{
public:
	FishingSpot() {}
	~FishingSpot() {}
};

class Mediator{
public:
	static Mediator* Instance(){ 
		if (instance == nullptr)
		{
			instance = new Mediator();
		}
		return instance;
	}
	std::list<Forest*> forests;
	std::list<FishingSpot*> fishingSpots;

	Forest* getBestForest(Vector3 settlerPos){
		Forest* nearestForest;
		float distance = 1000;

		std::list<Forest*>::iterator it;
		for (it = forests.begin(); it != forests.end(); it++)
		{
			float newDistance = distanceBetweenVectors((*it)->pos, settlerPos);
			if (newDistance < distance){
				distance = newDistance;
				nearestForest = *it;
			}
		}

		return nearestForest;
	}

	FishingSpot* getBestFishingSpot(Vector3 settlerPos){
		FishingSpot* nearestFishingSpot;
		float distance = 1000;

		std::list<FishingSpot*>::iterator it;
		for (it = fishingSpots.begin(); it != fishingSpots.end(); it++)
		{
			float newDistance = distanceBetweenVectors((*it)->pos, settlerPos);
			if (newDistance < distance){
				distance = newDistance;
				nearestFishingSpot = *it;
			}
		}

		return nearestFishingSpot;
	}
	
	float distanceBetweenVectors(Vector3 a, Vector3 b)
	{
		return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
	}
private:
	Mediator() {}
	~Mediator() {}
	static Mediator* instance;
};

class Settler {
public:
	Settler(SettlerType* pType) : type(pType), pos(Vector3(0, 0, 0)) {	}
	~Settler() { }
	SettlerType* type;

	void Update(){

	}

private:
	Vector3 pos;

	ResourceGroup* currentRG;
	std::string professionType;

	void findNewResourceGroup(){
		if (professionType == "woodChopper")
		{
			currentRG = Mediator::Instance()->getBestForest(pos);
		}
		else
		{
			currentRG = Mediator::Instance()->getBestFishingSpot(pos);
		}
	}
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



