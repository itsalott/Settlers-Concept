#pragma once
#ifndef WORLD_OBJS_H
#define WORLD_OBJS_H

#include "Helpers.h"
#include "Tools.h"

class Settler;

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

enum WorldObjecType {
	ResourceObject,
	SettlerObject
};

class WorldObject {
public:
	WorldObject();

	int id;
	Vector3 previousPos;
	Vector3 pos;
	WorldObjecType worldObjectType;

protected:
	virtual void update();
	virtual void updateGridPos(Vector2 oldCell);
	virtual void addToGrid();
};

class ResourceGroup : public WorldObject {
public:
	ResourceGroup();
	ResourceGroup(Vector3 pos, int resourceAmount);
	~ResourceGroup() {}

	bool isEmpty();
	int harvest();

protected:
	void update() override;

private:
	int resourcesLeft;
};

class Forest : public ResourceGroup {
public:
	Forest() : ResourceGroup() {}
	Forest(Vector3 pos, int resourceAmount) : ResourceGroup(pos, resourceAmount) {}
	~Forest() {}
};

class FishingSpot : public ResourceGroup {
public:
	FishingSpot() : ResourceGroup() {}
	FishingSpot(Vector3 pos, int resourceAmount) : ResourceGroup(pos, resourceAmount) {}
	~FishingSpot() {}
};

class ISettler {
public:
	void update();
	void switchTool(Tool* newTool);

protected:
	void findNewResourceGroup();
	void move();
};

class ProxySettler : public ISettler{
	friend class Settler;
public:
	ProxySettler(EnumSettlerType pType, Profession prof, Vector3 worldPos, Tool* tool = nullptr, ResourceGroup* rg = nullptr);
	~ProxySettler() { }

	static ProxySettler createProxy(Settler settler);

	EnumSettlerType type;
	Vector3 pos;
private:
	Tool* currentTool;				// should be proxytool
	Profession professionType;
	ResourceGroup* currentRG;
};

class Settler : public WorldObject, public ISettler {
	friend class ProxySettler;
public:
	Settler(SettlerType* pType, Profession prof, Vector3 worldPos, Tool* tool = nullptr, ResourceGroup* rg = nullptr);
	~Settler();

	static Settler createSettler(ProxySettler settler);

	SettlerType* type;

	void update();
	void switchTool(Tool* newTool);

private:
	Tool* currentTool;
	Profession professionType;
	ResourceGroup* currentRG;

	const int speed = 10;

	void findNewResourceGroup();
	void move();
};
#endif
