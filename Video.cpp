#include "Video.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

// Constructor: Initializes the video with dimensions from the binary file
Video::Video(const string &binFilePath)
{
    // Load video dimensions and total frames from the binary file
    extractMP4Dimensions(binFilePath);

    // Resize frames based on totalFrames and initialized width and height
    frames.resize(totalFrames, Frame(width, height)); // Resize frames to hold the frames
}

// Method to extract dimensions and frame count from the .bin file
void Video::extractMP4Dimensions(const string &binFilePath)
{
    // Open the .bin file in binary read mode
    ifstream binFile(binFilePath, ios::binary);

    if (!binFile.is_open())
    {
        cerr << "Failed to open the .bin file!" << endl;
        return;
    }

    // Read the entire content of the .bin file into a vector
    vector<unsigned char> binData((istreambuf_iterator<char>(binFile)), istreambuf_iterator<char>());

    // Extract video dimensions and total frame count from the binary file
    width = 369;      // Example, you can adjust this based on actual file parsing
    height = 369;     // Similarly, extract height
    totalFrames = 50; // Extract the number of frames based on the binary structure

    cout << "Extracted Dimensions: " << width << "x" << height << ", Total Frames: " << totalFrames << endl;
}

// Method to load frames from a combined .bin file
void Video::loadFromBin(const string &inputBinPath)
{
    ifstream file(inputBinPath, ios::binary);
    if (!file)
    {
        cerr << "Error: Unable to open .bin file!" << endl;
        return;
    }

    size_t frameDataSize = width * height * 3; // Each frame's data size
    vector<unsigned char> buffer(frameDataSize);

    for (int i = 0; i < totalFrames; ++i)
    {
        file.read(reinterpret_cast<char *>(buffer.data()), frameDataSize);
        if (file.gcount() != frameDataSize)
        {
            cerr << "Error: Failed to read frame " << i + 1 << " data!" << endl;
            return;
        }

        // Process the frame data (buffer) for each frame
        frames[i].extractRGB(buffer);
    }

    cout << "Loaded " << totalFrames << " frames from " << inputBinPath << endl;
}

// Method to modify all frames in the video
void Video::modifyFrames(void (*operation)(Frame &))
{
    for (auto &frame : frames)
    {
        operation(frame); // Apply the operation on each frame
    }
}

// Method to save the modified frames to a new combined .bin file
void Video::saveToCombinedBin(const string &outputBinPath)
{
    ofstream outputFile(outputBinPath, ios::binary);
    if (!outputFile)
    {
        cerr << "Error: Unable to open output .bin file for writing!" << endl;
        return;
    }

    // Write each frame's pixel data to the output file
    for (const auto &frame : frames)
    {
        for (int row = 0; row < frame.height; ++row)
        {
            for (int col = 0; col < frame.width; ++col)
            {
                outputFile.put(frame.rgbValues[row][col][0]); // Red
                outputFile.put(frame.rgbValues[row][col][1]); // Green
                outputFile.put(frame.rgbValues[row][col][2]); // Blue
            }
        }
    }

    cout << "Saved modified video to " << outputBinPath << endl;
}

void Video::reverse()
{
    // Create a new Video object for the reversed frames
    Video reversedVideo("path_to_video.bin"); // Pass the path here

    // Reverse the order of frames and add them to the reversedVideo object
    for (int i = 0; i < totalFrames; ++i)
    {
        reversedVideo.frames[i] = frames[totalFrames - 1 - i]; // Copy frames in reverse order
    }

    // Now, the reversedVideo contains the reversed frames
    // Save the reversed video to a new bin file
    string outputBinPath = "rev_video.bin";
    reversedVideo.saveToCombinedBin(outputBinPath);

    cout << "Video frames reversed and saved to: " << outputBinPath << endl;
}

void Video::swap_channel(unsigned char channel1, unsigned char channel2)
{
    // Check if the channels are valid (only 0, 1, or 2 for RGB)
    if (channel1 > 2 || channel2 > 2)
    {
        cerr << "Error: Invalid channel numbers! Channels must be 0 (Red), 1 (Green), or 2 (Blue)." << endl;
        return;
    }

    // Iterate over all frames in the video
    for (auto &frame : frames)
    {
        // Iterate through each pixel in the frame
        for (int row = 0; row < frame.height; ++row)
        {
            for (int col = 0; col < frame.width; ++col)
            {
                // Swap the channel values for the current pixel
                unsigned char temp = frame.rgbValues[row][col][channel1];
                frame.rgbValues[row][col][channel1] = frame.rgbValues[row][col][channel2];
                frame.rgbValues[row][col][channel2] = temp;
            }
        }
    }

    cout << "Channels " << static_cast<int>(channel1) << " and " << static_cast<int>(channel2) << " swapped for all frames." << endl;
}

void Video::clip_channel(unsigned char channel, unsigned char min, unsigned char max)
{
    // Check if the channel is valid (only 0, 1, or 2 for RGB)
    if (channel > 2)
    {
        cerr << "Error: Invalid channel number! Channel must be 0 (Red), 1 (Green), or 2 (Blue)." << endl;
        return;
    }

    // Validate the range
    if (min > max)
    {
        cerr << "Error: Minimum value cannot be greater than maximum value!" << endl;
        return;
    }

    // Iterate over all frames in the video
    for (auto &frame : frames)
    {
        // Iterate through each pixel in the frame
        for (int row = 0; row < frame.height; ++row)
        {
            for (int col = 0; col < frame.width; ++col)
            {
                // Get the current value for the specified channel
                unsigned char &value = frame.rgbValues[row][col][channel];

                // Clip the value to the specified range
                value = std::max(min, std::min(value, max));
            }
        }
    }

    cout << "Channel " << static_cast<int>(channel)
         << " clipped to range [" << static_cast<int>(min)
         << ", " << static_cast<int>(max) << "] for all frames." << endl;
}

void Video::scale_channel(unsigned char channel, float scaleFactor)
{
    // Check if the channel is valid (only 0, 1, or 2 for RGB)
    if (channel > 2)
    {
        cerr << "Error: Invalid channel number! Channel must be 0 (Red), 1 (Green), or 2 (Blue)." << endl;
        return;
    }

    // Iterate over all frames in the video
    for (auto &frame : frames)
    {
        // Iterate through each pixel in the frame
        for (int row = 0; row < frame.height; ++row)
        {
            for (int col = 0; col < frame.width; ++col)
            {
                // Get the current value for the specified channel
                unsigned char &value = frame.rgbValues[row][col][channel];

                // Scale the value
                int scaledValue = static_cast<int>(value * scaleFactor);

                // Clamp the value to the range [0, 255]
                if (scaledValue < 0)
                    scaledValue = 0;
                else if (scaledValue > 255)
                    scaledValue = 255;

                // Assign the clamped value back
                value = static_cast<unsigned char>(scaledValue);
            }
        }
    }

    cout << "Channel " << static_cast<int>(channel)
         << " scaled by factor " << scaleFactor << " for all frames." << endl;
}
