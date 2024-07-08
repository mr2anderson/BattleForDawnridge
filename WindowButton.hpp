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
#include "Label.hpp"
#include "Button.hpp"
#include "CloseWindowEvent.hpp"


#pragma once


class WindowButton : public CameraIndependentPopUpElement {
public:
	WindowButton(const std::string &soundName1, const std::string& soundName2, const std::wstring& message, const std::wstring &buttonText, uint32_t w = 400, uint32_t h = 225);

	Events run(uint32_t windowW, uint32_t windowH) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	Events click() override;
	void update() override;
private:
    uint32_t w, h;
    std::string soundName1, soundName2;
	std::wstring message;
    std::wstring buttonText;
	Label label;
	Button button;
	bool inited;

};