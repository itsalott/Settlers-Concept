#pragma once
#include <vector>
#include "WorldObjects.h"

class Mediator {
public:
	static Mediator* Instance();
	std::vector<Forest*> forests;
	std::vector<FishingSpot*> fishingSpots;

	Forest* getBestForest(Vector3 settlerPos);
	FishingSpot* getBestFishingSpot(Vector3 settlerPos);

	float distanceBetweenVectors(Vector3 a, Vector3 b)
	{
		return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
	}

private:
	static Mediator* instance;
	Mediator() {}
	~Mediator() {}
};


