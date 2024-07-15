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


#include "Gates.hpp"
#include "CreateEEvent.hpp"


Gates::Gates() = default;
Gates::Gates(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units, std::shared_ptr<GOCollection<TerritoryOriginB>> originsPtr) :
        TerritoryConductorB(x, y, playerId, units, originsPtr),
        Building(x, y, playerId, units) {

}
Events Gates::newMove(uint32_t playerId) {
    Events response;
    if (this->belongTo(playerId) and this->exist()) {
        return this->regenerate();
    }
    return response;
}
bool Gates::warriorCanStay(uint32_t warriorPlayerId) const {
    return (warriorPlayerId == this->getPlayerId());
}
uint32_t Gates::getWarriorMovementCost(uint32_t warriorPlayerId) const {
    if (warriorPlayerId == this->getPlayerId()) {
        return 1;
    }
    return WARRIOR_MOVEMENT_FORBIDDEN;
}
bool Gates::isHighObstacle() const {
    return true;
}
uint32_t Gates::getRadius() const {
    return 1;
}
Events Gates::getSelectionW() {
    Events response;

    std::vector<HorizontalSelectionWindowComponent> components;
    components.push_back(this->getExitComponent());
    components.push_back(this->getDescriptionComponent());
    components.push_back(this->getHpInfoComponent());
    components.push_back(this->getDestroyComponent());
    if (this->repairing()) {
        components.push_back(this->getBusyWithRepairingComponent());
    }
    if (this->works() and !this->connectedToTerritoryOriginB()) {
        components.push_back(this->getNotConnectedComponent());
    }

    std::shared_ptr<HorizontalSelectionWindow> window = std::make_shared<HorizontalSelectionWindow>(components);
    response.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));
    response.add(std::make_shared<CreateEEvent>(window));

    return response;
}
Events Gates::getGameObjectResponse(uint32_t playerId) {
    if (!this->exist()) {
        return Events();
    }
    if (this->belongTo(playerId)) {
        return this->getHighlightEvent() + this->getSelectionW();
    }
    return Events();
}