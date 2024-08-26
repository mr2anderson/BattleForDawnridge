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


#include "MenuBg.hpp"
#include "Textures.hpp"


MenuBg::MenuBg() = default;
void MenuBg::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::RectangleShape rect;
    rect.setPosition(0, 0);
    rect.setSize(sf::Vector2f(target.getSize().x, target.getSize().y));
    rect.setFillColor(sf::Color(11, 16, 20));

    sf::Sprite s;
    s.setTexture(*Textures::get().get("bg"));
    s.setPosition(target.getSize().x - s.getLocalBounds().width, target.getSize().y - s.getLocalBounds().height);

    target.draw(rect, states);
    target.draw(s, states);
}