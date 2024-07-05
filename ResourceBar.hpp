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


#include "Textures.hpp"
#include "Fonts.hpp"
#include "ColorTheme.hpp"
#include "Resources.hpp"


#pragma once


class ResourceBar : public sf::Drawable {
public:
	ResourceBar();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void plus(const Resource& resource);
	void minus(const Resource& resource);
	void plus(const Resources& resources);
	void minus(const Resources& resources);
	int32_t get(const std::string& id) const;
	Resources getResources() const;
private:
	Resources resources;

	void drawEverything(sf::RenderTarget& target, sf::RenderStates states) const;
};