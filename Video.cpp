#include "Video.h"
#include <iostream>
#include <algorithm> // For std::reverse and std::swap
#include <omp.h>
#include <chrono>

Video::Video(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(1);
    }

    readHeader(file);    // Call readHeader
    readFrameData(file); // Call readFrameData

    file.close();
}

Video::~Video() {}

void Video::readHeader(std::ifstream &file)
{
    file.read(reinterpret_cast<char *>(&numFrames), sizeof(long));
    file.read(reinterpret_cast<char *>(&channels), sizeof(unsigned char));
    file.read(reinterpret_cast<char *>(&height), sizeof(unsigned char));
    file.read(reinterpret_cast<char *>(&width), sizeof(unsigned char));
}

void Video::readFrameData(std::ifstream &file)
{
    frameData.resize(numFrames, std::vector<std::vector<std::vector<unsigned char>>>(
                                    channels, std::vector<std::vector<unsigned char>>(
                                                  height, std::vector<unsigned char>(width))));
    // #pragma omp parallel for
    for (long f = 0; f < numFrames; ++f)
    {
        for (unsigned char c = 0; c < channels; ++c)
        {
            for (unsigned char h = 0; h < height; ++h)
            {
                for (unsigned char w = 0; w < width; ++w)
                {
                    file.read(reinterpret_cast<char *>(&frameData[f][c][h][w]), sizeof(unsigned char));
                }
            }
        }
    }
}

void Video::printHeader() const
{
    std::cout << "Number of Frames: " << numFrames << std::endl;
    std::cout << "Channels: " << static_cast<int>(channels) << std::endl;
    std::cout << "Height: " << static_cast<int>(height) << std::endl;
    std::cout << "Width: " << static_cast<int>(width) << std::endl;
}

// Helper method to create a deep copy of frameData
std::vector<std::vector<std::vector<std::vector<unsigned char>>>> Video::copyFrameData() const
{
    return frameData;
}

// Reverse the video and save to a new file
void Video::reverse(const std::string &outputFilename)
{
    auto copiedData = copyFrameData();                  // Create a copy of the frame data
    std::reverse(copiedData.begin(), copiedData.end()); // Reverse the copied data
    Video outputVideo = *this;                          // Create a copy of the current video object
    outputVideo.frameData = copiedData;                 // Replace the frame data with the reversed data
    outputVideo.saveToFile(outputFilename);             // Save to the output file
}

// void Video::reverseMultiThreaded(const std::string &outputFilename)
// {
//     auto copiedData = copyFrameData(); // Create a copy of the frame data

// // Reverse the copied data in parallel
// #pragma omp parallel for
//     for (long f = 0; f < numFrames; ++f)
//     {
//         std::reverse(copiedData[f].begin(), copiedData[f].end());
//     }

//     Video outputVideo = *this;              // Create a copy of the current video object
//     outputVideo.frameData = copiedData;     // Replace the frame data with the reversed data
//     outputVideo.saveToFile(outputFilename); // Save to the output file
// }

void Video::reverseMultiThreaded(const std::string &outputFilename)
{
    auto copiedData = copyFrameData(); // Create a copy of the frame data

    // Manually reverse the order of the frames in parallel
    auto numFrames = copiedData.size();
    auto *dataPtr = copiedData.data(); // Pointer to the frame data

#pragma omp parallel for
    for (size_t i = 0; i < numFrames / 2; ++i)
    {
        std::swap(dataPtr[i], dataPtr[numFrames - 1 - i]);
    }

    Video outputVideo = *this;              // Create a copy of the current video object
    outputVideo.frameData = copiedData;     // Replace the frame data with the reversed data
    outputVideo.saveToFile(outputFilename); // Save to the output file
}
// Swap channels and save to a new file
void Video::swapChannels(unsigned char channel1, unsigned char channel2, const std::string &outputFilename)
{

    auto start = std::chrono::high_resolution_clock::now();

    auto copiedData = copyFrameData(); // Create a copy of the frame data
    for (long f = 0; f < numFrames; ++f)
    {
        for (unsigned char h = 0; h < height; ++h)
        {
            for (unsigned char w = 0; w < width; ++w)
            {
                std::swap(copiedData[f][channel1][h][w], copiedData[f][channel2][h][w]);
            }
        }
    }
    Video outputVideo = *this;              // Create a copy of the current video object
    outputVideo.frameData = copiedData;     // Replace the frame data with the modified data
    outputVideo.saveToFile(outputFilename); // Save to the output file

    auto end = std::chrono::high_resolution_clock::now();                               // Stop timer
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start); // Calculate duration
    std::cout << "swap took " << duration.count() << " milliseconds." << std::endl;
}

