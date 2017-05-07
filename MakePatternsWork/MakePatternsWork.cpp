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
	
	void update() {
		if (currentTool == nullptr) return;

		currentTool->useTool();
	}

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

	Tool* weapon = new Weapon();
	Tool* axe = new Axe();

	Settler settler = world->CreateNewSettler(Builder, weapon);
	Settler settler2 = world->CreateNewSettler(Builder, axe);

	for(int i = 0; i < 3; ++i) {
		settler.update();
		settler2.update();

		char c;
		std::cin >> c;
	}

	settler.switchTool(axe);
	settler2.switchTool(weapon);

	std::cout << "switched the two tools" << std::endl;

	for (int i = 0; i < 3; ++i) {
		settler.update();
		settler2.update();

		char c;
		std::cin >> c;
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



