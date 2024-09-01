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


#include "GraphicsButtonSpec.hpp"
#include "PlaySoundEvent.hpp"
#include "CreateEEvent.hpp"
#include "WindowButton.hpp"
#include "WindowEntry.hpp"
#include "HorizontalSelectionWindow.hpp"
#include "TurnOnIlluminationEvent.hpp"
#include "TurnOffIlluminationEvent.hpp"
#include "ChangeIlluminationBrightnessEvent.hpp"
#include "EntryNumSettings.hpp"
#include "ColorTheme.hpp"


GraphicsButtonSpec::GraphicsButtonSpec() : GraphicsButtonSpec(1) {

}
GraphicsButtonSpec::GraphicsButtonSpec(uint32_t index) : GraphicsButtonSpec(index, false, 0) {

}
GraphicsButtonSpec::GraphicsButtonSpec(uint32_t index, bool illuminationEnabled, uint8_t illuminationBrightness) : MenuButtonSpec(index) {
	this->illuminationEnabled = illuminationEnabled;
	this->illuminationBrightness = illuminationBrightness;
}
StringLcl GraphicsButtonSpec::getString() const {
	return { "{graphics}" };
}
Events GraphicsButtonSpec::getEvents() const {
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
		"illumination_settings_icon",
		StringLcl("{illumination_settings}"),
		true,
		this->getCreateIlluminationSettingsWindowEvent()
	);
	std::shared_ptr<HorizontalSelectionWindow> window = std::make_shared<HorizontalSelectionWindow>(components);

	Events createWindowEvent = clickEvent;
	createWindowEvent.add(std::make_shared<CreateEEvent>(window));
	return createWindowEvent;
}





Events GraphicsButtonSpec::getCreateIlluminationSettingsWindowEvent() const {
	Events clickEvent;
	clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

	std::vector<HorizontalSelectionWindowComponent> components;
	components.emplace_back(
		"exit_icon",
		StringLcl("{exit}"),
		true,
		clickEvent
	);
	if (this->illuminationEnabled) {
		components.push_back(this->getTurnOffIlluminationComponent());
		components.push_back(this->getSwitchIlluminationBrightnessComponent());
	}
	else {
		components.push_back(this->getTurnOnIlluminationComponent());
	}
	std::shared_ptr<HorizontalSelectionWindow> window = std::make_shared<HorizontalSelectionWindow>(components);

	Events createWindowEvent = clickEvent;
	createWindowEvent.add(std::make_shared<CreateEEvent>(window));
	return createWindowEvent;
}
HorizontalSelectionWindowComponent GraphicsButtonSpec::getTurnOnIlluminationComponent() const {
	Events clickEvent;
	clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

	std::shared_ptr<WindowButton> doneWindow = std::make_shared<WindowButton>(StringLcl("{illumination_was_turned_on}"), StringLcl("OK"), clickEvent);

	Events result = clickEvent;
	result.add(std::make_shared<TurnOnIlluminationEvent>());
	result.add(std::make_shared<CreateEEvent>(doneWindow));

	return {
		"illumination_settings_icon",
		StringLcl("{turn_on_illumination}"),
		true,
		result
	};
}
HorizontalSelectionWindowComponent GraphicsButtonSpec::getTurnOffIlluminationComponent() const {
	Events clickEvent;
	clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

	std::shared_ptr<WindowButton> doneWindow = std::make_shared<WindowButton>(StringLcl("{illumination_was_turned_off}"), StringLcl("OK"), clickEvent);

	Events result = clickEvent;
	result.add(std::make_shared<TurnOffIlluminationEvent>());
	result.add(std::make_shared<CreateEEvent>(doneWindow));

	return {
		"illumination_settings_icon",
		StringLcl("{turn_off_illumination}"),
		true,
		result
	};
}
HorizontalSelectionWindowComponent GraphicsButtonSpec::getSwitchIlluminationBrightnessComponent() const {
	Events clickEvent;
	clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

	std::shared_ptr<std::string> brightness = std::make_shared<std::string>();

	std::shared_ptr<EntryNumSettings> settings = std::make_shared<EntryNumSettings>();
	settings->setMax(255);

	std::shared_ptr<WindowButton> doneWindow = std::make_shared<WindowButton>(StringLcl("{brightness_was_changed}"), StringLcl("OK"), clickEvent);

	Events changeBrightnessEvent = clickEvent;
	changeBrightnessEvent.add(std::make_shared<ChangeIlluminationBrightnessEvent>(brightness));
	changeBrightnessEvent.add(std::make_shared<CreateEEvent>(doneWindow));
	
	std::shared_ptr<WindowEntry> enterBrightnessWindow = std::make_shared<WindowEntry>(StringLcl("{enter_brightness}"), StringLcl("{change}"), StringLcl("{cancel}"), brightness, settings, changeBrightnessEvent, clickEvent);

	Events result = clickEvent;
	result.add(std::make_shared<CreateEEvent>(enterBrightnessWindow));

	return {
		"illumination_settings_icon",
		StringLcl("{change_brightness}") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_NEUTRAL) + std::to_string(this->illuminationBrightness),
		true,
		result
	};
}