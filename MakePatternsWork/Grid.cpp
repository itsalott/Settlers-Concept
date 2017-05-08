#include "stdafx.h"
#include "Grid.h"
#include "World.h"

Cell::Cell() : x(0), y(0) { }

Cell::Cell(int x, int y) : x(x), y(y) { }

void Cell::addObject(WorldObject* object) {
	objectsInCell.push_back(object);
}

void Cell::removeObject(WorldObject* object) {
	int index;
	for (int i = 0; i < objectsInCell.size(); ++i) {
		if (objectsInCell[i] == object) {
			index = i;
			objectsInCell.erase(objectsInCell.begin() + index);
			break;
		}
	}
}

void Cell::findNeighbors() {
	neighbors.push_back(World::Instance()->grid->getCellAt(x + 1, y));
	neighbors.push_back(World::Instance()->grid->getCellAt(x - 1, y));
	neighbors.push_back(World::Instance()->grid->getCellAt(x, y + 1));
	neighbors.push_back(World::Instance()->grid->getCellAt(x, y - 1));
	
	neighbors.push_back(World::Instance()->grid->getCellAt(x + 1, y + 1));
	neighbors.push_back(World::Instance()->grid->getCellAt(x + 1, y - 1));
	neighbors.push_back(World::Instance()->grid->getCellAt(x - 1, y + 1));
	neighbors.push_back(World::Instance()->grid->getCellAt(x - 1, y - 1));
}

std::vector<Cell*> Cell::getNeighbors() {
	return neighbors;
}

Grid::Grid(int width, int height) : width(width), height(height), originInWorld(0, 0, 0) {
	cellsize = 100;
	cells = new Cell[width * height];
}

void Grid::buildGrid() {
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			cells[i * height + j].x = i;
			cells[i * height + j].y = j;
		}
	}

	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			cells[i * height + j].findNeighbors();
		}
	}
}

Vector3 Grid::gridToWorld(int x, int y) {
	return Vector3(x * cellsize, 0, y * cellsize);
}

Vector2 Grid::worldToGrid(Vector3 pos) {
	return Vector2((int)(pos.x / cellsize), (int)(pos.z / cellsize));
}

Cell* Grid::getCellAt(int x, int y) {
	if (x < 0 || x >= width || y < 0 || y >= height) return nullptr;

	return &cells[x * height + y];
}

std::vector<WorldObject*> Grid::getObjectsAtIndex(int x, int y) {
	return cells[x * height + y].objectsInCell;
}

void Grid::addObjectToGrid(WorldObject* object) {
	Vector2 gridPos = worldToGrid(object->pos);

	cells[(int)(gridPos.x * height + gridPos.y)].addObject(object);
}

void Grid::updateObjectInGrid(WorldObject* object) {
	Vector2 oldCell = worldToGrid(object->previousPos);
	Vector2 newCell = worldToGrid(object->pos);

	cells[(int)(oldCell.x * height + oldCell.y)].removeObject(object);
	cells[(int)(newCell.x * height + newCell.y)].addObject(object);
}