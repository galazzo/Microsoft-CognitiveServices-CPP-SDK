#include "microsoft/cognitive/cv/cv.h"

void Microsoft::CognitiveServices::ComputerVision::ToFile(std::string filename, Image& image) {

    ofstream fout;

    fout.open(filename, ios::binary | ios::out);

    fout.write((char*) image.buffer, image.size);

    fout.close();
}
