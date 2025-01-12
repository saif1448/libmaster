#include "Video.h"
#include <iostream>

using namespace std;

int main()
{
    // Load the video from the input file
    Video video("scaled.bin");

    // Print the header information
    cout << "Input Video Header Information:" << endl;
    video.printHeader();

    // // Perform operations and save to separate output files
    // video.reverse("reversed.bin"); // Reverse the video and save to "reversed.bin"
    // video.swapChannels(0, 1, "swapped.bin"); // Swap channels 0 and 1 and save to "swapped.bin"
    // video.clipChannel(2, 50, 200, "clipped.bin"); // Clip channel 2 and save to "clipped.bin"
    // video.scaleChannel(0, 1.5f, "scaled.bin"); // Scale channel 0 and save to "scaled.bin"

    // cout << "Operations completed. Output files created." << endl;

    return 0;
}