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


#include "LicenceButtonSpec.hpp"
#include "WindowButton.hpp"
#include "PlaySoundEvent.hpp"
#include "CreateEEvent.hpp"


LicenceButtonSpec::LicenceButtonSpec() = default;
LicenceButtonSpec::LicenceButtonSpec(uint32_t index) : MenuButtonSpec(index) {

}
StringLcl LicenceButtonSpec::getString() const {
    return { "{show_license}" };
}
Events LicenceButtonSpec::getEvents() const {
    Events clickEvent;
    clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

    std::shared_ptr<WindowButton> licenseWindow = std::make_shared<WindowButton>(StringLcl("{license}"), StringLcl("{close}"), clickEvent, 600, 600);

    Events licenseEvent = clickEvent;
    licenseEvent.add(std::make_shared<CreateEEvent>(licenseWindow));

    return licenseEvent;
}