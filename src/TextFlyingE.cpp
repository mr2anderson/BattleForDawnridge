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


#include <sstream>
#include "TextFlyingE.hpp"
#include "Fonts.hpp"
#include "RichText/RichText.hpp"


TextFlyingE::TextFlyingE() = default;
TextFlyingE::TextFlyingE(const StringLcl &str, uint32_t x, uint32_t y, uint32_t sx, uint32_t sy) : FlyingE(64 * x + 64 * sx / 2, 64 * y + 64 * sy / 2)  {
    this->str = str;
}
std::unique_ptr<sf::Drawable> TextFlyingE::getDrawable(sf::Vector2f position, sf::Color color) const {
    sfe::RichText text;
    text.setFont(*Fonts::get().get("1"));

    std::wstringstream ss(this->str.getNoColor());
    std::unordered_map<uint32_t, sf::Color> colors = this->str.getColors();
    std::wstring line;
    uint32_t ctr = 0;
    while (std::getline(ss, line, L'\n')) {
        std::wstringstream wordss(line);
        std::wstring word;
        while (std::getline(wordss, word, L' ')) {
            if (colors.find(ctr) == colors.end()) {
                text << sf::Color::White;
            }
            else {
                sf::Color orColor = colors[ctr];
                orColor.a = color.a;
                text << orColor;
            }
            text << word + L' ';
            ctr = ctr + 1;
        }
        text << L'\n';
    }

    text.setCharacterSize(18);
    text.setPosition(position);
    text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);

    return std::make_unique<sfe::RichText>(text);
}


BOOST_CLASS_EXPORT_IMPLEMENT(TextFlyingE)