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


#include <filesystem>
#include <fstream>
#include "IsServerTable.hpp"
#include "Root.hpp"


IsServerTable* IsServerTable::singletone = nullptr;


bool IsServerTable::isServer() const {
	return std::filesystem::is_regular_file(USERDATA_ROOT + "/network/i_am_server.cfg");
}
void IsServerTable::markAsServer() const {
	if (!std::filesystem::is_directory(USERDATA_ROOT + "/network")) {
		std::filesystem::create_directories(USERDATA_ROOT + "/network");
	}
	std::ofstream file(USERDATA_ROOT + "/network/i_am_server.cfg");
	file.close();
}
void IsServerTable::markAsClient() const {
	if (std::filesystem::is_regular_file(USERDATA_ROOT + "/network/i_am_server.cfg")) {
		std::filesystem::remove(USERDATA_ROOT + "/network/i_am_server.cfg");
	}
}
void IsServerTable::invert() const {
	if (this->isServer()) {
		this->markAsClient();
	}
	else {
		this->markAsServer();
	}
}