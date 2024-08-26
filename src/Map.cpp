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
#include "Healer.hpp"
#include "Treasure.hpp"
#include "Plant.hpp"
#include "ArchiveType.hpp"


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

    while (std::getline(file, line)) {
        if (line == "  <data encoding=\"csv\">") {
            read = true;
            std::getline(file, line);
        }
        else if (line == "</data>") {
            break;
        }
        if (!read) {
            continue;
        }
        std::stringstream ss(line);
        std::string word;
        x = 0;
        while (std::getline(ss, word, ',')) {
            uint32_t id = std::stoi(word);
            if (id >= 257 and id < 281) {
                this->getStatePtr()->getCollectionsPtr()->add(std::make_shared<Forest>(x, y, id - 257));
            }
            else if (id >= 281 and id < 296) {
                this->getStatePtr()->getCollectionsPtr()->add(std::make_shared<Mountains>(x, y, id - 281));
            }
            else if (id >= 296) {
                this->getStatePtr()->getCollectionsPtr()->add(std::make_shared<Water>(x, y, id - 296));
            }
            else if (id == 1) {
                this->state.getPlayersPtr()->addPlayer(Player(currentPlayerId));

                std::shared_ptr<Castle> c = std::make_shared<Castle>(x, y, currentPlayerId);
                c->setMaxHp();
                this->getStatePtr()->getCollectionsPtr()->add(c);

                this->getStatePtr()->getCollectionsPtr()->add(std::make_shared<Healer>(x + c->getSX(), y, currentPlayerId));
                this->getStatePtr()->getCollectionsPtr()->add(std::make_shared<Infantryman>(x + c->getSX() + 1, y, currentPlayerId));
                this->getStatePtr()->getCollectionsPtr()->add(std::make_shared<Infantryman>(x + c->getSX(), y + 1, currentPlayerId));
                this->getStatePtr()->getCollectionsPtr()->add(std::make_shared<Infantryman>(x + c->getSX() + 1, y + 1, currentPlayerId));

                currentPlayerId = currentPlayerId + 1;
            }
            else if (id == 4) {
                this->getStatePtr()->getCollectionsPtr()->add(std::make_shared<Iron>(x, y));
            }
            else if (id == 6) {
                this->getStatePtr()->getCollectionsPtr()->add(std::make_shared<Stone>(x, y));
            }
            else if (id == 8) {
                this->getStatePtr()->getCollectionsPtr()->add(std::make_shared<Treasure>(x, y));
            }
            else if (id == 3) {
                this->getStatePtr()->getCollectionsPtr()->add(std::make_shared<Plant>(x, y));
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