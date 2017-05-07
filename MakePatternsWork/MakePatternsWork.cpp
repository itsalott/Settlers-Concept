// MakePatternsWork.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <list>

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

// ------------------ TOOLS ----------------------

class ITool {
public:
	virtual void useTool() {}
};

class ToolImp : public ITool {
public:
	ToolImp() { }

	virtual void useTool() override {
		std::cout << "I don't do anything" << std::endl;
	}
};

class WeaponImp : public ToolImp {
public:
	WeaponImp() { }

	void useTool() override {
		std::cout << "Attacked" << std::endl;
	}
};

class AxeImp : public ToolImp {
public:
	AxeImp() { }

	void useTool() override {
		std::cout << "Harvested wood" << std::endl;
	}
};

class FishingNetImp : public ToolImp {
public:
	FishingNetImp() { }

	void useTool() override {
		std::cout << "Catched fish" << std::endl;
	}
};

class IWeapon : ITool {
public:
	virtual int getFireRange() { return 0; }
	virtual int getPreciseness() { return 0; }
};

class Tool : public ITool {
public:
	Tool() { imp = new ToolImp(); }
	~Tool() {}
	
	void useTool() override {
		imp->useTool();
	}

protected: 
	ToolImp* imp;
};

class Weapon : public Tool, public IWeapon {
public:
	Weapon() { imp = new WeaponImp(); }
	~Weapon() {}

	int getFireRange() override { return fireRange; }
	int getPreciseness() override { return preciseness; }

protected:
	int fireRange = 5;
	int preciseness = 10;
};

class Axe : public Tool {
public:
	Axe() { imp = new AxeImp(); }
	~Axe() {}
};

class FishingNet : public Tool {
public:
	FishingNet() { imp = new FishingNetImp(); }
	~FishingNet() {}
};

class WeaponUpgrade : public IWeapon {
public:
	WeaponUpgrade(IWeapon *baseWeapon) : weapon(baseWeapon) {}
	~WeaponUpgrade() {}

	int getFireRange() override{
		return weapon->getFireRange();
	}

	int getPreciseness() override {
		return weapon->getPreciseness();
	}

protected:
	IWeapon* weapon;
};

class GunpowderUpgrade : public WeaponUpgrade {
public:
	GunpowderUpgrade(IWeapon *baseWeapon) : WeaponUpgrade(baseWeapon) {}
	~GunpowderUpgrade() {}

	int getFireRange() override{
		//std::cout << weapon.getFireRange() << std::endl;
		return WeaponUpgrade::getFireRange() + 10;
	}
};

class BarrelUpgrade : public WeaponUpgrade {
public:
	BarrelUpgrade(IWeapon *baseWeapon) : WeaponUpgrade(baseWeapon) {}
	~BarrelUpgrade() {}

	int getPreciseness() override {
		return WeaponUpgrade::getPreciseness() + 8;
	}
};

// ------------------ END TOOLS -------------------

// ---------- RESOURCE GROUPS -------------------

class ResourceGroup {
public:
	ResourceGroup() : pos(0, 0, 0)
	{
		resourcesLeft = 10;
	}
	~ResourceGroup() {}
	bool isEmpty() { return resourcesLeft <= 0; }
	int harvest() {
		std::cout << "Current amount of resources: " << resourcesLeft << std::endl;
		resourcesLeft--;
		return 1;
	}
	Vector3 pos;

private:
	int resourcesLeft;
};

class Forest : public ResourceGroup {
public:
	Forest() : ResourceGroup() {}
	~Forest() {}
};

class FishingSpot : public ResourceGroup {
public:
	FishingSpot() : ResourceGroup() {}
	~FishingSpot() {}
};

class Mediator {
public:
	static Mediator* Instance();
	std::list<Forest*> forests;
	std::list<FishingSpot*> fishingSpots;

	Forest* getBestForest(Vector3 settlerPos) {
		Forest* nearestForest = nullptr;
		float distance = 1000;

		std::list<Forest*>::iterator it;
		for (it = forests.begin(); it != forests.end(); it++)
		{
			float newDistance = distanceBetweenVectors((*it)->pos, settlerPos);
			if (newDistance < distance && !(*it)->isEmpty()) {
				distance = newDistance;
				nearestForest = *it;
			}
		}

		return nearestForest;
	}

	FishingSpot* getBestFishingSpot(Vector3 settlerPos) {
		FishingSpot* nearestFishingSpot = nullptr;
		float distance = 1000;

		std::list<FishingSpot*>::iterator it;
		for (it = fishingSpots.begin(); it != fishingSpots.end(); it++)
		{
			float newDistance = distanceBetweenVectors((*it)->pos, settlerPos);
			if (newDistance < distance && !(*it)->isEmpty()) {
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

// ---------------------------------------------------------------------

enum Profession {
	WoodChopper = 0,
	Fisherman
};

class Settler {
public:
	Settler(SettlerType* pType, Profession prof, Tool* tool = nullptr) : type(pType), pos(Vector3(0, 0, 0)), currentTool(tool), professionType(prof) {	}
	~Settler() { }
	
	SettlerType* type;
	
	void update() {
		if (currentTool == nullptr) return;

		currentTool->useTool();
		if (currentRG == nullptr || currentRG->isEmpty()) findNewResourceGroup();
		else currentRG->harvest();
	}

	void switchTool(Tool* newTool) {
		currentTool = newTool;
	}

private:
	Vector3 pos;
	Tool* currentTool;

	Profession professionType;
	ResourceGroup* currentRG;

	void findNewResourceGroup() {
		if ((int)professionType == 0)
		{
			currentRG = Mediator::Instance()->getBestForest(pos);
			//std::cout << "Found new resource group: " << currentRG->isEmpty() << std::endl;
		}
		else if((int)professionType == 1)
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

	Settler CreateNewSettler(EnumSettlerType type, Profession prof, Tool* tool = nullptr) {
		switch(type) {
		case Builder:
			return Settler(types[0], prof, tool);
			break;
		case Civilian:
			return Settler(types[1], prof, tool);
			break;
		case Fighter:
			return Settler(types[2], prof, tool);
			break;
		case Trader:
			return Settler(types[3], prof, tool);
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

	Tool* fishingNet = new FishingNet();
	Tool* axe = new Axe();

	Settler settler = world->CreateNewSettler(Builder, Fisherman, fishingNet);
	Settler settler2 = world->CreateNewSettler(Builder, WoodChopper, axe);

	for(int i = 0; i < 30; ++i) {
		char c;
		std::cout << "Press Enter" << std::endl;
		std::cin >> c;

		settler.update();
		settler2.update();
	}

	//IWeapon *weapon = new Weapon();
	//std::cout << "Basic weapon has " << weapon->getPreciseness() << " preciseness and " << weapon->getFireRange() << " fire range." << std::endl;

	//weapon = new GunpowderUpgrade(weapon);
	//std::cout << "Weapon upgraded with gunpowder has " << weapon->getPreciseness() << " preciseness and " << weapon->getFireRange() << " fire range." << std::endl;

	//weapon = new BarrelUpgrade(weapon);
	//std::cout << "Weapon upgraded with barrel has " << weapon->getPreciseness() << " preciseness and " << weapon->getFireRange() << " fire range." << std::endl;

	//std::cout <<  "I'm a Settler of type " << settler.type << std::endl;
	//std::cout <<  "I'm a Settler of type " << settler2.type << std::endl;

    return 0;
}



