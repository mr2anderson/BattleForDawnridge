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


#include "Maps.hpp"


Maps* Maps::singletone = nullptr;


const uint32_t Maps::THUMBNAIL_SIZE = 64;


void Maps::add(const std::string &name, const std::string &path) {
    this->paths[name] = path;
    this->generateThumbnail(name);
}
std::shared_ptr<Map> Maps::load(const std::string& name) {
    std::shared_ptr<Map> map = std::make_shared<Map>(this->paths[name]);
    return map;
}
void Maps::generateThumbnail(const std::string& name) {
    std::shared_ptr<Map> map = this->load(name);

    float sx = std::min((float)THUMBNAIL_SIZE / (float)map->getW(), (float)THUMBNAIL_SIZE / (float)map->getH());
    float sy = sx;
    float scaleX = sx / 32.f;
    float scaleY = scaleX;

    sf::RenderTexture renderTexture;
    renderTexture.create(THUMBNAIL_SIZE, THUMBNAIL_SIZE);

    PlainsGeneration generation = PlainsGeneration(THUMBNAIL_SIZE / 32, THUMBNAIL_SIZE / 32);
    for (uint32_t i = 0; i < THUMBNAIL_SIZE / 32; i = i + 32) {
        for (uint32_t j = 0; j < THUMBNAIL_SIZE / 32; j = j + 32) {
            sf::Sprite sprite;
            sprite.setTexture(*Textures::get()->get(std::to_string(generation.getType(i, j) + 1)));
            sprite.setPosition(32 * i, THUMBNAIL_SIZE - 32 * j);
            renderTexture.draw(sprite);
        }
    }

    std::shared_ptr<GOCollection<GO>> go = map->getGO();
    for (uint32_t i = 0; i < go->size(); i = i + 1) {
        GO* o = go->at(i);
        float x = o->getX() * sx;
        float y = o->getY() * sy;
        sf::Sprite sprite;
        sprite.setTexture(*Textures::get()->get(o->getTextureName()));
        sprite.setPosition(x, THUMBNAIL_SIZE - y);
        sprite.setScale(scaleX, scaleY);
        renderTexture.draw(sprite);
    }

    Textures::get()->add(name, renderTexture.getTexture());
}