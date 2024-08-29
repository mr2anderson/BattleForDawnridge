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
#include "Logs.hpp"
#include "Root.hpp"
#include "UTFEncoder.hpp"


Logs::Logs() {
	this->limit = 0;
	if (!std::filesystem::is_directory(Root::get().getUserdataRoot())) {
		std::filesystem::create_directories(Root::get().getUserdataRoot());
	}
	this->file.open(Root::get().getUserdataRoot() + "/srv_logs.txt", std::ios::app);
}
Logs::~Logs() {
	this->file.close();
}
void Logs::setEntryLimit(uint32_t newLimit) {
	this->limit = newLimit;
}
uint32_t Logs::getEntryLimit() const {
	return this->limit;
}
void Logs::add(const StringLcl &lcl) {
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	std::stringstream ss;
	ss << std::put_time(&tm, "%Y-%m-%d-%H-%M-%S");
	std::string time = '[' + ss.str() + "]";
	StringLcl str = time + " ";
	str = str + lcl;

	this->file << UTFEncoder::get().utf16ToUtf8(str.get() + L"\n");

	this->content.push_back(str);
	while (this->content.size() > this->limit) {
		this->content.pop_front();
	}

	this->contentCached.clear();
	for (const auto& a : this->content) {
		this->contentCached = this->contentCached + a + "\n";
	}
}
Label Logs::toLabel(uint32_t windowW, uint32_t windowH) const {
	return Label(10, 10, windowW - 20, windowH - 20, this->contentCached, true, false);
}