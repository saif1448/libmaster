#ifndef VIDEO_H
#define VIDEO_H

#include <vector>
#include <string>
#include "Frame.h" // Include the Frame class

using namespace std;

class Video
{
public:
    vector<Frame> frames; // List of frames in the video
    int width;            // Width of each frame
    int height;           // Height of each frame
    int totalFrames;      // Total number of frames in the video

    // Constructor: Initializes the video with dimensions and frame count from the .bin file
    Video(const string &binFilePath);

    // Method to load frames from a combined .bin file
    void loadFromBin(const string &inputBinPath);

    // Method to modify all frames in the video (operation as a placeholder for future operations)
    void modifyFrames(void (*operation)(Frame &));

    // Method to save the modified frames to a new combined .bin file
    void saveToCombinedBin(const string &outputBinPath);

    // Method to reverse the entire video
    void reverse();

    // Method to swap channels in video frames
    void swap_channel(unsigned char channel1, unsigned char channel2);

    // Method to clip the range of each pixel in each frame
    void clip_channel(unsigned char channel, unsigned char min, unsigned char max);

    // Method to scale the channel values in each frame
    void scale_channel(unsigned char channel, float scaleFactor);
    
    // Extract video dimensions and frame count from the .bin file
    void extractMP4Dimensions(const string &binFilePath);
};

#endif // VIDEO_H
