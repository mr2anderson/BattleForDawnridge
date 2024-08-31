/*
 *  Battle for Dawnridge
 *  Copyright (C) 2024 mr2anderson
 *
 *  Battle for Dawnridge is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Battle for Dawnridge is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Battle for Dawnridge.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <fstream>
#include <sstream>
#include "Map.hpp"
#include "GO.hpp"
#include "Forest.hpp"
#include "Stone.hpp"
#include "Iron.hpp"
#include "Mountains.hpp"
#include "Water.hpp"
#include "CouldntOpenMap.hpp"
#include "Root.hpp"
#include "Castle.hpp"
#include "Infantryman.hpp"
#include "Treasure.hpp"
#include "Plant.hpp"
#include "ArchiveType.hpp"
#include "Textures.hpp"


Map::Map() = default;
void Map::load(const std::string &path) {
    std::ifstream file(Root::get().getDataRoot() + "/" + path);
    if (!file.is_open()) {
        throw CouldntOpenMap(path);
    }

    std::string line;
    bool read = false;
    uint32_t y = 0;
    uint32_t x;
    uint32_t currentPlayerId = 1;

    uint32_t totalWater = Textures::get().get("water")->getSize().x * Textures::get().get("water")->getSize().y / 64 / 64;
    uint32_t totalMountains = Textures::get().get("mountains")->getSize().x * Textures::get().get("mountains")->getSize().y / 64 / 64;
    uint32_t totalTmx = Textures::get().get("tmx_tileset")->getSize().x * Textures::get().get("tmx_tileset")->getSize().y / 64 / 64;
    uint32_t totalForest = Textures::get().get("forest")->getSize().x * Textures::get().get("forest")->getSize().y / 64 / 64;

    uint32_t waterBegin, waterEnd;
    uint32_t mountainsBegin, mountainsEnd;
    uint32_t tmxTilesetBegin, tmxTilesetEnd;
    uint32_t forestBegin, forestEnd;
    uint32_t curr = 1;

    while (std::getline(file, line)) {
        if (line == "  <data encoding=\"csv\">") {
            read = true;
            std::getline(file, line);
        }
        else if (line == "</data>") {
            break;
        }
        if (line.find("water") != std::string::npos) {
            waterBegin = curr;
            waterEnd = curr + totalWater;
            curr = waterEnd;
        }
        else if (line.find("mountains") != std::string::npos) {
            mountainsBegin = curr;
            mountainsEnd = curr + totalMountains;
            curr = mountainsEnd;
        }
        else if (line.find("forest") != std::string::npos) {
            forestBegin = curr;
            forestEnd = curr + totalForest;
            curr = forestEnd;
        }
        else if (line.find("tmx_tileset") != std::string::npos) {
            tmxTilesetBegin = curr;
            tmxTilesetEnd = curr + totalTmx;
            curr = tmxTilesetEnd;
        }
        if (!read) {
            continue;
        }
        std::stringstream ss(line);
        std::string word;
        x = 0;
        while (std::getline(ss, word, ',')) {
            uint32_t id = std::stoi(word);
            if (id >= forestBegin and id < forestEnd) {
                this->getStatePtr()->getCollectionsPtr()->add(std::make_shared<Forest>(x, y, id - forestBegin));
            }
            else if (id >= mountainsBegin and id < mountainsEnd) {
                this->getStatePtr()->getCollectionsPtr()->add(std::make_shared<Mountains>(x, y, id - mountainsBegin));
            }
            else if (id >= waterBegin and id < waterEnd) {
                this->getStatePtr()->getCollectionsPtr()->add(std::make_shared<Water>(x, y, id - waterBegin));
            }
            else if (id >= tmxTilesetBegin and id < tmxTilesetEnd) {
                if (id == tmxTilesetBegin) {
                    this->state.getPlayersPtr()->addPlayer(Player(currentPlayerId));

                    std::shared_ptr<Castle> c = std::make_shared<Castle>(x, y, currentPlayerId);
                    c->setMaxHp();
                    this->getStatePtr()->getCollectionsPtr()->add(c);

                    for (uint32_t dx = 0; dx < 3; dx = dx + 1) {
                        for (uint32_t dy = 0; dy < 2; dy = dy + 1) {
                            uint32_t resultX = x + c->getSX() + dx;
                            uint32_t resultY = y + dy;
                            this->getStatePtr()->getCollectionsPtr()->add(std::make_shared<Infantryman>(resultX, resultY, currentPlayerId));
                        }
                    }

                    currentPlayerId = currentPlayerId + 1;
                }
                else if (id == tmxTilesetBegin + 3) {
                    this->getStatePtr()->getCollectionsPtr()->add(std::make_shared<Iron>(x, y));
                }
                else if (id == tmxTilesetBegin + 5) {
                    this->getStatePtr()->getCollectionsPtr()->add(std::make_shared<Stone>(x, y));
                }
                else if (id == tmxTilesetBegin + 7) {
                    this->getStatePtr()->getCollectionsPtr()->add(std::make_shared<Treasure>(x, y));
                }
                else if (id == tmxTilesetBegin + 2) {
                    this->getStatePtr()->getCollectionsPtr()->add(std::make_shared<Plant>(x, y));
                }
            }
            x = x + 1;
        }
        y = y + 1;
    }
    file.close();

    this->state.getMapSizePtr()->setWidth(x);
    this->state.getMapSizePtr()->setHeight(y);
}
MapState* Map::getStatePtr() {
    return &this->state;
}
const MapState* Map::getStatePtr() const {
    return &this->state;
}


BOOST_CLASS_EXPORT_IMPLEMENT(Map)