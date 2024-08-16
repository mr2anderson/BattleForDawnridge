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


#include <vector>
#include <cstdint>
#include <boost/serialization/access.hpp>


#pragma once


template <typename T> class Collection {
public:
	Collection() = default;
    Collection(const Collection& collection) = delete;

	void push(T* t) {
        this->content.push_back(t);
	}
	uint32_t size() const {
		return this->content.size();
	}
    void clear() {
        this->content.clear();
    }
    T* at(uint32_t i) {
        return this->content.at(i);
    }
    T* at(uint32_t i) const {
        return this->content.at(i);
    }
private:
    std::vector<T*> content;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        ar & this->content;
    }
};




#include "SortedIndexedContainer.hpp"
#include "GO.hpp"
#include "CompByClickPriority.hpp"
#include "CompByDrawPriority.hpp"
#include "CompByNewMovePriority.hpp"



template<> class Collection<GO> {
public:
    Collection() = default;
    ~Collection() {
        this->clear();
    }
    Collection(const Collection& collection) = delete;

    bool hasError(MapSize mapSize, uint32_t totalPlayers) const {
        // Skipping checking drawPriority & clickPriority cuz they are built based on newMovePriority
        for (uint32_t i = 0; i < this->newMovePriority.size(); i = i + 1) {
            if (this->newMovePriority.at(i) == nullptr or this->newMovePriority.at(i)->hasError(mapSize, totalPlayers)) {
                return true;
            }
        }
        return false;
    }

    void push(GO* t) {
        this->newMovePriority.insert(t);
        this->drawPriority.insert(t);
        this->clickPriority.insert(t);
    }
    uint32_t size() const {
        return this->newMovePriority.size();
    }
    void clear() {
        for (uint32_t i = 0; i < this->newMovePriority.size(); i = i + 1) {
            delete this->newMovePriority.at(i);
        }
        this->newMovePriority.clear();
        this->drawPriority.clear();
        this->clickPriority.clear();
    }
    GO* at(uint32_t i, uint8_t filter) {
        switch (filter) {
            case FILTER::NEW_MOVE_PRIORITY:
                return this->newMovePriority.at(i);
            case FILTER::DRAW_PRIORITY:
                return this->drawPriority.at(i);
            case FILTER::CLICK_PRIORITY:
                return this->clickPriority.at(i);
        }
        return nullptr;
    }
    GO* at(uint32_t i, uint8_t filter) const {
        switch (filter) {
            case FILTER::NEW_MOVE_PRIORITY:
                return this->newMovePriority.at(i);
            case FILTER::DRAW_PRIORITY:
                return this->drawPriority.at(i);
            case FILTER::CLICK_PRIORITY:
                return this->clickPriority.at(i);
        }
        return nullptr;
    }
private:
    SortedIndexedContainer<GO*, CompByNewMovePriority> newMovePriority;
    SortedIndexedContainer<GO*, CompByDrawPriority> drawPriority;
    SortedIndexedContainer<GO*, CompByClickPriority> clickPriority;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        if (Archive::is_loading::value) {
            this->clear();
        }
        ar & this->newMovePriority;
        if (Archive::is_loading::value) {
            for (uint32_t i = 0; i < this->newMovePriority.size(); i = i + 1) {
                this->drawPriority.insert(this->newMovePriority.at(i));
                this->clickPriority.insert(this->newMovePriority.at(i));
            }
        }
    }
};