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
#include "CouldntOpenMap.hpp"
#include "Forest.hpp"
#include "Stone.hpp"
#include "Iron.hpp"
#include "Mountains.hpp"
#include "Water.hpp"
#include "Root.hpp"
#include "Infantryman.hpp"
#include "Legioner.hpp"


Map::Map(const std::string &path) {
    this->go = std::make_shared<GOCollection<GO>>();
    this->resourcePoints = std::make_shared<GOCollection<ResourcePoint>>();
    this->units = std::make_shared<GOCollection<Unit>>();
    this->tbs = std::make_shared<GOCollection<TerritoryB>>();
    this->tobs = std::make_shared<GOCollection<TerritoryOriginB>>();
    this->rsbs = std::make_shared<GOCollection<ResourceStorageB>>();
    this->vcbs = std::make_shared<GOCollection<VictoryConditionB>>();
    this->treasures = std::make_shared<GOCollection<Treasure>>();

    std::ifstream file(ROOT + "/" + path);
    if (!file.is_open()) {
        throw CouldntOpenMap(path);
    }

    std::string line;
    bool read = false;
    uint32_t y = 0;
    uint32_t x;
    uint32_t currentPlayerId = 0;
    while (std::getline(file, line)) {
        if (line == "  <data encoding=\"csv\">") {
            read = true;
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
            if (word == "10") {
                this->add(new Forest(x, y));
            }
            else if (word == "1") {
                this->players.resize(this->players.size() + 1);
                this->players.back() = Player(this->players.size());

                Castle* c = new Castle(x, y, this->players.at(currentPlayerId).getId(), this->getUnits());
                c->setMaxHp();
                this->add(c);
                
                this->add(new Legioner(x, y + c->getSY(), this->players.at(currentPlayerId).getId(), this->getUnits(), this->getGO(), this->getW(), this->getH()));
                for (uint32_t i = 1; i < 3; i = i + 1) {
                    this->add(new Infantryman(x + i * this->go->at(this->go->size() - 1)->getSX(), y + c->getSY(), this->players.at(currentPlayerId).getId(), this->getUnits(), this->getGO(), this->getW(), this->getH()));
                }

                currentPlayerId = currentPlayerId + 1;
            }
            else if (word == "18") {
                this->add(new Iron(x, y));
            }
            else if (word == "14") {
                this->add(new Stone(x, y));
            }
            else if (word == "22") {
                this->add(new Mountains(x, y));
            }
            else if (word == "26") {
                this->add(new Water(x, y));
            }
            else if (word == "30") {
                this->add(new Treasure(x, y));
            }
            x = x + 1;
        }
        y = y + 1;
    }
    file.close();

    this->w = x + 1;
    this->h = y + 1;
}
void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (uint8_t c = 0; c <= 1; c = c + 1) {
        for (uint32_t i = 0; i < this->go->size(); i = i + 1) {
            GO* o = this->go->at(i);
            if (o->highDrawingPriority() == c) {
                target.draw(*o, states);
            }
        }
    }
}
uint32_t Map::getW() const {
    return this->w;
}
uint32_t Map::getH() const {
    return this->h;
}
uint32_t Map::getPlayersNumber() const {
    return this->players.size();
}
Player* Map::getPlayer(uint32_t i) {
    return &this->players.at(i);
}
std::shared_ptr<GOCollection<GO>> Map::getGO() {
    return this->go;
}
std::shared_ptr<GOCollection<Unit>> Map::getUnits() {
    return this->units;
}
std::shared_ptr<GOCollection<ResourcePoint>> Map::getResourcePoints() {
    return this->resourcePoints;
}
std::shared_ptr<GOCollection<TerritoryB>> Map::getTbs() {
    return this->tbs;
}
std::shared_ptr<GOCollection<TerritoryOriginB>> Map::getTobs() {
    return this->tobs;
}
std::shared_ptr<GOCollection<ResourceStorageB>> Map::getRsbs() {
    return this->rsbs;
}
std::shared_ptr<GOCollection<VictoryConditionB>> Map::getVcbs() {
    return this->vcbs;
}
std::shared_ptr<GOCollection<Treasure>> Map::getTreasures() {
    return this->treasures;
}
void Map::add(GO *object) {
    this->go->push(object);

    
    if (ResourcePoint* rp = dynamic_cast<ResourcePoint*>(object)) {
        this->resourcePoints->push(rp);
    }
    if (Unit* u = dynamic_cast<Unit*>(object)) {
        this->units->push(u);
    }
    if (TerritoryB* tb = dynamic_cast<TerritoryB*>(object)) {
        this->tbs->push(tb);
    }
    if (TerritoryOriginB* tob = dynamic_cast<TerritoryOriginB*>(object)) {
        this->tobs->push(tob);
    }
    if (ResourceStorageB* rsb = dynamic_cast<ResourceStorageB*>(object)) {
        this->rsbs->push(rsb);
    }
    if (VictoryConditionB* vcb = dynamic_cast<VictoryConditionB*>(object)) {
        this->vcbs->push(vcb);
    }
    if (Treasure* t = dynamic_cast<Treasure*>(object)) {
        this->treasures->push(t);
    }
}