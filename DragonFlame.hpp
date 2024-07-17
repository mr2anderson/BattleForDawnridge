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


#include "CameraIndependentPopUpElement.hpp"
#include "DragonFlamePreview.hpp"


#pragma once


class DragonFlame : public CameraIndependentPopUpElement {
public:
	DragonFlame();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
	void run(uint32_t windowW, uint32_t windowH) override;
	Events click() override;
	void update() override;
	uint32_t getXMin() const;
	uint32_t getXMax() const;
	uint32_t getYMin() const;
	uint32_t getYMax() const;
	void setCenterX(uint32_t newCenterX);
	void setCenterY(uint32_t newCenterY);

	std::shared_ptr<DragonFlamePreview> getPreview() const;
protected:
	virtual std::shared_ptr<DragonFlamePreview> getPreviewType() const = 0;
	virtual uint32_t getRadius() const = 0;
	virtual float getAnimationTime() const = 0;
private:
	uint32_t centerX, centerY;

	uint32_t windowW, windowH;
	sf::Clock timer;

	sf::Color getCurrentColor() const;
};