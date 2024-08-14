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


#include "SpellProducerSpec.hpp"
#include "Building.hpp"
#include "WindowButton.hpp"
#include "Locales.hpp"
#include "CreateEEvent.hpp"
#include "ResetHighlightEvent.hpp"
#include "SubResourcesEvent.hpp"
#include "SetSpellEvent.hpp"
#include "UseSpellEvent.hpp"
#include "FirstTimeTipsTable.hpp"
#include "PlaySoundEvent.hpp"


SpellProducerSpec::SpellProducerSpec() {
	this->spell = nullptr;
}
SpellProducerSpec::~SpellProducerSpec() {
	if (this->spell != nullptr) {
		delete this->spell;
	}
}
Events SpellProducerSpec::getActiveNewMoveEvent(const Building* building, MapState* state) {
	if (this->spell == nullptr or !building->works()) {
		return Events();
	}
	return this->spell->newMove(building);
}
std::vector<BuildingHorizontalSelectionWindowComponent> SpellProducerSpec::getComponents(const Building *building, MapState *state) {
	std::vector<BuildingHorizontalSelectionWindowComponent> components;

	if (building->works()) {
		if (this->spell == nullptr or this->spell->wasUsed()) {
			std::vector<std::shared_ptr<Spell>> toProduce = this->getSpellsToProduce(building->getPlayerId());
			Resources resources = state->getPlayersPtr()->getPlayerPtr(building->getPlayerId())->getResources();
			for (uint32_t i = 0; i < toProduce.size(); i = i + 1) {
				std::shared_ptr<Spell> spell = toProduce.at(i);

				Events clickSoundEvent;
				clickSoundEvent.add(std::make_shared<PlaySoundEvent>("click"));

				Events event;
				event.add(std::make_shared<ResetHighlightEvent>());
				if (resources >= spell->getCost()) {
					std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(StringLcl("{spell_producing_started}"), StringLcl("{OK}"), clickSoundEvent);
					event.add(std::make_shared<PlaySoundEvent>(building->getSoundName()));
					event.add(std::make_shared<SubResourcesEvent>(spell->getCost()));
					event.add(std::make_shared<CreateEEvent>(w));
					event.add(std::make_shared<SetSpellEvent>(this, spell));
				}
				else {
					std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(StringLcl("{no_resources_for_spell_producing}"), StringLcl("{OK}"), clickSoundEvent);
					event = event + clickSoundEvent;
					event.add(std::make_shared<CreateEEvent>(w));
				}

				components.emplace_back(
					HorizontalSelectionWindowComponent(spell->getTextureName(),
                    spell->getDescription() + StringLcl("\n") +
                    StringLcl("{cost}") + spell->getCost().getReadableInfo() + StringLcl(". ") + StringLcl("{time_to_make_spell}") + StringLcl(std::to_string(spell->getCreationTime())),
					true,
					event),
                    false
				);
			}
		}
		else {
			if (!this->spell->wasUsed()) {
				if (this->spell->isReady()) {
					Events useSpellEvent;
					useSpellEvent.add(std::make_shared<UseSpellEvent>(this->spell));

                    Events event;
                    if (FirstTimeTipsTable::get()->wasDisplayed("spell_mode_guide")) {
                        event = useSpellEvent;
                    }
                    else {
                        Events clickSoundEvent;
                        clickSoundEvent.add(std::make_shared<PlaySoundEvent>("click"));

                        FirstTimeTipsTable::get()->markAsDisplayed("spell_mode_guide");
                        std::shared_ptr<WindowButton> guide = std::make_shared<WindowButton>(StringLcl("{spell_mode_guide}"), StringLcl("{OK}"), useSpellEvent);
                        event = event + clickSoundEvent;
                        event.add(std::make_shared<CreateEEvent>(guide));
                    }

					components.emplace_back(
						HorizontalSelectionWindowComponent(spell->getTextureName(),
						spell->getDescription(),
						true,
						event),
                        true
					);
				}
				else {
					components.emplace_back(
						HorizontalSelectionWindowComponent(spell->getTextureName(),
                        StringLcl("{spell_producing_in_progress}") + StringLcl(std::to_string(spell->getCreationMovesLeft())) + StringLcl("\n") +
                        spell->getDescription(),
						false,
						Events()),
                        true
					);
				}
			}
		}
	}
	else {
		components.emplace_back(
			HorizontalSelectionWindowComponent(  "hammer_icon",
			StringLcl("{does_not_produce_spells_if_hp_isnt_full}"),
			false,
			Events()),
            true
		);
	}
	

	return components;
}
boost::optional<BuildingShortInfo> SpellProducerSpec::getShortInfo(const Building* building) const {
	if (!building->works()) {
		return boost::none;
	}

    std::string pictureName;
	std::string text;

    if (this->spell == nullptr or this->spell->wasUsed()) {
        pictureName = "priest_icon";
        text = "...";
    }
    else {
        pictureName = this->spell->getTextureName();
        if (this->spell->isReady()) {
            text = "!";
        }
        else {
            text = std::to_string(this->spell->getCreationMovesLeft());
        }
    }

	return BuildingShortInfo(building->getXInPixels(), building->getYInPixels(), building->getSX(), building->getSY(), pictureName, text);
}
void SpellProducerSpec::setSpell(std::shared_ptr<Spell> newSpell) {
	if (this->spell != nullptr) {
		delete this->spell;
	}
	this->spell = newSpell->clone();
}


BOOST_CLASS_EXPORT_IMPLEMENT(SpellProducerSpec)