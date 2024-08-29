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


#include <fstream>
#include <filesystem>
#include "IlluminationTableSettings.hpp"
#include "Root.hpp"




static std::string PATH() {
	return Root::get().getUserdataRoot() + "/illumination_table_settings.cfg";
}




IlluminationTableSettings::IlluminationTableSettings() {
	this->enable = true;
	this->brightness = 150;
}
void IlluminationTableSettings::load() {
	std::ifstream file(PATH());

	if (!file.is_open()) {
		return;
	}

	std::string buff;

	std::getline(file, buff);
	this->enable = (std::stoi(buff) != 0);

	std::getline(file, buff);
	this->brightness = std::stoi(buff);

	file.close();
}
void IlluminationTableSettings::setEnable(bool val) {
	this->enable = val;
	this->save();
}
void IlluminationTableSettings::setBrightness(uint8_t val) {
	this->brightness = val;
	this->save();
}
bool IlluminationTableSettings::isEnabled() const {
	return this->enable;
}
uint8_t IlluminationTableSettings::getBrightness() const {
	return this->brightness;
}
void IlluminationTableSettings::save() {
	if (!std::filesystem::is_directory(Root::get().getUserdataRoot())) {
		std::filesystem::create_directories(Root::get().getUserdataRoot());
	}

	std::ofstream file(PATH());
	file << std::to_string(this->enable) + "\n";
	file << std::to_string(this->brightness) + "\n";
	file.close();
}