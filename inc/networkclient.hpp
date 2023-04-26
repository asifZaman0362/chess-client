#ifndef NETWORKMANAGER_HPP
#define NETWORKMANAGER_HPP

#include <SFML/Network.hpp>
#include <deque>
#include <string>
#include <utility>

#include "message.hpp"

namespace zifmann {
namespace chess {
namespace network {

class NetworkManager {
   public:
    NetworkManager() = default;
    static bool Connect(const std::string& address, int port);
    static bool SendMessage(const OutgoingMessage&, bool blocking = false);
    static std::pair<bool, IncomingMessage> ReadMessage();
    static void UpdateRead();
    static void UpdateWrite();

   private:
    static sf::TcpSocket m_socket;

    static std::deque<OutgoingMessage> outgoingQueue;
    static std::deque<IncomingMessage> incomingQueue;

    static size_t writtenBytes;
    static size_t readBytes;
    static size_t remainingWrite;
    static size_t remainingRead;

    static char* writeBuffer;

    static std::string readBuffer;
};

}  // namespace network
}  // namespace chess
}  // namespace zifmann

#endif
