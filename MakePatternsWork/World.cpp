#include "stdafx.h"
#include "World.h"
#include "Mediator.h"
#include "Server.h"

World::World(){}
World::~World() { }

World* World::instance = 0;
World* World::Instance() {
	if (!instance)
	{
		instance = new World();
	}
	return instance;
}

void World::init() {
	types = new SettlerType*[4];
	types[0] = new SettlerType("BuilderModel", "BuilderTexture", "BuilderMaterial", Builder);
	types[1] = new SettlerType("CivilianModel", "CivilianTexture", "CivilianMaterial", Civilian);
	types[2] = new SettlerType("FighterModel", "FighterTexture", "FighterMaterial", Fighter);
	types[3] = new SettlerType("TraderModel", "TraderTexture", "TraderMaterial", Trader);

	grid = new Grid(10, 10);
	grid->buildGrid();

	createResourceGroups();

	Tool* fishingNet = new FishingNet();
	Tool* axe = new Axe();
	Settler settler = (*createNewSettler(Builder, Fisherman, Vector3(250, 0, 35), fishingNet));
	settler.id = 1;
	//Settler settler2 = (*createNewSettler(Builder, WoodChopper, Vector3(150, 0, 230), axe));
	settlers.push_back(settler);
	//settlers.push_back(settler2);

	sendUpdateDataToServer();
}

void World::update() {
	receiveUpdateDataFromServer();

	for (int i = 0; i < settlers.size(); ++i) {
		std::cout << "\nUpdating settler " << i << std::endl;
		settlers[i].update();

		// test for grid
		std::vector<WorldObject*> objectsAtPos = grid->getObjectsAtIndex(grid->worldToGrid(settlers[i].pos).x, grid->worldToGrid(settlers[i].pos).y);
		for (int j = 0; j < objectsAtPos.size(); ++j) {
			std::cout << "Located world object type " << objectsAtPos[j]->worldObjectType << " at this Settlers location" << std::endl;
		}

		//std::vector<Cell*> neighborCellsAtPos = grid->getCellAt(grid->worldToGrid(settlers[i].pos).x, grid->worldToGrid(settlers[i].pos).y)->getNeighbors();
		//Cell* currCell = grid->getCellAt(grid->worldToGrid(settlers[i].pos).x, grid->worldToGrid(settlers[i].pos).y);
		//std::cout << "Current cell: " << currCell->getPos().x << ", " << currCell->getPos().y << std::endl;
		//for (int j = 0; j < neighborCellsAtPos.size(); ++j) {
			//if (neighborCellsAtPos[j] == nullptr) continue;
			//std::cout << "Neighboring cell " << j << ": " << neighborCellsAtPos[j]->getPos().x << ", " << neighborCellsAtPos[j]->getPos().y << std::endl;
		//}
	}

	// update resource locations

	sendUpdateDataToServer();
}

void World::receiveUpdateDataFromServer() {
	std::vector<ProxySettler> proxySettlers = Server::receiveProxyObjectsFromServer();
	std::cout << "\nWorld received " << proxySettlers.size() << " proxy objects..." << std::endl;
	settlers.clear();
	for (int i = 0; i < proxySettlers.size(); ++i) {
		settlers.push_back(Settler::createSettler(proxySettlers[i]));
	}

	std::cout << "Converted message size in bytes: " << sizeof(std::vector<Settler>) + (sizeof(Settler) * proxySettlers.size()) << std::endl;
}

void World::sendUpdateDataToServer() {
	std::vector<ProxySettler> proxySettlers;

	for (int i = 0; i < settlers.size(); ++i) {
		proxySettlers.push_back(ProxySettler::createProxy(settlers[i]));	
		grid->getCellAt(grid->worldToGrid(settlers[i].pos).x, World::Instance()->grid->worldToGrid(settlers[i].pos).y)->removeObject(&settlers[i]);
	}

	Server::sendProxyObjectsToServer(proxySettlers);
}

Settler* World::createNewSettler(EnumSettlerType type, Profession prof, Vector3 worldPos, Tool* tool, ResourceGroup* rg) {
	switch (type) {
	case Builder:
		return new Settler(types[0], prof, worldPos, tool, rg);
	case Civilian:
		return new Settler(types[1], prof, worldPos, tool, rg);
	case Fighter:
		return new Settler(types[2], prof, worldPos, tool, rg);
	case Trader:
		return new Settler(types[3], prof, worldPos, tool, rg);
	}
}

void World::createResourceGroups() {
	Forest* forest1 = new Forest(Vector3(20, 0, 510), 15);
	forest1->id = 2;
	Forest* forest2 = new Forest(Vector3(400, 0, 330), 5);
	forest2->id = 3;
	Mediator::Instance()->forests.push_back(forest1);
	Mediator::Instance()->forests.push_back(forest2);

	FishingSpot* fishingSpot1 = new FishingSpot(Vector3(100, 0, 850), 8);
	fishingSpot1->id = 4;
	Mediator::Instance()->fishingSpots.push_back(fishingSpot1);
}