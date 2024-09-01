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


#include "BuildButtonSpec.hpp"
#include "HorizontalSelectionWindow.hpp"
#include "PlaySoundEvent.hpp"
#include "TryToBuildEvent.hpp"
#include "CreateEEvent.hpp"
#include "Road.hpp"
#include "House.hpp"
#include "Arable.hpp"
#include "Sawmill.hpp"
#include "Quarry.hpp"
#include "Mine.hpp"
#include "Well.hpp"
#include "WarehouseFood.hpp"
#include "WarehouseWood.hpp"
#include "WarehouseStone.hpp"
#include "WarehouseIron.hpp"
#include "WarehouseGold.hpp"
#include "WarehouseCrystal.hpp"
#include "Barracks.hpp"
#include "Infirmary.hpp"
#include "Workshop.hpp"
#include "Wall1.hpp"
#include "Gates1.hpp"
#include "Tower1.hpp"
#include "Market.hpp"
#include "SpellFactory.hpp"
#include "Castle.hpp"
#include "SockererHouse.hpp"
#include "ColorTheme.hpp"


BuildButtonSpec::BuildButtonSpec() = default;
BuildButtonSpec::BuildButtonSpec(uint32_t index) : MainScreenButtonSpec(index) {

}
std::string BuildButtonSpec::getTextureName() const {
	return   "hammer_icon";
}
StringLcl BuildButtonSpec::getString() const {
	return StringLcl("{open_build_menu}");
}




template<typename T> StringLcl GET_BUILD_DESCRIPTION() {
	std::unique_ptr<T> t = std::make_unique<T>();
	StringLcl description = t->getDescription() + '\n' +
		StringLcl("{cost}") + t->getCost().getReadableInfo() + "\n" + 
		StringLcl("{hp}") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_BEST) + std::to_string(t->getMaxHP()) + StringLcl(" ") + t->getDefence().getReadable() + StringLcl(". {building_speed}") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_NEUTRAL) + std::to_string(t->getRegenerationSpeed());
	return description;
}
template<typename T> HorizontalSelectionWindowComponent GET_BUILD_COMPONENT() {
	Events clickEvent;
	clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

	Events createBuildingModeEvent = clickEvent;
	createBuildingModeEvent.add(std::make_shared<TryToBuildEvent>(std::make_shared<T>(0, 0, 0)));

	HorizontalSelectionWindowComponent component = {
		T().getTextureName(),
		GET_BUILD_DESCRIPTION<T>(),
		true,
		createBuildingModeEvent
	};

	return component;
}




