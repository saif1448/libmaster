#include <iostream>
#include "Video.h"

using namespace std;

int main()
{
    string inputBinPath = "path_to_video.bin";  // Path to your input video binary file
    string outputBinPath = "swapped_video.bin"; // Path to save the modified video binary file

    // Create a Video object from the binary file
    Video video(inputBinPath);

    // Load frames from the binary file into the Video object
    video.loadFromBin(inputBinPath);

    // Swap channels in the video (example: swap Red (0) and Green (1))
    video.swap_channel(0, 1); // Swapping Red and Green channels (0 for Red, 1 for Green)

    // Save the modified video to a new .bin file
    video.saveToCombinedBin(outputBinPath);

    cout << "Channel swap completed. The modified video has been saved to: " << outputBinPath << endl;

    return 0;
}
