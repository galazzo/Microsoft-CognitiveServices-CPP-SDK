#ifndef COGNITIVE
#define COGNITIVE

#include <string>
#include <array>
#include <fstream>

using namespace std;

namespace Microsoft {
    namespace CognitiveServices {

        enum ApiServerRegion {
            West_US,
            West_US_2,
            East_US,
            East_US_2,
            West_Central_US,
            South_Central_US,
            West_Europe,
            North_Europe,
            Southeast_Asia,
            East_Asia,
            Australia_East,
            Brazil_South,
            Canada_Central,
            Central_India,
            UK_South,
            Japan_East
        };

        enum Language {
            unk,
            zh_Hans,
            zh_Hant,
            cs,
            da,
            nl,
            en,
            fi,
            fr,
            de,
            el,
            hu,
            it,
            ja,
            ko,
            nb,
            pl,
            pt,
            ru,
            es,
            sv,
            tr,
            ar,
            ro,
            sr_Cyrl,
            sr_Latn,
            sk
        };

        std::string ApiServer(ApiServerRegion);

        using BoundingBox = std::array<int, 8>;

        void ToFile(std::string filename, ios_base::openmode mode, const char* buffer, int size);

    }
}

#endif
