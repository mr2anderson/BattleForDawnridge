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


#include <iostream>
#include <sstream>
#include "StringLcl.hpp"
#include "Locales.hpp"
#include "UTFEncoder.hpp"


StringLcl::StringLcl() = default;
StringLcl::StringLcl(const std::string& data) {
	this->data = data;
}
std::unordered_map<uint32_t, StringLcl::Style> StringLcl::getStyles() const {
    try {
        std::unordered_map<uint32_t, StringLcl::Style> result;
        std::wstringstream ss(this->toWstring());
        std::wstring line;
        sf::Color currentColor = sf::Color::White;
        sf::Text::Style currentStyle = sf::Text::Style::Regular;
        uint32_t ctr = 0;
        while (std::getline(ss, line, L'\n')) {
            std::wstringstream wordSS(line);
            std::wstring word;
            while (wordSS >> word) {
                if (word == L"[c]") {
                    std::wstring r, g, b;
                    wordSS >> r >> g >> b;
                    currentColor = sf::Color(std::stoi(r), std::stoi(g), std::stoi(b));
                }
                else if (word == L"[b]") {
                    currentStyle = sf::Text::Style::Bold;
                }
                else {
                    if (currentColor != sf::Color::White or currentStyle != sf::Text::Style::Regular) {
                        result[ctr] = Style();
                        result[ctr].color = currentColor;
                        result[ctr].style = currentStyle;
                        currentColor = sf::Color::White;
                        currentStyle = sf::Text::Style::Regular;
                    }
                    ctr = ctr + 1;
                }
            }
        }
        return result;
    }
    catch (std::exception&) {
        std::cout << "Invalid string lcl format: " << this->data << std::endl;
        return {};
    }
}
std::wstring StringLcl::getWithoutStyles() const {
    try {
        std::wstring result;
        std::wstringstream ss(this->toWstring());
        std::wstring line;
        while (std::getline(ss, line, L'\n')) {
            std::wstringstream wordSS(line);
            std::wstring word;
            while (wordSS >> word) {
                if (word == L"[c]") {
                    std::wstring trash;
                    wordSS >> trash >> trash >> trash;
                }
                else if (word == L"[b]") {

                }
                else {
                    result = result + word + L' ';
                }
            }
            result = result + L'\n';
        }
        return result;
    }
    catch (std::exception&) {
        std::cout << "Invalid string lcl format: " << this->data << std::endl;
        return L"COULDNT'T CONVERT STRING LCL FORMAT REPORT DEVELOPER";
    }
}
std::string StringLcl::toRawString() const {
	return this->data;
}
StringLcl StringLcl::operator+(const StringLcl& b) const {
	return StringLcl(this->data + b.data);
}
StringLcl StringLcl::operator+(const std::string & b) const {
	return StringLcl(this->data + b);
}
StringLcl StringLcl::operator+(char c) const {
	StringLcl result(this->data);
	result.data.push_back(c);
	return result;
}
void StringLcl::clear() {
	this->data.clear();
}
std::wstring StringLcl::toWstring() const {
    std::wstring result;

    std::string buff;
    for (uint32_t i = 0; i < this->data.size(); i = i + 1) {
        if (this->data.at(i) == '{') {
            result = result + UTFEncoder::get().utf8ToUtf16(buff);
            buff.clear();
            continue;
        }
        if (this->data.at(i) == '}') {;
            result = result + *Locales::get().get(buff);
            buff.clear();
            continue;
        }
        buff = buff + this->data.at(i);
    }
    result = result + UTFEncoder::get().utf8ToUtf16(buff);

    return result;
}
StringLcl StringLcl::COLOR(uint32_t r, uint32_t g, uint32_t b) {
    StringLcl s("[c]");
    s = s + " ";
    s = s + std::to_string(r);
    s = s + " ";
    s = s + std::to_string(g);
    s = s + " ";
    s = s + std::to_string(b);
    s = s + " ";
    return s;
}
StringLcl StringLcl::COLOR(sf::Color c) {
    return COLOR(c.r, c.g, c.b);
}
StringLcl StringLcl::BOLD() {
    return {"[b] "};
}