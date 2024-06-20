#include <vector>
#include <string>
#include <map>

struct Client_info; // Forward declaration

struct Channel {
    std::string name;
    std::vector<clientInfo*> users; // Pointers to users in the channel
};
