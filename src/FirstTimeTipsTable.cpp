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
#include "FirstTimeTipsTable.hpp"
#include "Root.hpp"


FirstTimeTipsTable* FirstTimeTipsTable::singletone = nullptr;


static std::string GET_LOCAL_ROOT() {
	return USERDATA_ROOT + "/first_time_tips";
}
static bool LOCAL_ROOT_EXIST() {
	return std::filesystem::is_directory(GET_LOCAL_ROOT());
}
static void CREATE_LOCAL_ROOT() {
	std::filesystem::create_directories(GET_LOCAL_ROOT());
}
static std::string GET_PATH(const std::string& id) {
	return GET_LOCAL_ROOT() + "/" + id + ".cfg";
}


bool FirstTimeTipsTable::wasDisplayed(const std::string& id) const {
	if (LOCAL_ROOT_EXIST()) {
		return std::filesystem::exists(GET_PATH(id));
	}
	return false;
}
void FirstTimeTipsTable::markAsDisplayed(const std::string& id) {
	if (!LOCAL_ROOT_EXIST()) {
		CREATE_LOCAL_ROOT();
	}
	std::ofstream file(GET_PATH(id));
	file.close();
}
void FirstTimeTipsTable::clear() {
	if (LOCAL_ROOT_EXIST()) {
		std::filesystem::remove_all(GET_LOCAL_ROOT());
	}
}