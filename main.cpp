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


#include "Game.hpp"


/*void process(std::string path) {
	sf::Image image;
	image.loadFromFile(path);
	for (uint32_t i = 0; i < image.getSize().x; i = i + 1) {
		for (uint32_t j = 0; j < image.getSize().y; j = j + 1) {
			if (image.getPixel(i, j) == sf::Color(106, 76, 48) or image.getPixel(i, j) == sf::Color(39, 27, 17)) {
				image.setPixel(i, j, sf::Color::Transparent);
			}
		}
	}
	path.pop_back();
	path.pop_back();
	path.pop_back();
	image.saveToFile(path + "png");
}*/
int main() {
	/*for (const std::string& d : {"n", "s", "w", "e"}) {
		for (uint32_t n = 0; n < 8; n = n + 1) {
			std::string ns = std::to_string(n);
			while (ns.size() < 4) {
				ns = ('0' + ns);
			}
			process("resources/infantryman/walking " + d + ns + ".bmp");
		}
		for (uint32_t n = 0; n < 13; n = n + 1) {
			std::string ns = std::to_string(n);
			while (ns.size() < 4) {
				ns = ('0' + ns);
			}
			process("resources/infantryman/attack " + d + ns + ".bmp");
		}
	}
	for (uint32_t n = 0; n < 7; n = n + 1) {
		std::string ns = std::to_string(n);
		while (ns.size() < 4) {
			ns = ('0' + ns);
		}
		process("resources/infantryman/talking e" + ns + ".bmp");
	}
	for (const std::string& d : { "n", "s", "w", "e", "nw", "ne", "sw", "se" }) {
		for (uint32_t n = 0; n < 7; n = n + 1) {
			std::string ns = std::to_string(n);
			while (ns.size() < 4) {
				ns = ('0' + ns);
			}
			process("resources/infantryman/been hit " + d + ns + ".bmp");
		}
	}
	for (const std::string& d : { "n", "s", "w", "e", "nw", "ne", "sw", "se" }) {
		for (uint32_t n = 0; n < 9; n = n + 1) {
			std::string ns = std::to_string(n);
			while (ns.size() < 4) {
				ns = ('0' + ns);
			}
			process("resources/infantryman/tipping over " + d + ns + ".bmp");
		}
	}*/
	Game::get()->run();
	return 0;
}