Events BuildButtonSpec::getEvents() const {
	Events clickEvent;
	clickEvent.add(std::make_shared<PlaySoundEvent>("click"));



	std::vector<HorizontalSelectionWindowComponent> buildMenuSectionMainComponents;
	buildMenuSectionMainComponents.emplace_back(  "hammer_icon", StringLcl("{leave}"), true, clickEvent);
	buildMenuSectionMainComponents.emplace_back(GET_BUILD_COMPONENT<Road>());
	buildMenuSectionMainComponents.emplace_back(GET_BUILD_COMPONENT<House>());
	buildMenuSectionMainComponents.emplace_back(GET_BUILD_COMPONENT<Castle>());

	std::shared_ptr<HorizontalSelectionWindow> buildWindowSectionMain = std::make_shared<HorizontalSelectionWindow>(buildMenuSectionMainComponents);
	Events createBuildWindowSectionMainEvent = clickEvent;
	createBuildWindowSectionMainEvent.add(std::make_shared<CreateEEvent>(buildWindowSectionMain));



	std::vector<HorizontalSelectionWindowComponent> buildMenuSectionResourceCollectorsComponents;
	buildMenuSectionResourceCollectorsComponents.emplace_back(  "hammer_icon", StringLcl("{leave}"), true, clickEvent);
	buildMenuSectionResourceCollectorsComponents.emplace_back(GET_BUILD_COMPONENT<Arable>());
	buildMenuSectionResourceCollectorsComponents.emplace_back(GET_BUILD_COMPONENT<Sawmill>());
	buildMenuSectionResourceCollectorsComponents.emplace_back(GET_BUILD_COMPONENT<Quarry>());
	buildMenuSectionResourceCollectorsComponents.emplace_back(GET_BUILD_COMPONENT<Mine>());
	buildMenuSectionResourceCollectorsComponents.emplace_back(GET_BUILD_COMPONENT<Well>());

	std::shared_ptr<HorizontalSelectionWindow> buildWindowSectionResourceCollectors = std::make_shared<HorizontalSelectionWindow>(buildMenuSectionResourceCollectorsComponents);
	Events createBuildWindowSectionResourceCollectorsEvent = clickEvent;
	createBuildWindowSectionResourceCollectorsEvent.add(std::make_shared<CreateEEvent>(buildWindowSectionResourceCollectors));



	std::vector<HorizontalSelectionWindowComponent> buildMenuSectionWarehousesComponents;
	buildMenuSectionWarehousesComponents.emplace_back(  "hammer_icon", StringLcl("{leave}"), true, clickEvent);
	buildMenuSectionWarehousesComponents.emplace_back(GET_BUILD_COMPONENT<WarehouseFood>());
	buildMenuSectionWarehousesComponents.emplace_back(GET_BUILD_COMPONENT<WarehouseWood>());
	buildMenuSectionWarehousesComponents.emplace_back(GET_BUILD_COMPONENT<WarehouseStone>());
	buildMenuSectionWarehousesComponents.emplace_back(GET_BUILD_COMPONENT<WarehouseIron>());
	buildMenuSectionWarehousesComponents.emplace_back(GET_BUILD_COMPONENT<WarehouseCrystal>());
	buildMenuSectionWarehousesComponents.emplace_back(GET_BUILD_COMPONENT<WarehouseGold>());

	std::shared_ptr<HorizontalSelectionWindow> buildWindowSectionWarehouses = std::make_shared<HorizontalSelectionWindow>(buildMenuSectionWarehousesComponents);
	Events createBuildWindowSectionWarehousesEvent = clickEvent;
	createBuildWindowSectionWarehousesEvent.add(std::make_shared<CreateEEvent>(buildWindowSectionWarehouses));



	std::vector<HorizontalSelectionWindowComponent> buildMenuSectionTroopsComponents;
	buildMenuSectionTroopsComponents.emplace_back(  "hammer_icon", StringLcl("{leave}"), true, clickEvent);
	buildMenuSectionTroopsComponents.emplace_back(GET_BUILD_COMPONENT<Barracks>());
	buildMenuSectionTroopsComponents.emplace_back(GET_BUILD_COMPONENT<Infirmary>());
	buildMenuSectionTroopsComponents.emplace_back(GET_BUILD_COMPONENT<Workshop>());
	buildMenuSectionTroopsComponents.emplace_back(GET_BUILD_COMPONENT<SockererHouse>());
	buildMenuSectionTroopsComponents.emplace_back(GET_BUILD_COMPONENT<SpellFactory>());

	std::shared_ptr<HorizontalSelectionWindow> buildWindowSectionTroops = std::make_shared<HorizontalSelectionWindow>(buildMenuSectionTroopsComponents);
	Events createBuildWindowSectionTroopsEvent = clickEvent;
	createBuildWindowSectionTroopsEvent.add(std::make_shared<CreateEEvent>(buildWindowSectionTroops));



	std::vector<HorizontalSelectionWindowComponent> buildMenuSectionDefenceComponents;
	buildMenuSectionDefenceComponents.emplace_back(  "hammer_icon", StringLcl("{leave}"), true, clickEvent);
	buildMenuSectionDefenceComponents.emplace_back(GET_BUILD_COMPONENT<Wall1>());
	buildMenuSectionDefenceComponents.emplace_back(GET_BUILD_COMPONENT<Gates1>());
	buildMenuSectionDefenceComponents.emplace_back(GET_BUILD_COMPONENT<Tower1>());

	std::shared_ptr<HorizontalSelectionWindow> buildWindowSectionDefence = std::make_shared<HorizontalSelectionWindow>(buildMenuSectionDefenceComponents);
	Events createBuildWindowSectionDefenceEvent = clickEvent;
	createBuildWindowSectionDefenceEvent.add(std::make_shared<CreateEEvent>(buildWindowSectionDefence));



	std::vector<HorizontalSelectionWindowComponent> buildMenuSectionOtherComponents;
	buildMenuSectionOtherComponents.emplace_back(  "hammer_icon", StringLcl("{leave}"), true, clickEvent);
	buildMenuSectionOtherComponents.emplace_back(GET_BUILD_COMPONENT<Market>());

	std::shared_ptr<HorizontalSelectionWindow> buildWindowSectionOther = std::make_shared<HorizontalSelectionWindow>(buildMenuSectionOtherComponents);
	Events createBuildWindowSectionOtherEvent = clickEvent;
	createBuildWindowSectionOtherEvent.add(std::make_shared<CreateEEvent>(buildWindowSectionOther));



	std::vector<HorizontalSelectionWindowComponent> buildMenuComponents;
	buildMenuComponents.emplace_back(  "hammer_icon", StringLcl("{leave}"), true, clickEvent);
	buildMenuComponents.emplace_back(  "hammer_icon", StringLcl("{main}"), true, createBuildWindowSectionMainEvent);
	buildMenuComponents.emplace_back(  "hammer_icon", StringLcl("{resource_collectors}"), true, createBuildWindowSectionResourceCollectorsEvent);
	buildMenuComponents.emplace_back(  "hammer_icon", StringLcl("{warehouses}"), true, createBuildWindowSectionWarehousesEvent);
	buildMenuComponents.emplace_back(  "hammer_icon", StringLcl("{troops}"), true, createBuildWindowSectionTroopsEvent);
	buildMenuComponents.emplace_back(  "hammer_icon", StringLcl("{defence}"), true, createBuildWindowSectionDefenceEvent);
	buildMenuComponents.emplace_back(  "hammer_icon", StringLcl("{other}"), true, createBuildWindowSectionOtherEvent);

	std::shared_ptr<HorizontalSelectionWindow> buildWindow = std::make_shared<HorizontalSelectionWindow>(buildMenuComponents);
	Events buildEvent = clickEvent;
	buildEvent.add(std::make_shared<CreateEEvent>(buildWindow));



	return buildEvent;
}