#ifndef GRID_H
#define GRID_H

#include "Helpers.h"
#include "WorldObjects.h"
#include <vector>

class Cell {
	friend class Grid;

public:
	Cell();
	Cell(int x, int y);
	~Cell() {}

	bool isOccupied() { return objectsInCell.size() == 0 ? false : true; }
	std::vector<WorldObject*> getAllObjects() { return objectsInCell; }

	void addObject(WorldObject* object);
	void removeObject(WorldObject* object);
	void findNeighbors();
	std::vector<Cell*> getNeighbors();
	Vector2 getPos() { return Vector2(x, y); }

private:
	std::vector<WorldObject*> objectsInCell;
	int x, y;
	std::vector<Cell*> neighbors;
};

class Grid {
public:
	Grid(int width, int height);
	~Grid() { }

	unsigned int width, height, cellSize;
	Cell* cells;
	Vector3 originInWorld;

	void buildGrid();
	Vector3 gridToWorld(int x, int y);
	Vector2 worldToGrid(Vector3 pos);

	Cell* getCellAt(int x, int y);

	std::vector<WorldObject*> getObjectsAtIndex(int x, int y);
	void addObjectToGrid(WorldObject* object);
	void updateObjectInGrid(WorldObject* object);
};

#endif