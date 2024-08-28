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


#include <boost/optional.hpp>
#include <memory>
#include "CameraIndependentPopUpElement.hpp"
#include "Label.hpp"
#include "Button.hpp"
#include "IEntrySettings.hpp"


#pragma once


class WindowEntry : public CameraIndependentPopUpElement {
public:
    WindowEntry();
    WindowEntry(const StringLcl& message, const StringLcl &buttonText1, const StringLcl &buttonText2, std::shared_ptr<std::string> dst, std::shared_ptr<IEntrySettings> settings, const Events &events1, const Events &events2, uint32_t w = 400, uint32_t h = 225);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    Events click(uint32_t mouseX, uint32_t mouseY, uint32_t windowW, uint32_t windowH) override;
    void keyPressed(sf::Keyboard::Key key) override;
private:
    uint32_t w, h;
    StringLcl message;
    StringLcl buttonText1, buttonText2;
    Events events1, events2;
    std::shared_ptr<std::string> dst;
    std::shared_ptr<IEntrySettings> settings;

    Label getLabel(uint32_t windowW, uint32_t windowH) const;
    Button getButton1(uint32_t windowW, uint32_t windowH) const;
    Button getButton2(uint32_t windowW, uint32_t windowH) const;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive& ar, const unsigned int version) {
        ar& boost::serialization::base_object<CameraIndependentPopUpElement>(*this);
        ar& this->w;
        ar& this->h;
        ar& this->message;
        ar& this->buttonText1;
        ar & this->buttonText2;
    }
};


BOOST_CLASS_EXPORT_KEY(WindowEntry)