/*
 *  Conquesta
 *  Copyright (C) 2023 mr2anderson
 *
 *  Conquesta is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Conquesta is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Conquesta.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <optional>
#include <tuple>
#include <cstdint>


#pragma once


struct GameEvent {
	GameEvent();

	// Это событие создается виртуально внутри класса BattleScreen в режиме строительства
	// std::optional<Building*> buildBuilding;

	// Используются в меню, возвращаемом при нажатии на мастерскую игроком-владельцем
	// std::optional<std::tuple<Tank*, Workshop*>> createTank;
	// std::optional<Workshop*> upgradeWorkshop;
	
	// Используются в меню, возвращаемом при нажатии на караван игроком-владельцем
	// std::optional<std::tuple<Trade, Caravan*>> doTrade;
	
	// Используется при нажатии на танк игроком-владельцем
	// std::optional<Tank*> selectTank;
	
	// Это событие создается виртуально внутри класса BattleScreen при нажатии игроком на пустую клетку
	// std::optional<std::tuple<uint32_t, uint32_t>> moveTo;
	
	// Используется при нажатии на танк или здание игроком-противником
	// std::optional<std::tuple<Unit*>> attack;

	// Используется при нажатии почти на все игровые объекты, а также может создаваться виртуально внутри класса BatlleScreen при нажатии игроком на пустую клетку
	// std::optional<sf::Sound*> playSound;
};