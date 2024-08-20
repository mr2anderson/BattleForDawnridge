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


#include <SFML/Network.hpp>
#include <functional>
#include <queue>
#include <atomic>
#include <optional>
#include <mutex>


#pragma once




namespace bfdlib {




    class tcp_helper {
    public:


        template<typename T> class safe_writing_queue {
        public:
            safe_writing_queue() = default;

            void push(const T& value) {
                std::lock_guard<std::mutex> lock(this->mutex);
                this->data.push(value);
            }
        private:
            std::queue<T> data;
            mutable std::mutex mutex;

            std::optional<T> pop() {
                std::lock_guard<std::mutex> lock(this->mutex);
                if (this->data.empty()) {
                    return std::nullopt;
                }
                T value = this->data.front();
                this->data.pop();
                return value;
            }

            friend class tcp_helper;
        };




        template<typename T> class safe_reading_queue {
        public:
            safe_reading_queue() = default;

            std::optional<T> pop() {
                std::lock_guard<std::mutex> lock(this->mutex);
                if (this->data.empty()) {
                    return std::nullopt;
                }
                T value = this->data.front();
                this->data.pop();
                return value;
            }
        private:
            std::queue<T> data;
            mutable std::mutex mutex;

            void push(const T& value) {
                std::lock_guard<std::mutex> lock(this->mutex);
                this->data.push(value);
            }

            friend class tcp_helper;
        };



        typedef safe_writing_queue<sf::Packet> queue_w;
        typedef safe_reading_queue<sf::Packet> queue_r;



        static void process_sending(sf::TcpSocket &socket, queue_w &q, std::atomic<bool> &flag, std::atomic<uint64_t> &bytes) {
            for (; ; sf::sleep(sf::milliseconds(5))) {
                if (flag) {
                    break;
                }
                std::optional<sf::Packet> packet = q.pop();
                if (packet == std::nullopt) {
                    continue;
                }
                bytes = bytes + packet->getDataSize();
                while (socket.send(packet.value()) != sf::Socket::Status::Done) {
                    sf::sleep(sf::milliseconds(5));
                    if (flag) {
                        break;
                    }
                }
            }
        }
        static void process_receiving(sf::TcpSocket &socket, queue_r &q, std::atomic<bool> &flag, std::atomic<uint64_t> &bytes) {
            for (; ; sf::sleep(sf::milliseconds(5))) {
                if (flag) {
                    break;
                }
                sf::Packet packet;
                while (socket.receive(packet) != sf::Socket::Status::Done) {
                    sf::sleep(sf::milliseconds(5));
                    if (flag) {
                        break;
                    }
                }
                bytes = bytes + packet.getDataSize();
                q.push(packet);
            }
        }
    };


}