#ifndef VIDEON_H
#define VIDEON_H

#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <algorithm>

class VideoN {
public:
    int width;            // Width of each frame
    int height;           // Height of each frame
    int totalFrames;      // Total number of frames
    int channels;         // Number of channels

    VideoN(const std::string& filename);
    void removeHeaderAndSave(const std::string& outputFilename);
    void reverse(const std::string& outputFilename);
    void swapChannel(const std::string& outputFilename, unsigned char channel1, unsigned char channel2);
    void clipChannel(const std::string& outputFilename, unsigned char channel, unsigned char minVal, unsigned char maxVal);
    void scaleChannel(const std::string& outputFilename, unsigned char channel, float scaleFactor);

private:
    std::vector<uint8_t> data;
    void readFile(const std::string& filename);
    void extractHeader();
    void saveToFile(const std::string& outputFilename);
};

#endif // VIDEON_H
