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
					std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(*Locales::get()->get("spell_producing_started"), *Locales::get()->get("OK"), clickSoundEvent);
					event.add(std::make_shared<PlaySoundEvent>(building->getSoundName()));
					event.add(std::make_shared<SubResourcesEvent>(spell->getCost()));
					event.add(std::make_shared<CreateEEvent>(w));
					event.add(std::make_shared<SetSpellEvent>(this, spell));
				}
				else {
					std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(*Locales::get()->get("no_resources_for_spell_producing"), *Locales::get()->get("OK"), clickSoundEvent);
					event = event + clickSoundEvent;
					event.add(std::make_shared<CreateEEvent>(w));
				}

				components.emplace_back(
					HorizontalSelectionWindowComponent(spell->getTextureName(),
                    spell->getDescription() + L"\n" +
                    *Locales::get()->get("cost") + spell->getCost().getReadableInfo() + L". " + *Locales::get()->get("time_to_make_spell") + std::to_wstring(spell->getCreationTime()),
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

					components.emplace_back(
						HorizontalSelectionWindowComponent(spell->getTextureName(),
						spell->getDescription(),
						true,
						useSpellEvent),
                        true
					);
				}
				else {
					components.emplace_back(
						HorizontalSelectionWindowComponent(spell->getTextureName(),
                        *Locales::get()->get("spell_producing_in_progress") + std::to_wstring(spell->getCreationMovesLeft()) + L"\n" +
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
			HorizontalSelectionWindowComponent("hammer_icon",
			*Locales::get()->get("does_not_produce_spells_if_hp_isnt_full"),
			false,
			Events()),
            true
		);
	}
	

	return components;
}
std::optional<BuildingShortInfo> SpellProducerSpec::getShortInfo(const Building* building) const {
	if (this->spell == nullptr or this->spell->wasUsed()) {
		return std::nullopt;
	}

	std::string text;
	
	if (this->spell->isReady()) {
		text = "!";
	}
	else {
		text = std::to_string(this->spell->getCreationMovesLeft());
	}

	return BuildingShortInfo(building->getXInPixels(), building->getYInPixels(), building->getSX(), building->getSY(), this->spell->getTextureName(), text);
}
void SpellProducerSpec::setSpell(std::shared_ptr<Spell> newSpell) {
	if (this->spell != nullptr) {
		delete this->spell;
	}
	this->spell = newSpell->clone();
}