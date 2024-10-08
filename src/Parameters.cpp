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
#include <fstream>
#include <sstream>
#include <vector>
#include "Parameters.hpp"
#include "CouldntOpenParameters.hpp"
#include "Root.hpp"


static std::string GET_PATH() {
	return Root::get().getDataRoot() + "/configs/parameters.cfg";
}
void Parameters::load() {
	std::ifstream file(GET_PATH());
	if (!file.is_open()) {
		throw CouldntOpenParameters(GET_PATH());
	}

	std::string line;
	while (std::getline(file, line, '\n')) {
		std::stringstream ss(line);
		std::vector<std::string> words;
		std::string word;
		while (std::getline(ss, word, ' ')) {
			words.push_back(word);
		}

		if (words.size() < 3) {
			continue;
		}

		std::string type = words.at(0);
		bool ref = (words.at(1) == "ref");
		std::string key = words.at(2);
		try {
			if (type == "damage") {
				if (ref) {
					this->damages[key] = this->damages[words.at(3)];
				}
				else {
					uint32_t damagePoints = std::stoul(words.at(3));
					std::string damageTypeVal = words.at(4);
                    uint8_t damageType;
					if (damageTypeVal == "cut") {
                        damageType = Damage::CUT;
					}
					else if (damageTypeVal == "stab") {
						damageType = Damage::STAB;
					}
					else if (damageTypeVal == "crush") {
						damageType = Damage::CRUSH;
					}
                    else if (damageTypeVal == "magick") {
                        damageType = Damage::MAGICK;
                    }
                    else if (damageTypeVal == "service") {
                        damageType = Damage::SERVICE;
                    }
                    uint8_t damageSpec = Damage::SPEC::NONE;
                    for (uint32_t i = 5; i < words.size(); i = i + 1) {
                        if (words.at(i) == "poison") {
                            damageSpec = damageSpec | Damage::SPEC::POISON;
                        }
                    }
                    this->damages[key] = Damage(damagePoints, damageType, damageSpec);
				}
			}
			else if (type == "defence") {
				if (ref) {
					this->defences[key] = this->defences[words.at(3)];
				}
				else {
					this->defences[key] = Defence(std::stod(words.at(3)), std::stod(words.at(4)), std::stod(words.at(5)), std::stod(words.at(6)));
				}
			}
			else if (type == "resources") {
				if (ref) {
					this->resources[key] = this->resources[words.at(3)];
				}
				else {
					Resources resources;
					for (uint32_t i = 3; i < words.size(); i = i + 2) {
						std::string type = words.at(i);
						uint32_t value = std::stoul(words.at(i + 1));
						Resource resource(type, value);
						resources.plus(resource);
					}
					this->resources[key] = resources;
				}
			}
			else if (type == "resource") {
				if (ref) {
					this->resource[key] = this->resource[words.at(3)];
				}
				else {
					this->resource[key] = Resource(words.at(3), std::stoul(words.at(4)));
				}
			}
			else if (type == "int") {
				if (ref) {
					this->ints[key] = this->ints[words.at(3)];
				}
				else {
					this->ints[key] = std::stoul(words.at(3));
				}
			}
			else if (type == "double") {
				if (ref) {
					this->doubles[key] = this->doubles[words.at(3)];
				}
				else {
					this->doubles[key] = std::stod(words.at(3));
				}
			}
		}
		catch (std::exception&) {
			std::cout << "Error while reading key " << key << std::endl;
			throw CouldntOpenParameters(GET_PATH());
		}
	}

	file.close();
}
Damage Parameters::getDamage(const std::string& id) const {
	auto it = this->damages.find(id);
	if (it == this->damages.end()) {
		std::cerr << "Invalid damage uid: " << id << std::endl;
		return Damage(0, Damage::CUT);
	}
	return it->second;
}
Defence Parameters::getDefence(const std::string& id) const {
	auto it = this->defences.find(id);
	if (it == this->defences.end()) {
		std::cerr << "Invalid defence uid: " << id << std::endl;
		return Defence(1, 1, 1, 1);
	}
	return it->second;
}
Resources Parameters::getResources(const std::string& id) const {
	auto it = this->resources.find(id);
	if (it == this->resources.end()) {
		std::cerr << "Invalid resources uid: " << id << std::endl;
		return Resources();
	}
	return it->second;
}
Resource Parameters::getResource(const std::string& id) const {
	auto it = this->resource.find(id);
	if (it == this->resource.end()) {
		std::cerr << "Invalid resource uid: " << id << std::endl;
		return Resource();
	}
	return it->second;
}
uint32_t Parameters::getInt(const std::string& id) const {
	auto it = this->ints.find(id);
	if (it == this->ints.end()) {
		std::cerr << "Invalid int uid: " << id << std::endl;
		return 1;
	}
	return it->second;
}
double Parameters::getDouble(const std::string& id) const {
	auto it = this->doubles.find(id);
	if (it == this->doubles.end()) {
		std::cerr << "Invalid double uid: " << id << std::endl;
		return 1;
	}
	return it->second;
}