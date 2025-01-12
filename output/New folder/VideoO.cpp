#include "Video.h"
#include "Frame.h"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Constructor: Initializes the video with dimensions and frame count from the .bin file
Video::Video(const string &binFilePath)
{
    extractMP4Dimensions(binFilePath);
    loadFromBin(binFilePath);
}

// Method to load frames from a combined .bin file
void Video::loadFromBin(const string &inputBinPath)
{
    ifstream inFile(inputBinPath, ios::binary);
    if (!inFile)
    {
        throw runtime_error("Error: Unable to open binary file!");
    }

    // Skip the header bytes (11 bytes: frame count, channels, width, and height)
    size_t headerSize = 8 + 1 + 1 + 1; // 8 bytes for No. Frames, 1 byte each for Channels, Width, Height
    inFile.seekg(headerSize, ios::beg);

    // Extract frames using the Frame class logic
    frames = Frame::extractFrames(inputBinPath, width, height, totalFrames);

    inFile.close();
}

// Method to modify all frames in the video
void Video::modifyFrames(void (*operation)(Frame &))
{
    for (auto &frame : frames)
    {
        operation(frame);
    }
}

// Method to save the modified frames to a new combined .bin file
void Video::saveToCombinedBin(const string &outputBinPath)
{
    ofstream outFile(outputBinPath, ios::binary);
    if (!outFile)
    {
        throw runtime_error("Error: Unable to open output binary file for writing!");
    }

    // Write the header (Total frames, channels, width, height)
    uint64_t frameCount = static_cast<uint64_t>(totalFrames);
    outFile.write(reinterpret_cast<const char *>(&frameCount), sizeof(frameCount));

    uint8_t channelCount = 3; // RGB
    outFile.write(reinterpret_cast<const char *>(&channelCount), sizeof(channelCount));
    outFile.write(reinterpret_cast<const char *>(&width), sizeof(width));
    outFile.write(reinterpret_cast<const char *>(&height), sizeof(height));

    // Write the frames data
    for (const auto &frame : frames)
    {
        for (int row = 0; row < frame.height; ++row)
        {
            for (int col = 0; col < frame.width; ++col)
            {
                outFile.put(frame.rgbValues[row][col][0]); // Red
                outFile.put(frame.rgbValues[row][col][1]); // Green
                outFile.put(frame.rgbValues[row][col][2]); // Blue
            }
        }
    }

    outFile.close();
}

// Method to reverse the entire video
void Video::reverse()
{
    std::reverse(frames.begin(), frames.end());
}

// Method to swap channels in video frames
void Video::swap_channel(unsigned char channel1, unsigned char channel2)
{
    for (auto &frame : frames)
    {
        for (auto &row : frame.rgbValues)
        {
            for (auto &pixel : row)
            {
                std::swap(pixel[channel1], pixel[channel2]);
            }
        }
    }
}

// Method to clip the range of each pixel in each frame
void Video::clip_channel(unsigned char channel, unsigned char min, unsigned char max)
{
    for (auto &frame : frames)
    {
        for (auto &row : frame.rgbValues)
        {
            for (auto &pixel : row)
            {
                pixel[channel] = clamp(pixel[channel], min, max);
            }
        }
    }
}

// Method to scale the channel values in each frame
void Video::scale_channel(unsigned char channel, float scaleFactor)
{
    for (auto &frame : frames)
    {
        for (auto &row : frame.rgbValues)
        {
            for (auto &pixel : row)
            {
                int newValue = static_cast<int>(pixel[channel] * scaleFactor);
                pixel[channel] = static_cast<unsigned char>(clamp(newValue, 0, 255));
            }
        }
    }
}

// Extract video dimensions and frame count from the .bin file
void Video::extractMP4Dimensions(const string &binFilePath)
{
    // Create a new file without the header
    string outputBinPath = "output_no_header.bin";
    saveWithoutHeader(binFilePath, outputBinPath); // Create the no-header file

    ifstream inFile(outputBinPath, ios::binary); // Load from the no-header file
    if (!inFile)
    {
        throw runtime_error("Error: Unable to open binary file!");
    }

    // Read Number of Frames (8 bytes)
    uint64_t frameCount;
    inFile.read(reinterpret_cast<char *>(&frameCount), sizeof(frameCount));
    totalFrames = static_cast<int>(frameCount); // Store as int for simplicity

    // Read Channels (1 byte)
    uint8_t channelCount;
    inFile.read(reinterpret_cast<char *>(&channelCount), sizeof(channelCount));
    unsigned char channels = static_cast<unsigned char>(channelCount);

    // Read Width (1 byte)
    uint8_t widthValue;
    inFile.read(reinterpret_cast<char *>(&widthValue), sizeof(widthValue));
    width = static_cast<int>(widthValue);

    // Read Height (1 byte)
    uint8_t heightValue;
    inFile.read(reinterpret_cast<char *>(&heightValue), sizeof(heightValue));
    height = static_cast<int>(heightValue);

    inFile.close();

    // Display the extracted values
    cout << "Extracted Video Dimensions:" << endl;
    cout << "  Total Frames: " << totalFrames << endl;
    cout << "  Channels: " << static_cast<int>(channels) << endl;
    cout << "  Width: " << width << endl;
    cout << "  Height: " << height << endl;

    if (channels != 3)
    {
        throw runtime_error("Error: Only 3-channel (RGB) video is supported!");
    }

    // Now, load frames from the no-header file
    loadFromBin(outputBinPath);
}

// Method to create an output file without the first 11 bytes of data
void Video::saveWithoutHeader(const string &inputBinPath, const string &outputBinPath)
{
    ifstream inFile(inputBinPath, ios::binary);
    if (!inFile)
    {
        throw runtime_error("Error: Unable to open binary file for reading!");
    }

    // Skip the first 11 bytes (header bytes)
    inFile.seekg(11, ios::beg);

    // Open the output file
    ofstream outFile(outputBinPath, ios::binary);
    if (!outFile)
    {
        throw runtime_error("Error: Unable to open output binary file for writing!");
    }

    // Read the remaining content and write to the output file
    outFile << inFile.rdbuf();

    inFile.close();
    outFile.close();

    cout << "File saved without header to " << outputBinPath << endl;
}

// Method to save frames to a bin file without the header
void Video::saveToBinWithoutHeader(const string &outputBinPath)
{
    ofstream outFile(outputBinPath, ios::binary);
    if (!outFile)
    {
        throw runtime_error("Error: Unable to open output binary file for writing!");
    }

    // Write the frames data (RGB pixel values) without the header
    for (const auto &frame : frames)
    {
        for (int row = 0; row < frame.height; ++row)
        {
            for (int col = 0; col < frame.width; ++col)
            {
                // Writing RGB data in the correct order for each pixel
                outFile.put(frame.rgbValues[row][col][0]); // Red
                outFile.put(frame.rgbValues[row][col][1]); // Green
                outFile.put(frame.rgbValues[row][col][2]); // Blue
            }
        }
    }

    outFile.close();
}

int clamp(int value, int min, int max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}
