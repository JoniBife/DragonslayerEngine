
#ifndef FLOAT_ARRAY_FILE
#define FLOAT_ARRAY_FILE

#include <string>
#include <fstream>
#include <cmath>

namespace fa {

    static float* loadFromFile(const std::string& inputPath, unsigned int length) {
        float* data = new float[length];

        std::ifstream file(inputPath, std::ios_base::binary);
        if (!file.read((char*)data, sizeof(float) * (length)))
            return nullptr;
        return data;
    }

    static float* reverseData(float* data, unsigned int length) {

        unsigned int j = length - 1;
        for (unsigned int i = 0; i < (unsigned int)floor(length / 2); i += 3) {

            float startR = data[i];
            float startG = data[i + 1];
            float startB = data[i + 2];

            float endR = data[j - 2];
            float endG = data[j - 1];
            float endB = data[j];

            data[i] = endR;
            data[i + 1] = endG;
            data[i + 2] = endB;

            data[j - 2] = startR;
            data[j - 1] = startG;
            data[j] = startB;

            j = length - i - 1;
        }

        return data;
    }

    // TODO Add some kind of compression to reduce file size
    static void writeToFile(float* data, unsigned int length, const std::string& outputPath, bool reverseOnWrite) {

        if (reverseOnWrite)
            data = reverseData(data, length);

        // Writing the whole array in binary format
        std::ofstream file(outputPath, std::ios_base::binary);
        file.write((char*)data, sizeof(float) * length);
        file.close();

        delete[] data;
    }

}


#endif
