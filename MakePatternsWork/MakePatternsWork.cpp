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
	int harvest() {
		std::cout << "Current amount of resources: " << resourcesLeft << std::endl;
		resourcesLeft--; 
		return 1;
	}
	Vector3 pos;

private:
	int resourcesLeft;
};

class Forest : public ResourceGroup{
public:
	Forest() : ResourceGroup() {}
	~Forest() {}
};

class FishingSpot : public ResourceGroup{
public:
	FishingSpot() : ResourceGroup() {}
	~FishingSpot() {}
};

class Mediator{
public:
	static Mediator* Instance();
	std::list<Forest*> forests;
	std::list<FishingSpot*> fishingSpots;

	Forest* getBestForest(Vector3 settlerPos){
		Forest* nearestForest = nullptr;
		float distance = 1000;

		std::list<Forest*>::iterator it;
		for (it = forests.begin(); it != forests.end(); it++)
		{
			float newDistance = distanceBetweenVectors((*it)->pos, settlerPos);
			if (newDistance < distance && !(*it)->isEmpty()){
				distance = newDistance;
				nearestForest = *it;
			}
		}

		return nearestForest;
	}

	FishingSpot* getBestFishingSpot(Vector3 settlerPos){
		FishingSpot* nearestFishingSpot = nullptr;
		float distance = 1000;

		std::list<FishingSpot*>::iterator it;
		for (it = fishingSpots.begin(); it != fishingSpots.end(); it++)
		{
			float newDistance = distanceBetweenVectors((*it)->pos, settlerPos);
			if (newDistance < distance && !(*it)->isEmpty()){
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
	static Mediator* instance;
	Mediator() {}
	~Mediator() {}
};

Mediator* Mediator::instance = 0;
Mediator* Mediator::Instance() {
	if (!instance)
	{
		instance = new Mediator();
	}
	return instance;
}


class Settler {
public:
	Settler(SettlerType* pType) : type(pType), pos(Vector3(0, 0, 0)), professionType("woodChopper") {	}
	~Settler() { }
	SettlerType* type;

	void Update(){
		if (currentRG == nullptr || currentRG->isEmpty()) findNewResourceGroup();
		else currentRG->harvest();
	}

private:
	Vector3 pos;

	std::string professionType;
	ResourceGroup* currentRG;

	void findNewResourceGroup(){
		if (professionType == "woodChopper")
		{
			currentRG = Mediator::Instance()->getBestForest(pos);
			std::cout << "Found new resource group: " << currentRG->isEmpty() << std::endl;
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

	Forest* forest1 = new Forest();
	Forest* forest2 = new Forest();
	Mediator::Instance()->forests.push_back(forest1);
	Mediator::Instance()->forests.push_back(forest2);

	Settler settler = world->CreateNewSettler(Builder);
	
	//Settler settler2 = world->CreateNewSettler(Builder);
	//std::cout <<  "I'm a Settler of type " << settler.type << std::endl;
	//std::cout <<  "I'm a Settler of type " << settler2.type << std::endl;

	for(int i = 0; i < 16; ++i) {
		char c;
		std::cout << "Press Enter" << std::endl;
		std::cin >> c;
		settler.Update();
	}

    return 0;
}



