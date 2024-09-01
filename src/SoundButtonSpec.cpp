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


#include "SoundButtonSpec.hpp"
#include "HorizontalSelectionWindow.hpp"
#include "CreateEEvent.hpp"
#include "PlaySoundEvent.hpp"
#include "WindowEntry.hpp"
#include "WindowButton.hpp"
#include "ChangeSoundVolumeEvent.hpp"
#include "ChangeMusicVolumeEvent.hpp"
#include "EntryNumSettings.hpp"
#include "ColorTheme.hpp"


SoundButtonSpec::SoundButtonSpec() : SoundButtonSpec(0) {

}
SoundButtonSpec::SoundButtonSpec(uint32_t index) : SoundButtonSpec(index, 0, 0) {

}
SoundButtonSpec::SoundButtonSpec(uint32_t index, uint32_t currentSoundVolume, uint32_t currentMusicVolume) : MenuButtonSpec(index) {
    this->currentSoundVolume = currentSoundVolume;
    this->currentMusicVolume = currentMusicVolume;
}
StringLcl SoundButtonSpec::getString() const {
    return { "{sound}" };
}
Events SoundButtonSpec::getEvents() const {
    Events clickEvent;
    clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

    std::shared_ptr<std::string> soundVolume = std::make_shared<std::string>();
    std::shared_ptr<EntryNumSettings> soundVolumeWindowSettings = std::make_shared<EntryNumSettings>();
    soundVolumeWindowSettings->setMax(100);
    std::shared_ptr<WindowButton> soundVolumeWasChangedWindow = std::make_shared<WindowButton>(StringLcl("{sound_volume_was_changed}"), StringLcl("{OK}"), clickEvent);
    Events changeSoundVolumeEvent = clickEvent;
    changeSoundVolumeEvent.add(std::make_shared<ChangeSoundVolumeEvent>(soundVolume));
    changeSoundVolumeEvent.add(std::make_shared<CreateEEvent>(soundVolumeWasChangedWindow));
    std::shared_ptr<WindowEntry> soundVolumeWindow = std::make_shared<WindowEntry>(StringLcl("{enter_sound_volume}"), StringLcl("{change}"), StringLcl("{cancel}"), soundVolume, soundVolumeWindowSettings, changeSoundVolumeEvent, clickEvent);
    Events createSoundVolumeWindowEvent = clickEvent;
    createSoundVolumeWindowEvent.add(std::make_shared<CreateEEvent>(soundVolumeWindow));

    std::shared_ptr<std::string> musicVolume = std::make_shared<std::string>();
    std::shared_ptr<EntryNumSettings> musicVolumeWindowSettings = std::make_shared<EntryNumSettings>();
    musicVolumeWindowSettings->setMax(100);
    std::shared_ptr<WindowButton> musicVolumeWasChangedWindow = std::make_shared<WindowButton>(StringLcl("{music_volume_was_changed}"), StringLcl("{OK}"), clickEvent);
    Events changeMusicVolumeEvent = clickEvent;
    changeMusicVolumeEvent.add(std::make_shared<ChangeMusicVolumeEvent>(musicVolume));
    changeMusicVolumeEvent.add(std::make_shared<CreateEEvent>(musicVolumeWasChangedWindow));
    std::shared_ptr<WindowEntry> musicVolumeWindow = std::make_shared<WindowEntry>(StringLcl("{enter_music_volume}"), StringLcl("{change}"), StringLcl("{cancel}"), musicVolume, musicVolumeWindowSettings, changeMusicVolumeEvent, clickEvent);
    Events createMusicVolumeWindowEvent = clickEvent;
    createMusicVolumeWindowEvent.add(std::make_shared<CreateEEvent>(musicVolumeWindow));

    std::vector<HorizontalSelectionWindowComponent> soundWindowComponents;
    soundWindowComponents.emplace_back(
        "exit_icon",
        StringLcl("{exit}"),
        true,
        clickEvent
    );
    soundWindowComponents.emplace_back(
        "sound_icon",
        StringLcl("{current_sound_volume}") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_NEUTRAL) + std::to_string(this->currentSoundVolume) + "%. {you_can_change_it_if_you_want}",
        true,
        createSoundVolumeWindowEvent
    );
    soundWindowComponents.emplace_back(
        "music_icon",
        StringLcl("{current_music_volume}") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_NEUTRAL) + std::to_string(this->currentMusicVolume) + "%. {you_can_change_it_if_you_want}",
        true,
        createMusicVolumeWindowEvent
    );

    std::shared_ptr<HorizontalSelectionWindow> soundWindow = std::make_shared<HorizontalSelectionWindow>(soundWindowComponents);
    Events createSoundWindowEvent = clickEvent;
    createSoundWindowEvent.add(std::make_shared<CreateEEvent>(soundWindow));

    return createSoundWindowEvent;
}