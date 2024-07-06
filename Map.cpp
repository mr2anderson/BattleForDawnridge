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


#include "Map.hpp"


Map::Map(const std::string &path) {
    this->players = new std::vector<Player*>();
    this->go = new std::vector<GO*>();
    this->resourcePoints = new std::vector<ResourcePoint*>();
    this->units = new std::vector<Unit*>();
    this->tbs = new std::vector<TerritoryB*>();
    this->tcbs = new std::vector<TerritoryConductorB*>();
    this->tobs = new std::vector<TerritoryOriginB*>();

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
                this->players->resize(this->players->size() + 1);
                this->players->back() = new Player(this->players->size());
                Castle* c = new Castle(x, y, this->players->at(currentPlayerId));
                c->setMaxHp();
                this->add(c);
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
            x = x + 1;
        }
        y = y + 1;
    }
    file.close();

    this->w = x + 1;
    this->h = y + 1;
}
Map::~Map() {
    for (uint32_t i = 0; i < this->players->size(); i = i + 1) {
        delete this->players->at(i);
    }
    delete this->players;

    for (uint32_t i = 0; i < this->go->size(); i = i + 1) {
        delete this->go->at(i);
    }
    delete this->go;

    delete this->resourcePoints;
    delete this->units;
    delete this->tbs;
    delete this->tcbs;
    delete this->tobs;
}
void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (uint32_t i = 0; i < this->go->size(); i = i + 1) {
        target.draw(*this->go->at(i), states);
    }
}
uint32_t Map::getW() const {
    return this->w;
}
uint32_t Map::getH() const {
    return this->h;
}
uint32_t Map::getPlayersNumber() const {
    return this->players->size();
}
Player* Map::getPlayer(uint32_t i) {
    return this->players->at(i);
}
std::vector<GO*>* Map::getGO() {
    return this->go;
}
std::vector<Unit*>* Map::getUnits() {
    return this->units;
}
std::vector<ResourcePoint*>* Map::getResourcePoints() {
    return this->resourcePoints;
}
std::vector<TerritoryB*>* Map::getTbs() {
    return this->tbs;
}
std::vector<TerritoryConductorB*>* Map::getTcbs() {
    return this->tcbs;
}
std::vector<TerritoryOriginB*>* Map::getTobs() {
    return this->tobs;
}
void Map::add(GO *object) {
    this->go->push_back(object);

    ResourcePoint* rp = dynamic_cast<ResourcePoint*>(object);
    if (rp) {
        this->resourcePoints->push_back(rp);
    }
    else {
        Unit* u = dynamic_cast<Unit*>(object);
        if (u) {
            this->units->push_back(u);

            TerritoryB* tb = dynamic_cast<TerritoryB*>(object);
            if (tb) {
                this->tbs->push_back(tb);

                TerritoryOriginB* tob = dynamic_cast<TerritoryOriginB*>(object);
                if (tob) {
                    this->tobs->push_back(tob);
                }
                else {
                    TerritoryConductorB* tcb = dynamic_cast<TerritoryConductorB*>(object);
                    if (tcb) {
                        this->tcbs->push_back(tcb);
                    }
                }
            }
        }
    }
}