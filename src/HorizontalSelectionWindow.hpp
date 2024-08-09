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
#include "MoveHorizontalSelectionWindowUpEvent.hpp"
#include "MoveHorizontalSelectionWindowDownEvent.hpp"
#include "HorizontalSelectionWindowStructure.hpp"


#pragma once


class HorizontalSelectionWindow : public CameraIndependentPopUpElement {
public:
	HorizontalSelectionWindow(const std::vector<HorizontalSelectionWindowComponent>& components, uint32_t componentSize = 64, uint32_t marginSize = 10);

	void onRestart() override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	Events click(uint32_t mouseX, uint32_t mouseY, uint32_t windowW, uint32_t windowH) override;
private:
	RectangularUiElement rect;
	std::vector<HorizontalSelectionWindowComponent> components;
	uint32_t componentSize, marginSize;
	uint32_t offset;

	bool possibleToMoveUp(uint32_t componentsInFrame) const;
	bool possibleToMoveDown() const;
	void moveUp();
	void moveDown();
	void handle(Events& events);

	void handleMoveUpEvent(std::shared_ptr<MoveHorizontalSelectionWindowUpEvent> e);
	void handleMoveDownEvent(std::shared_ptr<MoveHorizontalSelectionWindowDownEvent> e);

	uint32_t getComponentsInFrame(uint32_t windowW, uint32_t windowH) const;
	HorizontalSelectionWindowStructure getStructure(uint32_t windowW, uint32_t windowH) const;
};