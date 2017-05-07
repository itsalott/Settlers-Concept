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

// ------------------ TOOLS ----------------------

class ITool {
public:
	virtual void useTool() {}
};

class IWeapon : ITool {
public:
	virtual int getFireRange() { return 0; }
	virtual int getPreciseness() { return 0; }
};

enum EnumToolTypes {
	WeaponTool = 0,
	AxeTool
};

class Tool : public ITool {
public:
	Tool(EnumToolTypes type) : type(type) {}
	~Tool() {}

protected: 
	EnumToolTypes type;

	void useTool() override {
		std::cout << "Basic tool used" << std::endl;
	}
};

class Weapon : public Tool, public IWeapon {
public:
	Weapon(): Tool(WeaponTool) {}
	~Weapon() {}

	int getFireRange() override { return 5; }
	int getPreciseness() override { return 10; }

	void useTool() override {
		std::cout << "Weapon used" << std::endl;
	}

protected:
	int fireRange;
	int preciseness;
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

class Settler {
public:
	Settler(SettlerType* pType, Tool* tool = nullptr) : type(pType), pos(Vector3(0, 0, 0)), currentTool(tool) {	}
	~Settler() { }
	
	SettlerType* type;
	void switchTool(Tool* newTool) {
		currentTool = newTool;
	}

private:
	Vector3 pos;
	Tool* currentTool;
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

	Settler CreateNewSettler(EnumSettlerType type, Tool* tool = nullptr) {
		switch(type) {
		case Builder:
			return Settler(types[0], tool);
			break;
		case Civilian:
			return Settler(types[1], tool);
			break;
		case Fighter:
			return Settler(types[2], tool);
			break;
		case Trader:
			return Settler(types[3], tool);
			break;
		}
	}

private:
	SettlerType** types;
};

int main()
{
	World* world = new World();

	IWeapon *weapon = new Weapon();
	std::cout << "Basic weapon has " << weapon->getPreciseness() << " preciseness and " << weapon->getFireRange() << " fire range." << std::endl;

	weapon = new GunpowderUpgrade(weapon);
	std::cout << "Weapon upgraded with gunpowder has " << weapon->getPreciseness() << " preciseness and " << weapon->getFireRange() << " fire range." << std::endl;

	weapon = new BarrelUpgrade(weapon);
	std::cout << "Weapon upgraded with barrel has " << weapon->getPreciseness() << " preciseness and " << weapon->getFireRange() << " fire range." << std::endl;

	

	Settler settler = world->CreateNewSettler(Builder);
	Settler settler2 = world->CreateNewSettler(Builder);
	//std::cout <<  "I'm a Settler of type " << settler.type << std::endl;
	//std::cout <<  "I'm a Settler of type " << settler2.type << std::endl;

	char c;
	std::cin >> c;

    return 0;
}



