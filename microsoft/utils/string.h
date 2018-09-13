#ifndef STRING_H
#define STRING_H

#include <string>
#include <vector>

using namespace std;

namespace Microsoft {
    namespace CognitiveServices {
        namespace Utils {
            namespace String {
                std::vector<std::string> Split(const std::string& s, char delimiter);
            }
        }
    }
}

#endif // STRING_H
