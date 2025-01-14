#ifndef VIDEO_H
#define VIDEO_H

#include <fstream>
#include <vector>
#include <string>

class Video
{
public:
    Video(const std::string &filename);
    ~Video();

    // Getter methods
    long getNumFrames() const;
    unsigned char getChannels() const;
    unsigned char getHeight() const;
    unsigned char getWidth() const;
    // Getter for frameData
    const std::vector<std::vector<std::vector<std::vector<unsigned char>>>> &getFrameData() const;

    // Operations that create a new output file

    //Single Threaded
    void reverse(const std::string &outputFilename);
    void swapChannels(unsigned char channel1, unsigned char channel2, const std::string &outputFilename);
    void clipChannel(unsigned char channel, unsigned char min, unsigned char max, const std::string &outputFilename);
    void scaleChannel(unsigned char channel, float scaleFactor, const std::string &outputFilename);

    // Multi-threaded operations (OpenMP)
    void reverseMultiThreaded(const std::string &outputFilename);
    void swapChannelsMultiThreaded(unsigned char channel1, unsigned char channel2, const std::string &outputFilename);
    void clipChannelMultiThreaded(unsigned char channel, unsigned char min, unsigned char max, const std::string &outputFilename);
    void scaleChannelMultiThreaded(unsigned char channel, float scaleFactor, const std::string &outputFilename);

    void saveToFile(const std::string &filename);

    void printHeader() const;

private:
    long numFrames;
    unsigned char channels;
    unsigned char height;
    unsigned char width;

    std::vector<std::vector<std::vector<std::vector<unsigned char>>>> frameData;

    void readHeader(std::ifstream &file);    // Declare readHeader
    void readFrameData(std::ifstream &file); // Declare readFrameData

    // Helper method to create a deep copy of frameData
    std::vector<std::vector<std::vector<std::vector<unsigned char>>>> copyFrameData() const;
};

#endif // VIDEO_H