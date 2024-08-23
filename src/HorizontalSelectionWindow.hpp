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


#include <boost/serialization/vector.hpp>
#include "CameraIndependentPopUpElement.hpp"
#include "Button.hpp"
#include "HorizontalSelectionWindowComponent.hpp"
#include "LabelWithImage.hpp"
#include "HorizontalSelectionWindowStructure.hpp"


#pragma once


class HorizontalSelectionWindow : public CameraIndependentPopUpElement {
public:
	HorizontalSelectionWindow();
	HorizontalSelectionWindow(const std::vector<HorizontalSelectionWindowComponent>& components, uint32_t componentSize = 64, uint32_t marginSize = 10);

	void onRestart() override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	Events click(uint32_t mouseX, uint32_t mouseY, uint32_t windowW, uint32_t windowH) override;
    void moveUp();
    void moveDown();
private:
	std::vector<HorizontalSelectionWindowComponent> components;
	uint32_t componentSize, marginSize;
	uint32_t offset;

	bool possibleToMoveUp(uint32_t componentsInFrame) const;
	bool possibleToMoveDown() const;

	uint32_t getComponentsInFrame(uint32_t windowW, uint32_t windowH) const;
    HorizontalSelectionWindowStructure getUIStructure(uint32_t windowW, uint32_t windowH) const;
	HorizontalSelectionWindowStructure getStructure(uint32_t windowW, uint32_t windowH);

	friend class boost::serialization::access;
	template<class Archive> void serialize(Archive& ar, const unsigned int version) {
		ar& boost::serialization::base_object<CameraIndependentPopUpElement>(*this);
		ar& this->components;
		ar& this->componentSize;
		ar& this->marginSize;
		ar& this->offset;
	}
};


BOOST_CLASS_EXPORT_KEY(HorizontalSelectionWindow)