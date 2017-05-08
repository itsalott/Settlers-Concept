#include "stdafx.h"
#include "Mediator.h"

Mediator* Mediator::instance = 0;
Mediator* Mediator::Instance() {
	if (!instance)
	{
		instance = new Mediator();
	}
	return instance;
}

Forest* Mediator::getBestForest(Vector3 settlerPos) {
	Forest* nearestForest = nullptr;
	float distance = 1000;

	std::vector<Forest*>::iterator it;
	for (it = forests.begin(); it != forests.end(); ++it)
	{
		float newDistance = distanceBetweenVectors((*it)->pos, settlerPos);
		if (newDistance < distance && !(*it)->isEmpty()) {
			distance = newDistance;
			nearestForest = *it;
		}
	}

	return nearestForest;
}

FishingSpot* Mediator::getBestFishingSpot(Vector3 settlerPos) {
	FishingSpot* nearestFishingSpot = nullptr;
	float distance = 1000;

	std::vector<FishingSpot*>::iterator it;
	for (it = fishingSpots.begin(); it != fishingSpots.end(); ++it)
	{
		float newDistance = distanceBetweenVectors((*it)->pos, settlerPos);
		if (newDistance < distance && !(*it)->isEmpty()) {
			distance = newDistance;
			nearestFishingSpot = *it;
		}
	}

	return nearestFishingSpot;
}