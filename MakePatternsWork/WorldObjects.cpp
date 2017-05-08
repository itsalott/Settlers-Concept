#include "stdafx.h"
#include "WorldObjects.h"
#include "Mediator.h"
#include "World.h"

WorldObject::WorldObject() : pos(0, 0, 0) {
	previousPos = pos;
}

void WorldObject::update() { }

void WorldObject::updateGridPos(Vector2 oldCell) {
	Vector2 newCell = World::Instance()->grid->worldToGrid(pos);

	World::Instance()->grid->updateObjectInGrid(this);
}

void WorldObject::addToGrid() { 
	World::Instance()->grid->addObjectToGrid(this);
}

ResourceGroup::ResourceGroup() : resourcesLeft(10) {
	this->pos = Vector3(0, 0, 0);
	worldObjectType = ResourceObject;
	addToGrid();
}

ResourceGroup::ResourceGroup(Vector3 pos, int resourceAmount) : resourcesLeft(resourceAmount) {
	this->pos = pos;
	worldObjectType = ResourceObject;
	addToGrid();
}

bool ResourceGroup::isEmpty() { return resourcesLeft <= 0; }

int ResourceGroup::harvest() {
	std::cout << "Current amount of resources: " << resourcesLeft << std::endl;
	resourcesLeft--;
	return 1;
}

void ResourceGroup::update() {
	// for example: chance to respawn resources.
}

Settler::Settler(SettlerType* pType, Profession prof, Vector3 worldPos, Tool* tool)
: type(pType), currentTool(tool), professionType(prof) {
	pos = worldPos;
	previousPos = pos;
	worldObjectType = SettlerObject;
	addToGrid();
}

void Settler::update() {
	if (currentTool == nullptr) {
		std::cout << " >> I don't have a tool, so I can't work" << std::endl;
		return;
	}
	if (currentRG == nullptr || currentRG->isEmpty()) {
		std::cout << " >> I don't have a valid resource group right now" << std::endl;
		findNewResourceGroup();
	}
	else if(pos.x != currentRG->pos.x && pos.z != currentRG->pos.z) {
		// not at resource location right now
		move();
	}
	else {
		std::cout << " >> I'm working" << std::endl;
		currentTool->useTool();
		currentRG->harvest();
	}
}

void Settler::move() {
	previousPos = pos;
	pos = currentRG->pos;

	std::cout << " >> I moved to my current resource group" << std::endl;
	std::cout << "Move (in world) performed from: [" << previousPos.x << "," << previousPos.z << "] to [" << pos.x << ", " << pos.z << "]" << std::endl;
	std::cout << "Move (in grid) performed from: [" << World::Instance()->grid->worldToGrid(previousPos).x << "," << World::Instance()->grid->worldToGrid(previousPos).y << "] to [" << World::Instance()->grid->worldToGrid(pos).x << ", " << World::Instance()->grid->worldToGrid(pos).y << "]" << std::endl;
	updateGridPos(World::Instance()->grid->worldToGrid(previousPos));
}

void Settler::switchTool(Tool* newTool) {
	currentTool = newTool;
}

void Settler::findNewResourceGroup() {
	if ((int)professionType == 0)
	{
		currentRG = Mediator::Instance()->getBestForest(pos);
	}
	else if ((int)professionType == 1)
	{
		currentRG = Mediator::Instance()->getBestFishingSpot(pos);
	}

	if(currentRG == nullptr) {
		std::cout << " >> I'm out of work" << std::endl;
	} 
	else {
		std::cout << " >> I found a new resource group" << std::endl;
	}
}