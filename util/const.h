#pragma once

const int MAP_SQUARE_PIXEL_SIZE = 32;
const int WINDOW_WIDTH = 640, WINDOW_HEIGHT = 496;
const int ACTION_BAR_HEIGHT = 100;
const int MAP_VIEW_WIDTH = WINDOW_WIDTH, MAP_VIEW_HEIGHT = WINDOW_HEIGHT - ACTION_BAR_HEIGHT;
const int BUTTON_WIDTH = 157;
const int BUTTON_HEIGHT = 41;

enum Player {PLAYER1, PLAYER2};

enum Action {NONE, MOVE, ATTACK, BUILD, BUILD_BARRACK, BUILD_DEFENSE_TURRET, BUILD_BRIDGE, BUILD_DRILL, BUILD_EXTRACTOR};

enum EntityType {BUILDING, FIGHTINGUNIT, BUILDINGUNIT};
