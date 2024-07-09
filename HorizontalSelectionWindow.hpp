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


#include "CameraIndependentPopUpElement.hpp"
#include "Button.hpp"
#include "HorizontalSelectionWindowComponent.hpp"
#include "LabelWithImage.hpp"
#include "CloseWindowEvent.hpp"
#include "MoveHorizontalSelectionWindowUpEvent.hpp"
#include "MoveHorizontalSelectionWindowDownEvent.hpp"
#include "SoundQueue.hpp"
#include "Sounds.hpp"


#pragma once


class HorizontalSelectionWindow : public CameraIndependentPopUpElement {
public:
	HorizontalSelectionWindow(const std::string &soundName1, const std::string &soundName2, const std::vector<HorizontalSelectionWindowComponent>& components, uint32_t componentSize = 64);

	Events run(uint32_t windowW, uint32_t windowH) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	Events click() override;
private:
	RectangularUiElement rect;
	std::string soundName1;
	std::string soundName2;
	std::vector<HorizontalSelectionWindowComponent> components;
	uint32_t componentSize;
	std::vector<Button> buttons;
	Button up;
	Button down;
	bool inited;

	void finish() override;
	bool show(const Button& button) const;
	bool possibleToMoveUp() const;
	bool possibleToMoveDown() const;
	void moveUp();
	void moveDown();
	void handle(Events& events);

	void handleMoveUpEvent(std::shared_ptr<MoveHorizontalSelectionWindowUpEvent> e);
	void handleMoveDownEvent(std::shared_ptr<MoveHorizontalSelectionWindowDownEvent> e);
	void handlePlaySoundEvent(std::shared_ptr<PlaySoundEvent> e);
};