#ifndef NETWORKMANAGER_HPP
#define NETWORKMANAGER_HPP

#include <SFML/Network.hpp>
#include <string>

namespace zifmann {
namespace chess {
namespace network {

class NetworkManager {
   public:
    NetworkManager* GetInstance();
    bool Connect(const std::string& address, int port);
    void writeFrame();
    void readFrame();

   private:
    static NetworkManager* m_instance;
    sf::TcpSocket m_socket;

   private:
    NetworkManager();
};

}  // namespace network
}  // namespace chess
}  // namespace zifmann

#endif
