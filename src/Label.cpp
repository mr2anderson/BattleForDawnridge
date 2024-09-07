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
#include <algorithm>
#include "Label.hpp"
#include "Fonts.hpp"
#include "RichText/RichText.hpp"


Label::Label() = default;
Label::Label(int32_t x, int32_t y, uint32_t w, uint32_t h, const StringLcl &str, bool center) : RectangularUiElement(x, y, w, h) {
	this->message = str;
	this->center = center;
}
static sf::String WRAP_TEXT(sf::String string, unsigned width, const sf::Font* font, unsigned charicterSize) {
	unsigned currentOffset = 0;
	bool firstWord = true;
	std::size_t wordBegining = 0;

	for (std::size_t pos(0); pos < string.getSize(); ++pos) {
		auto currentChar = string[pos];
		if (currentChar == '\n') {
			currentOffset = 0;
			firstWord = true;
			continue;
		}
		else if (currentChar == ' ') {
			wordBegining = pos;
			firstWord = false;
		}

		auto glyph = font->getGlyph(currentChar, charicterSize, false);
		currentOffset += glyph.advance;

		if (!firstWord && currentOffset > width) {
			pos = wordBegining;
			string[pos] = '\n';
			firstWord = true;
			currentOffset = 0;
		}
	}

	return string;
}
void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    this->RectangularUiElement::draw(target, states);
	
	sfe::RichText text;
	text.setCharacterSize(12);
	text.setFont(*Fonts::get().get("1"));
    std::wstring string = WRAP_TEXT(this->message.getWithoutStyles(), this->getW() - 10, text.getFont(), text.getCharacterSize()).toWideString();
    std::wstringstream lineSS(string);
    std::wstring line;
    uint32_t ctr = 0;
    std::unordered_map<uint32_t, StringLcl::Style> styles = this->message.getStyles();
    while (std::getline(lineSS, line, L'\n')) {
        std::wstringstream wordSS(line);
        std::wstring word;
        while (wordSS >> word) {
            if (styles.find(ctr) != styles.end()) {
                text << styles[ctr].color << styles[ctr].style;
            }
            else {
                text << sf::Color::White << sf::Text::Style::Regular;
            }
            text << word;
            text << L' ';
            ctr = ctr + 1;
        }
        text << L'\n';
    }

	if (this->center) {
		text.setPosition(this->getX() + this->getW() / 2, this->getY() + 5);
		text.setOrigin(text.getGlobalBounds().width / 2, 0);
	}
	else {
		text.setPosition(this->getX() + 5, this->getY() + 5);
	}

	target.draw(text, states);
}


BOOST_CLASS_EXPORT_IMPLEMENT(Label)