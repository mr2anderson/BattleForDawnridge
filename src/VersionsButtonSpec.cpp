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


#include <boost/version.hpp>
#include <SFML/Config.hpp>
#include "VersionsButtonSpec.hpp"
#include "HorizontalSelectionWindow.hpp"
#include "PlaySoundEvent.hpp"
#include "CreateEEvent.hpp"
#include "version.hpp"
#include "ColorTheme.hpp"


VersionsButtonSpec::VersionsButtonSpec() = default;
VersionsButtonSpec::VersionsButtonSpec(uint32_t index) : MenuButtonSpec(index) {

}
StringLcl VersionsButtonSpec::getString() const {
    return {"{libs}"};
}
Events VersionsButtonSpec::getEvents() const {
    Events clickEvent;
    clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

    std::vector<HorizontalSelectionWindowComponent> components;
    components.emplace_back(
            "exit_icon",
            StringLcl("{exit}"),
            true,
            clickEvent
    );
    components.emplace_back(
        "icon",
        StringLcl("{battle_for_dawnridge}") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_NEUTRAL) + StringLcl(std::string(VERSION)),
        false,
        Events()
    );
    components.emplace_back(
          "boost_icon",
          StringLcl("{boost}") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_NEUTRAL) + StringLcl(std::string(BOOST_LIB_VERSION)),
          false,
          Events()
    );
    components.emplace_back(
            "sfml_icon",
            StringLcl("{sfml}") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_NEUTRAL) + StringLcl(std::to_string(SFML_VERSION_MAJOR)) + StringLcl(".") + StringLcl(std::to_string(SFML_VERSION_MINOR)) + StringLcl(".") + StringLcl(std::to_string(SFML_VERSION_PATCH)),
            false,
            Events()
    );

    std::shared_ptr<HorizontalSelectionWindow> window = std::make_shared<HorizontalSelectionWindow>(components);

    Events createWindowEvent = clickEvent;
    createWindowEvent.add(std::make_shared<CreateEEvent>(window));

    return createWindowEvent;
}