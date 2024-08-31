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


#include "StatusBar.hpp"
#include "Time.hpp"
#include "GO.hpp"
#include "ColorTheme.hpp"
#include "Textures.hpp"
#include "Fonts.hpp"
#include "Image.hpp"
#include "Label.hpp"


StatusBar::StatusBar() = default;
void StatusBar::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::RectangleShape rect;
    rect.setPosition(target.getSize().x - 200 - 5, 5);
    rect.setFillColor(COLOR_THEME::UI_COLOR_TRANSPARENT);
    rect.setOutlineThickness(2);
    rect.setOutlineColor(sf::Color::Black);
    rect.setSize(sf::Vector2f(200, target.getSize().y - 10));
    target.draw(rect, states);

    sf::Sprite timeSprite;
    timeSprite.setTexture(*Textures::get().get(this->timeTextureName));
    timeSprite.setPosition(rect.getPosition().x + rect.getSize().x - 5 - timeSprite.getLocalBounds().width, rect.getPosition().y + 5);
    target.draw(timeSprite, states);

    sf::Text timeText;
    timeText.setFont(*Fonts::get().get("1"));
    timeText.setString(std::to_string(this->timeNumber) + " / " + std::to_string(Time::TOTAL));
    timeText.setStyle(sf::Text::Style::Bold);
    timeText.setCharacterSize(13);
    timeText.setPosition(rect.getPosition().x + 5, timeSprite.getPosition().y + timeSprite.getGlobalBounds().height / 2 - timeText.getGlobalBounds().height / 2);
    target.draw(timeText, states);

    Image goTexture(rect.getPosition().x + 5, timeText.getPosition().y + timeText.getGlobalBounds().height + 20, 128, this->goTextureName, IntRectSerializable(this->goTextureRect));
    target.draw(goTexture, states);

    Label goLabel(rect.getPosition().x + 5, goTexture.getY() + goTexture.getH() + 5, rect.getSize().x - 10, rect.getSize().y - 5 - (goTexture.getY() + goTexture.getH() + 5), this->goDescription, false);
    target.draw(goLabel, states);

    sf::RectangleShape darkness; // This can move focus from status bar to field
    darkness.setPosition(rect.getPosition());
    darkness.setSize(rect.getSize());
    darkness.setFillColor(COLOR_THEME::UI_COLOR_TRANSPARENT);
    target.draw(darkness, states);
}
void StatusBar::setTime(Time *time) {
    this->timeNumber = (uint32_t)time->getType() + 1;
    this->timeTextureName = time->getTextureName();
}
void StatusBar::setGO(std::shared_ptr<GO> go, MapState *state) {
    this->goTextureName = go->getTextureName();
    this->goTextureRect = go->getTextureRect();
    this->goDescription = go->getDetailedDescription(state);
}