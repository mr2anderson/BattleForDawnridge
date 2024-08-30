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


#include "Time.hpp"
#include "GlobalRandomGenerator32.hpp"
#include "Textures.hpp"


Time::Time() {
    this->type = static_cast<Type>(GlobalRandomGenerator32::get().gen() % 6);
}
TimeMod Time::getTimeMod() const {
    switch (this->type) {
        case Type::Dawn:
            return NEUTRAL_TIME_MOD;
        case Type::Morning:
            return POSITIVE_TIME_MOD;
        case Type::Day:
            return POSITIVE_TIME_MOD;
        case Type::Sunset:
            return NEUTRAL_TIME_MOD;
        case Type::Night1:
            return NEGATIVE_TIME_MOD;
        case Type::Night2:
            return NEGATIVE_TIME_MOD;
    }
    return NEUTRAL_TIME_MOD;
}
Time::Type Time::getType() const {
    return this->type;
}

void Time::change() {
    this->type = static_cast<Type>((this->type + 1) % 6);
}

std::string Time::getSoundName() const {
    switch (this->type) {
        case Type::Dawn:
            return "dawn";
        case Type::Morning:
            return "morning";
        case Type::Day:
            return "day";
        case Type::Sunset:
            return "sunset";
        case Type::Night1:
            return "night";
        case Type::Night2:
            return "night";
    }
    return "";
}

sf::Sprite Time::getIcon(uint32_t viewW, uint32_t viewH) const {
    std::string name;
    switch (this->type) {
        case Type::Dawn:
            name = "dawn";
            break;
        case Type::Morning:
            name = "morning";
            break;
        case Type::Day:
            name = "day";
            break;
        case Type::Sunset:
            name = "sunset";
            break;
        case Type::Night1:
            name = "night1";
            break;
        case Type::Night2:
            name = "night2";
            break;
    }
    sf::Sprite sprite;
    sprite.setPosition(viewW - 144 - 5, 40);
    sprite.setTexture(*Textures::get().get(name));
    return sprite;
}

std::shared_ptr<sf::Drawable> Time::getEffect(const sf::RenderWindow &window, const sf::View &view) const {
    sf::Color color;
    switch (this->type) {
        case Type::Dawn:
            color = sf::Color(238, 169, 136, 15);
            break;
        case Type::Morning:
            color = sf::Color(238, 169, 136, 5);
            break;
        case Type::Day:
            color = sf::Color::Transparent;
            break;
        case Type::Sunset:
            color = sf::Color(192, 88, 126, 15);
            break;
        case Type::Night1:
            color = sf::Color(0, 0, 0, 75);
            break;
        case Type::Night2:
            color = sf::Color(0, 0, 0, 100);
    }

    sf::RectangleShape rect;
    rect.setPosition(window.mapPixelToCoords(sf::Vector2i(0, 0), view));
    rect.setSize(view.getSize());
    rect.setFillColor(color);
    return std::make_shared<sf::RectangleShape>(rect);
}