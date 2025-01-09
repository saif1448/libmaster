#ifndef FRAME_H
#define FRAME_H

#include <vector>
#include <cstdint>
#include <string>

using namespace std;

class Frame
{
public:
    int width;
    int height;
    vector<vector<vector<unsigned char>>> rgbValues;

    // Constructor
    Frame(int w, int h);

    // Method to store RGB values for each pixel
    void setRGB(int row, int col, unsigned char r, unsigned char g, unsigned char b);

    // Method to output RGB values
    void printRGB() const;

    // Method to extract RGB values from a .bin file
    void extractRGB(const vector<unsigned char> &buffer);

    // Static method to extract frames from a combined .bin file
    static vector<Frame> extractFrames(const string &inputBinPath, int width, int height, int totalFrames);
};

#endif // FRAME_H
