#ifndef NETWORKMANAGER_HPP
#define NETWORKMANAGER_HPP

#include <SFML/Network.hpp>
#include <deque>
#include <string>

#include "message.hpp"

namespace zifmann {
namespace chess {
namespace network {

class NetworkManager {
   public:
    NetworkManager* GetInstance();
    bool Connect(const std::string& address, int port);
    void SendMessage(const OutgoingMessage&);
    void ReadMessage();
    void UpdateRead();
    void UpdateWrite();

   private:
    static NetworkManager* m_instance;
    sf::TcpSocket m_socket;

    std::deque<OutgoingMessage> outgoingQueue;
    std::deque<IncomingMessage> incomingQueue;

    size_t writtenBytes;
    size_t readBytes;
    size_t remainingWrite;
    size_t remainingRead;

    char* writeBuffer;

    std::string readBuffer;

   private:
    NetworkManager();
};

}  // namespace network
}  // namespace chess
}  // namespace zifmann

#endif
