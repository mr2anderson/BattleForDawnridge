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


#include "GuideButtonSpec.hpp"
#include "HorizontalSelectionWindow.hpp"
#include "CreateEEvent.hpp"
#include "PlaySoundEvent.hpp"


GuideButtonSpec::GuideButtonSpec() = default;
GuideButtonSpec::GuideButtonSpec(uint32_t index) : MenuButtonSpec(index) {

}
StringLcl GuideButtonSpec::getString() const {
    return { "{guide}" };
}
Events GuideButtonSpec::getEvents() const {
    Events clickEvent;
    clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

    std::vector<HorizontalSelectionWindowComponent> guideWindowComponents;
    guideWindowComponents.emplace_back(
          "exit_icon",
        StringLcl("{leave}"),
        true,
        clickEvent
    );
    guideWindowComponents.emplace_back(
          "food_icon",
        StringLcl("{guide_food_description}"),
        false,
        Events()
    );
    guideWindowComponents.emplace_back(
          "wood_icon",
        StringLcl("{guide_wood_description}"),
        false,
        Events()
    );
    guideWindowComponents.emplace_back(
          "stone_icon",
        StringLcl("{guide_stone_description}"),
        false,
        Events()
    );
    guideWindowComponents.emplace_back(
          "iron_icon",
        StringLcl("{guide_iron_description}"),
        false,
        Events()
    );
    guideWindowComponents.emplace_back(
          "crystal_icon",
        StringLcl("{guide_crystal_description}"),
        false,
        Events()
    );
    guideWindowComponents.emplace_back(
          "gold_icon",
        StringLcl("{guide_gold_description}"),
        false,
        Events()
    );
    guideWindowComponents.emplace_back(
          "road_none",
        StringLcl("{guide_road_system_description}"),
        false,
        Events()
    );
    guideWindowComponents.emplace_back(
          "shield_icon",
        StringLcl("{guide_damage_mods_description}"),
        false,
        Events()
    );
    guideWindowComponents.emplace_back(
          "fire1",
        StringLcl("{guide_fire_system_description}"),
        false,
        Events()
    );
    guideWindowComponents.emplace_back(
          "warehouse_food",
        StringLcl("{guide_storage_system_description}"),
        false,
        Events()
    );
    guideWindowComponents.emplace_back(
          "wall1",
        StringLcl("{guide_wall_system_description}"),
        false,
        Events()
    );
    guideWindowComponents.emplace_back(
          "forest_icon",
        StringLcl("{guide_movement_cost_system_description}"),
        false,
        Events()
    );
    guideWindowComponents.emplace_back(
          "water_icon",
        StringLcl("{guide_obstacle_system_description}"),
        false,
        Events()
    );
    guideWindowComponents.emplace_back(
          "helmet",
        StringLcl("{guide_population_limit_system_description}"),
        false,
        Events()
    );

    std::shared_ptr<HorizontalSelectionWindow> guideWindow = std::make_shared<HorizontalSelectionWindow>(guideWindowComponents);
    Events createGuideWindowEvent = clickEvent;
    createGuideWindowEvent.add(std::make_shared<CreateEEvent>(guideWindow));
    
    return createGuideWindowEvent;
}