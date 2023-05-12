#ifndef NETWORK_PLAYER_HPP
#define NETWORK_PLAYER_HPP

#include <string>

namespace zifmann::chess::network {

class NetworkPlayer {
   public:
    static std::string PLAYER_USERNAME;
    static bool PLAYER_WHITE_PIECES;
};

}  // namespace zifmann::chess::network

#endif
