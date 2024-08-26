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


#include "Label.hpp"
#include "Fonts.hpp"
#include "SfmlTextAline/SfmlTextAline.hpp"


Label::Label() = default;
Label::Label(int32_t x, int32_t y, uint32_t w, uint32_t h, const StringLcl &str, bool center) : RectangularUiElement(x, y, w, h) {
	this->message = str;
	this->center = center;
}
void Label::makeLineBold(int32_t smartIndex) {
	this->bold.push_back(smartIndex);
}
void Label::makeLineItalic(int32_t smartIndex) {
	this->italic.push_back(smartIndex);
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
static uint32_t TO_NORMAL_INDEX(uint32_t lines, int32_t smartIndex) {
	if (smartIndex >= 0) {
		return smartIndex;
	}
	return lines + smartIndex;
}
void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    this->RectangularUiElement::draw(target, states);
	
	SfmlTextAline text;
	text.setPosition(this->getX() + 5, this->getY() + 5);
	text.setCharacterSize(12);
	if (this->center) {
		text.setAlignment(SfmlTextAline::Alignment::Center);
	}
	text.setMinWidth(this->getW() - 10);
	text.setColor(sf::Color::White);
	text.setFont(*Fonts::get().get("1"));
	text.setString(WRAP_TEXT(this->message.get(), this->getW() - 10, text.getFont(), text.getCharacterSize()));

	uint32_t n = text.getNumberOfLines();
	for (auto b : this->bold) {
		text.setLineBold(TO_NORMAL_INDEX(n, b), true);
	}
	for (auto i : this->italic) {
		text.setLineItalic(TO_NORMAL_INDEX(n, i), true);
	}

	target.draw(text, states);
}


BOOST_CLASS_EXPORT_IMPLEMENT(Label)