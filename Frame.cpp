#include "Frame.h"
#include <iostream>
#include <fstream>

using namespace std;

// Constructor: Initializes the 3D vector for RGB values
Frame::Frame(int w, int h) : width(w), height(h)
{
    rgbValues.resize(height, vector<vector<unsigned char>>(width, vector<unsigned char>(3)));
}

// Method to store RGB values for each pixel
void Frame::setRGB(int row, int col, unsigned char r, unsigned char g, unsigned char b)
{
    rgbValues[row][col][0] = r; // Red
    rgbValues[row][col][1] = g; // Green
    rgbValues[row][col][2] = b; // Blue
}

// Method to output RGB values
void Frame::printRGB() const
{
    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            unsigned char r = rgbValues[row][col][0];
            unsigned char g = rgbValues[row][col][1];
            unsigned char b = rgbValues[row][col][2];
            cout << "Pixel (" << row << ", " << col << "): "
                 << "R=" << static_cast<int>(r) << ", "
                 << "G=" << static_cast<int>(g) << ", "
                 << "B=" << static_cast<int>(b) << endl;
        }
    }
}

// Method to extract RGB values from a .bin file and populate the Frame object
void Frame::extractRGB(const vector<unsigned char> &buffer)
{
    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            int index = (row * width + col) * 3;
            unsigned char r = buffer[index];
            unsigned char g = buffer[index + 1];
            unsigned char b = buffer[index + 2];

            setRGB(row, col, r, g, b);
        }
    }
}

// Static method to extract frames from a combined .bin file
vector<Frame> Frame::extractFrames(const string &inputBinPath, int width, int height, int totalFrames)
{
    vector<Frame> frames;
    frames.reserve(totalFrames);

    ifstream file(inputBinPath, ios::binary);
    if (!file)
    {
        cerr << "Error: Unable to open .bin file!" << endl;
        return frames;
    }

    // Calculate the size of a single frame (width * height * 3)
    size_t frameSize = width * height * 3;

    for (int i = 0; i < totalFrames; ++i)
    {
        Frame frame(width, height);
        vector<unsigned char> buffer(frameSize);

        // Read the frame's pixel data from the file
        file.read(reinterpret_cast<char *>(buffer.data()), frameSize);

        if (file.gcount() != frameSize)
        {
            cerr << "Error: File size does not match expected number of frames!" << endl;
            break;
        }

        // Populate the Frame object with RGB values
        for (int row = 0; row < height; ++row)
        {
            for (int col = 0; col < width; ++col)
            {
                // Calculate the index for the pixel in the buffer
                int index = (row * width + col) * 3;
                unsigned char r = buffer[index];
                unsigned char g = buffer[index + 1];
                unsigned char b = buffer[index + 2];

                // Set the RGB values in the Frame object
                frame.setRGB(row, col, r, g, b);
            }
        }

        // Add the frame to the vector
        frames.push_back(frame);
    }

    return frames;
}
