#pragma once

#include "Field.h"
#include <vector>
#include "../util/const.h"
#include "../util/Buffer.h"
#include "../util/Vect2D.h"
#include <Imagine/Graphics.h>

enum TerrainType {
	TERRAIN_MEADOW, TERRAIN_FOREST, TERRAIN_RIVER
};

Imagine::AlphaColor getTerrainTypeColor(TerrainType terrainType);

class Map {
private:
	int width, height;
	std::vector<TerrainType> terrainType;
	std::vector<Field> fields;
public:
	Map(int width, int height);

	void setTerrainType(int x, int y, TerrainType terrainType);

	TerrainType getTerrainType(int x, int y);

	void addField(Field field);

	bool isField(int x, int y);

	Field getField(int x, int y);

	void draw(Buffer &buffer, Vect2D viewOffsetY, Vect2D minXRender, Vect2D maxXRender);

	const int getWidth();

	const int getHeight();
};