// Multi-threaded swapChannels (OpenMP)
void Video::swapChannelsMultiThreaded(unsigned char channel1, unsigned char channel2, const std::string &outputFilename)
{

    auto start = std::chrono::high_resolution_clock::now();

    auto copiedData = copyFrameData();
// #pragma omp parallel for
#pragma omp parallel for collapse(3)
    for (long f = 0; f < numFrames; ++f)
    {
        for (unsigned char h = 0; h < height; ++h)
        {
            for (unsigned char w = 0; w < width; ++w)
            {
                std::swap(copiedData[f][channel1][h][w], copiedData[f][channel2][h][w]);
            }
        }
    }
    Video outputVideo = *this;
    outputVideo.frameData = copiedData;
    outputVideo.saveToFile(outputFilename);

    auto end = std::chrono::high_resolution_clock::now();                               // Stop timer
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start); // Calculate duration
    std::cout << "swap par took " << duration.count() << " milliseconds." << std::endl;
}

// Clip a channel and save to a new file
void Video::clipChannel(unsigned char channel, unsigned char min, unsigned char max, const std::string &outputFilename)
{
    auto copiedData = copyFrameData(); // Create a copy of the frame data
    for (long f = 0; f < numFrames; ++f)
    {
        for (unsigned char h = 0; h < height; ++h)
        {
            for (unsigned char w = 0; w < width; ++w)
            {
                if (copiedData[f][channel][h][w] < min)
                    copiedData[f][channel][h][w] = min;
                if (copiedData[f][channel][h][w] > max)
                    copiedData[f][channel][h][w] = max;
            }
        }
    }
    Video outputVideo = *this;              // Create a copy of the current video object
    outputVideo.frameData = copiedData;     // Replace the frame data with the modified data
    outputVideo.saveToFile(outputFilename); // Save to the output file
}

// Multi-threaded clipChannel (OpenMP)
void Video::clipChannelMultiThreaded(unsigned char channel, unsigned char min, unsigned char max, const std::string &outputFilename)
{
    auto copiedData = copyFrameData();
// #pragma omp parallel for
#pragma omp parallel for collapse(3)
    for (long f = 0; f < numFrames; ++f)
    {
        for (unsigned char h = 0; h < height; ++h)
        {
            for (unsigned char w = 0; w < width; ++w)
            {
                if (copiedData[f][channel][h][w] < min)
                    copiedData[f][channel][h][w] = min;
                if (copiedData[f][channel][h][w] > max)
                    copiedData[f][channel][h][w] = max;
            }
        }
    }
    Video outputVideo = *this;
    outputVideo.frameData = copiedData;
    outputVideo.saveToFile(outputFilename);
}

// Scale a channel and save to a new file
void Video::scaleChannel(unsigned char channel, float scaleFactor, const std::string &outputFilename)
{
    auto copiedData = copyFrameData(); // Create a copy of the frame data
    for (long f = 0; f < numFrames; ++f)
    {
        for (unsigned char h = 0; h < height; ++h)
        {
            for (unsigned char w = 0; w < width; ++w)
            {
                copiedData[f][channel][h][w] = static_cast<unsigned char>(copiedData[f][channel][h][w] * scaleFactor);
            }
        }
    }
    Video outputVideo = *this;              // Create a copy of the current video object
    outputVideo.frameData = copiedData;     // Replace the frame data with the modified data
    outputVideo.saveToFile(outputFilename); // Save to the output file
}

// Multi-threaded scaleChannel (OpenMP)
void Video::scaleChannelMultiThreaded(unsigned char channel, float scaleFactor, const std::string &outputFilename)
{
    auto copiedData = copyFrameData();
#pragma omp parallel for
    for (long f = 0; f < numFrames; ++f)
    {
        for (unsigned char h = 0; h < height; ++h)
        {
            for (unsigned char w = 0; w < width; ++w)
            {
                copiedData[f][channel][h][w] = static_cast<unsigned char>(copiedData[f][channel][h][w] * scaleFactor);
            }
        }
    }
    Video outputVideo = *this;
    outputVideo.frameData = copiedData;
    outputVideo.saveToFile(outputFilename);
}

void Video::saveToFile(const std::string &filename)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file)
    {
        std::cerr << "Error creating file: " << filename << std::endl;
        exit(1);
    }

    // Write the header
    file.write(reinterpret_cast<char *>(&numFrames), sizeof(long));
    file.write(reinterpret_cast<char *>(&channels), sizeof(unsigned char));
    file.write(reinterpret_cast<char *>(&height), sizeof(unsigned char));
    file.write(reinterpret_cast<char *>(&width), sizeof(unsigned char));

    // Write the frame data in the correct order
    // #pragma omp parallel for
    for (long f = 0; f < numFrames; ++f)
    {
        for (unsigned char c = 0; c < channels; ++c)
        {
            for (unsigned char h = 0; h < height; ++h)
            {
                for (unsigned char w = 0; w < width; ++w)
                {
                    file.write(reinterpret_cast<char *>(&frameData[f][c][h][w]), sizeof(unsigned char));
                }
            }
        }
    }

    file.close();
}

// Getter methods
long Video::getNumFrames() const
{
    return numFrames;
}

unsigned char Video::getChannels() const
{
    return channels;
}

unsigned char Video::getHeight() const
{
    return height;
}

unsigned char Video::getWidth() const
{
    return width;
}

// Getter for frameData
const std::vector<std::vector<std::vector<std::vector<unsigned char>>>> &Video::getFrameData() const
{
    return frameData;
}