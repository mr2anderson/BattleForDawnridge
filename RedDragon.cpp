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


#include "RedDragon.hpp"
#include "RedDragonFlame.hpp"
#include "Texts.hpp"


RedDragon::RedDragon() = default;
RedDragon::RedDragon(std::shared_ptr<GOCollection<Unit>> units, uint32_t playerId) : Dragon(playerId) {
	this->units = units;
}
Dragon* RedDragon::clone() const {
	return new RedDragon(*this);
}
std::wstring RedDragon::getDescriptionText() const {
	return *Texts::get()->get("red_dragon_description");
}
std::wstring RedDragon::getRaiseText() const {
	return *Texts::get()->get("red_dragon_raise");
}
std::wstring RedDragon::getRestText() const {
	return *Texts::get()->get("red_dragon_rest");
}
std::wstring RedDragon::getAttackText() const {
	return *Texts::get()->get("red_dragon_attack");
}
std::string RedDragon::getTextureName() const {
	return "red_dragon";
}
Resources RedDragon::getCost() const {
	Resources cost;

	cost.plus(Resource("food", 50000));
	cost.plus(Resource("iron", 50000));

	return cost;
}
uint32_t RedDragon::getRecoverTime() const {
	return 10;
}
std::string RedDragon::getInfoSoundName() const {
	return "red_dragon_info";
}
std::string RedDragon::getAttackSoundName() const {
	return "red_dragon_attack";
}
std::shared_ptr<DragonFlame> RedDragon::getFlameType() const {
	return std::make_shared<RedDragonFlame>();
}
Events RedDragon::doAttack(uint32_t centerX, uint32_t centerY) {
	std::shared_ptr<DragonFlame> f = this->getFlame(centerX, centerY);

	sf::IntRect fRect;
	fRect.left = f->getXMin();
	fRect.top = f->getYMin();
	fRect.width = f->getXMax() - f->getXMin() + 1;
	fRect.height = f->getYMax() - f->getYMin() + 1;

	if (fRect.left != 0) {
		fRect.left = fRect.left - 1;
		fRect.width = fRect.width + 1;
	}
	if (fRect.top != 0) {
		fRect.top = fRect.top - 1;
		fRect.height = fRect.height + 1;
	}

	Events result;
	for (uint32_t i = 0; i < this->units->size(); i = i + 1) {
		Unit* u = this->units->at(i);
		if (!u->exist() or u->getPlayerId() == this->getPlayerId()) {
			continue;
		}
		
		bool contains = false;
		for (uint32_t x = u->getX(); x <= u->getX() + u->getSX(); x = x + u->getSX()) {
			for (uint32_t y = u->getY(); y <= u->getY() + u->getSY(); y = y + u->getSY()) {
				if (fRect.contains(x, y)) {
					contains = true;
					break;
				}
			}
		}

		if (contains) {
			result = result + u->hit(Damage(10000 * (u->getHP() != 1), Damage::TYPE::FLAME), std::nullopt); // preventing destroying new buildings (would be too imbalance)
		}
	}
	return result;
}