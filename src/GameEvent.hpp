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

	// ��� ������� ��������� ���������� ������ ������ BattleScreen � ������ �������������
	// std::optional<Building*> buildBuilding;

	// ������������ � ����, ������������ ��� ������� �� ���������� �������-����������
	// std::optional<std::tuple<Tank*, Workshop*>> createTank;
	// std::optional<Workshop*> upgradeWorkshop;
	
	// ������������ � ����, ������������ ��� ������� �� ������� �������-����������
	// std::optional<std::tuple<Trade, Caravan*>> doTrade;
	
	// ������������ ��� ������� �� ���� �������-����������
	// std::optional<Tank*> selectTank;
	
	// ��� ������� ��������� ���������� ������ ������ BattleScreen ��� ������� ������� �� ������ ������
	// std::optional<std::tuple<uint32_t, uint32_t>> moveTo;
	
	// ������������ ��� ������� �� ���� ��� ������ �������-�����������
	// std::optional<std::tuple<Unit*>> attack;

	// ������������ ��� ������� ����� �� ��� ������� �������, � ����� ����� ����������� ���������� ������ ������ BatlleScreen ��� ������� ������� �� ������ ������
	// std::optional<sf::Sound*> playSound;
};