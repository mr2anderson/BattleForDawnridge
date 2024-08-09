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


#include "LanguageButtonSpec.hpp"
#include "ChooseLanguageEvent.hpp"
#include "HorizontalSelectionWindow.hpp"
#include "StaticString.hpp"
#include "CreateEEvent.hpp"
#include "PlaySoundEvent.hpp"


LanguageButtonSpec::LanguageButtonSpec() = default;
LanguageButtonSpec::LanguageButtonSpec(uint32_t index) : MenuButtonSpec(index) {

}
StringLcl LanguageButtonSpec::getString() const {
    return { "{language}" };
}
Events LanguageButtonSpec::getEvents() const {
    Events clickEvent;
    clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

    Events chooseLanguageEnglishEvent;
    chooseLanguageEnglishEvent.add(std::make_shared<ChooseLanguageEvent>("en.cfg"));

    Events chooseLanguageRussianEvent;
    chooseLanguageRussianEvent.add(std::make_shared<ChooseLanguageEvent>("ru.cfg"));

    std::vector<HorizontalSelectionWindowComponent> chooseLanguageWindowComponents;
    chooseLanguageWindowComponents.emplace_back(
        std::make_shared<StaticString>("exit_icon"),
        StringLcl("{exit}"),
        true,
        clickEvent
    );
    chooseLanguageWindowComponents.emplace_back(
        std::make_shared<StaticString>("english_icon"),
        StringLcl("English"),
        true,
        chooseLanguageEnglishEvent
    );
    chooseLanguageWindowComponents.emplace_back(
        std::make_shared<StaticString>("russian_icon"),
        StringLcl("Russian"),
        true,
        chooseLanguageRussianEvent
    );

    std::shared_ptr<HorizontalSelectionWindow> chooseLanguageWindow = std::make_shared<HorizontalSelectionWindow>(chooseLanguageWindowComponents);
    Events createChooseLanguageWindowEvent = clickEvent;
    createChooseLanguageWindowEvent.add(std::make_shared<CreateEEvent>(chooseLanguageWindow));
    
    return createChooseLanguageWindowEvent;
}