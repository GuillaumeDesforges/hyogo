#include "Game.h"

Game::Game() : map(MapGen::uniformRandomMapGenerator(50, 50)), viewOffset(Vect2D(0, 0)) {
    window = openWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hyogo");
    resourceManager.initResources();
    // TODO change map generation
    // TODO REMOVE
    FightingUnit unitTest0(FightingUnits::INFANTRY_MELEE, Vect2D(3, 5));
    unitStore.add(PLAYER1, unitTest0);
    FightingUnit unitTest1(FightingUnits::SCOUT, Vect2D(5, 3));
    unitStore.add(PLAYER1, unitTest1);
    FightingUnit unitTest2(FightingUnits::INFANTRY_MELEE, Vect2D(2, 1));
    unitStore.add(PLAYER2, unitTest2);
    // end REMOVE
    unitStore.updatePossibleMoves(map, buildingStore, PLAYER1);
}

void Game::addViewOffset(const Vect2D &v) {
    viewOffset += v;
    if (viewOffset.x() > 0)
        viewOffset.x() = 0;
    if (viewOffset.y() > 0)
        viewOffset.y() = 0;
    if (viewOffset.x() + map.getWidth() * MAP_SQUARE_PIXEL_SIZE < MAP_VIEW_WIDTH)
        viewOffset.x() = -map.getWidth() * MAP_SQUARE_PIXEL_SIZE + MAP_VIEW_WIDTH;
    if (viewOffset.y() + map.getHeight() * MAP_SQUARE_PIXEL_SIZE < MAP_VIEW_HEIGHT)
        viewOffset.y() = -map.getHeight() * MAP_SQUARE_PIXEL_SIZE + MAP_VIEW_HEIGHT;
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
    // Event handler
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
                clickManager(ev.pix);
                break;
            }
            default:
                break;
        }
    } while (ev.type != EVT_NONE);

    // End turn handler
    if (currentAction == ENDTURN) {
        viewOffset -= viewOffset;
        playerTurn = (playerTurn == PLAYER1) ? PLAYER2 : PLAYER1;
        currentAction = NONE;
        unitStore.clearFinishedTurn();
        unitStore.updatePossibleMoves(map, buildingStore, playerTurn);
        Imagine::fillRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, ABLACK);
        string msg = (playerTurn == PLAYER1) ? "Player 1 turn" : "Player 2 turn";
        Imagine::drawString(WINDOW_WIDTH / 2 - 35, WINDOW_HEIGHT / 2, msg, AWHITE);
        Imagine::click();
    }
}

void Game::clickManager(const Vect2D &position) {
    // click on map view
    if (position.y() < MAP_VIEW_HEIGHT) {
        Vect2D coordPos = ((position - viewOffset) - modulo(position - viewOffset, MAP_SQUARE_PIXEL_SIZE)) /
                          MAP_SQUARE_PIXEL_SIZE;
        actionManager.clickMap(coordPos, currentAction, playerTurn, selectedEntity, unitStore, buildingStore, map,
                               uiManager,mineralQuantity[playerTurn],gasQuantity[playerTurn]);
    }
        // click on ui panel
    else {
        uiManager.clickActionButton(position, currentAction);
    }

}

void Game::draw(const ResourceManager &resourceManager) const {
    // calculate render range
    int minXRender = -viewOffset.x() / MAP_SQUARE_PIXEL_SIZE;
    int maxXRender = MAP_VIEW_WIDTH / MAP_SQUARE_PIXEL_SIZE + minXRender + 2;
    int minYRender = -viewOffset.y() / MAP_SQUARE_PIXEL_SIZE;
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
    // draw unit selection box
    if (selectedEntity != NULL) {
        if (inside(selectedEntity->getPosition(), minRender, maxRender)) {
            selectedEntity->drawSelectionBox(viewOffset);
        }
        if (currentAction == MOVE) {
            const Unit *selectedUnit = dynamic_cast<const Unit *>(selectedEntity);
            selectedUnit->drawPossibleMoves(viewOffset, minRender, maxRender);
        }
    }
    // draw units
    unitStore.draw(playerTurn, resourceManager, viewOffset, minRender, maxRender);
    // draw buildings
    buildingStore.draw(playerTurn, resourceManager, viewOffset, minRender, maxRender);
    // draw UI
    uiManager.draw(resourceManager);
    // draw on screen
    Imagine::noRefreshEnd();
}
