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



#include <iostream>
#include "Valkyr.hpp"
#include "Root.hpp"
void process(std::string path) {
	sf::Image image;
	image.loadFromFile(path);
	for (uint32_t i = 0; i < image.getSize().x; i = i + 1) {
		for (uint32_t j = 0; j < image.getSize().y; j = j + 1) {
			if (image.getPixel(i, j) == sf::Color(97, 68, 43) or image.getPixel(i, j) == sf::Color(31, 22, 13)) {
				image.setPixel(i, j, sf::Color::Transparent);
			}
		}
	}
	path.pop_back();
	path.pop_back();
	path.pop_back();
	image.saveToFile(path + "png");
    std::cout << "saving to " << path + "png" << std::endl;
}




int main() {
	for (const std::string& a : {"talking", "running", "attack", "been hit", "tipping over"}) {
		for (const std::string& d : { "n", "s", "w", "e", "nw", "ne", "sw", "se" }) {
			for (const std::tuple<std::string, uint32_t>& w : {
				std::make_tuple("valkyr", Valkyr().getAnimationNumber(a, d)) }) {
				for (uint32_t i = 0; i < std::get<uint32_t>(w); i = i + 1) {
					std::string s = std::to_string(i);
					while (s.size() < 4) {
						s = ('0' + s);
					}
					process(DATA_ROOT + "/images/" + std::get<std::string>(w) + "/" + a + " " + d + s + ".bmp");
				}
			}
		}
	}
	Game::get()->run();
	return 0;
}