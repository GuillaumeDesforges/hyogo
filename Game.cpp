#include "Game.h"

Game::Game() : map(MapGen::uniformRandomMapGenerator(50, 50)), viewOffset(Vect2D(0, 0)) {
	window = openWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hyogo");
	// TODO change map generation
	// TODO REMOVE
	resourceManager.initResources();
	FightingUnit scoutTest(FightingUnits::SCOUT, Vect2D(1, 1));
	unitStore.add(1, scoutTest);
}

void Game::addViewOffset(const Vect2D &v) {
	viewOffset += v;
	if (viewOffset.x > 0)
		viewOffset.x = 0;
	if (viewOffset.y > 0)
		viewOffset.y = 0;
	if (viewOffset.x + map.getWidth() * MAP_SQUARE_PIXEL_SIZE < MAP_VIEW_WIDTH)
		viewOffset.x = -map.getWidth() * MAP_SQUARE_PIXEL_SIZE + MAP_VIEW_WIDTH;
	if (viewOffset.y + map.getHeight() * MAP_SQUARE_PIXEL_SIZE < MAP_VIEW_HEIGHT)
		viewOffset.y = -map.getHeight() * MAP_SQUARE_PIXEL_SIZE + MAP_VIEW_HEIGHT;
}

void Game::play() {
	playing = true;
	while (playing) {
		logic();
		// no frame skip
		draw(resourceManager);
	}
}

void Game::logic() {
	/*
	 * Events :
	 * 	keyboard :
	 * 		up/down/left/right : move camera
	 */
	Event ev;
	do {
		getEvent(1, ev); // ev becomes the next event
		switch (ev.type) {
			case EVT_KEY_ON:
				switch (ev.key) {
					case KEY_UP:
						addViewOffset(Vect2D(0, 16));
						break;
					case KEY_DOWN:
						addViewOffset(Vect2D(0, -16));
						break;
					case KEY_LEFT:
						addViewOffset(Vect2D(16, 0));
						break;
					case KEY_RIGHT:
						addViewOffset(Vect2D(-16, 0));
						break;
					default:
						break;
				}
				break;
			case EVT_BUT_ON: {
				// TODO handle mouse
				int x = ev.pix.x();
				int y = ev.pix.y();
				cout << "Click " << x << " " << y << endl;
				break;
			}
			default:
				break;
		}
	} while (ev.type != EVT_NONE);
	// TODO update UI
	// TODO update game state : entity store, buildings store
}

void Game::draw(const ResourceManager &resourceManager) const {
	// calculate render range
	int minXRender = -viewOffset.x / MAP_SQUARE_PIXEL_SIZE;
	int maxXRender = MAP_VIEW_WIDTH / MAP_SQUARE_PIXEL_SIZE + minXRender + 2;
	int minYRender = -viewOffset.y / MAP_SQUARE_PIXEL_SIZE;
	int maxYRender = MAP_VIEW_HEIGHT / MAP_SQUARE_PIXEL_SIZE + minYRender + 2;
	minXRender = (minXRender < 0) ? 0 : minXRender;
	minYRender = (minYRender < 0) ? 0 : minYRender;
	maxXRender = (maxXRender > map.getWidth()) ? map.getWidth() : maxXRender;
	maxYRender = (maxYRender > map.getHeight()) ? map.getHeight() : maxYRender;
	Vect2D minRender(minXRender, minYRender), maxRender(maxXRender, maxYRender);
	// prepare draw
	Imagine::noRefreshBegin();
	// clear screen
	Imagine::fillRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Imagine::AWHITE);
	// draw map
	map.draw(resourceManager, viewOffset, minRender, maxRender);
	// draw units
	unitStore.draw(playerTurn, resourceManager, viewOffset, minRender, maxRender);
	// draw buildings
	buildingStore.draw(playerTurn, resourceManager, viewOffset, minRender, maxRender);
	// draw UI
	uiManager.draw(resourceManager);
	// draw on screen
	Imagine::noRefreshEnd();
}