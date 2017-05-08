#include "stdafx.h"
#include "World.h"
#include "Mediator.h"

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
	Settler* settler = createNewSettler(Builder, Fisherman, Vector3(250, 0, 35), fishingNet);
	Settler* settler2 = createNewSettler(Builder, WoodChopper, Vector3(150, 0, 230), axe);
	settlers.push_back(settler);
	settlers.push_back(settler2);
}

void World::update() {
	for (int i = 0; i < settlers.size(); ++i) {
		std::cout << "\nUpdating settler " << i << std::endl;
		settlers[i]->update();

		// test for grid
		std::vector<WorldObject*> objectsAtPos = grid->getObjectsAtIndex(grid->worldToGrid(settlers[i]->pos).x, grid->worldToGrid(settlers[i]->pos).y);
		for (int j = 0; j < objectsAtPos.size(); ++j) {
			std::cout << "Located world object type " << objectsAtPos[j]->worldObjectType << " at this Settlers location" << std::endl;
		}

		std::vector<Cell*> neighborCellsAtPos = grid->getCellAt(grid->worldToGrid(settlers[i]->pos).x, grid->worldToGrid(settlers[i]->pos).y)->getNeighbors();
		Cell* currCell = grid->getCellAt(grid->worldToGrid(settlers[i]->pos).x, grid->worldToGrid(settlers[i]->pos).y);
		std::cout << "Current cell: " << currCell->getPos().x << ", " << currCell->getPos().y << std::endl;
		for (int j = 0; j < neighborCellsAtPos.size(); ++j) {
			if (neighborCellsAtPos[j] == nullptr) continue;
			std::cout << "Neighboring cell " << j << ": " << neighborCellsAtPos[j]->getPos().x << ", " << neighborCellsAtPos[j]->getPos().y << std::endl;
		}
	}

	// update resource locations
}

Settler* World::createNewSettler(EnumSettlerType type, Profession prof, Vector3 worldPos, Tool* tool) {
	switch (type) {
	case Builder:
		return new Settler(types[0], prof, worldPos, tool);
	case Civilian:
		return new Settler(types[1], prof, worldPos, tool);
	case Fighter:
		return new Settler(types[2], prof, worldPos, tool);
	case Trader:
		return new Settler(types[3], prof, worldPos, tool);
	}
}

void World::createResourceGroups() {
	Forest* forest1 = new Forest(Vector3(20, 0, 510), 15);
	Forest* forest2 = new Forest(Vector3(400, 0, 330), 5);
	Mediator::Instance()->forests.push_back(forest1);
	Mediator::Instance()->forests.push_back(forest2);

	FishingSpot* fishingSpot1 = new FishingSpot(Vector3(100, 0, 850), 8);
	Mediator::Instance()->fishingSpots.push_back(fishingSpot1);
}