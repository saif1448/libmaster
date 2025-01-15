#include <iostream>
#include <sstream>
#include <algorithm> // For std::remove_if
#include "Video.h"

void printUsage()
{
    std::cerr << "Usage: ./runme [input] [output] [-S/-M] [function] [options]" << std::endl;
    std::cerr << "Functions:" << std::endl;
    std::cerr << "  reverse" << std::endl;
    std::cerr << "  swap_channel [channel1],[channel2]" << std::endl;
    std::cerr << "  clip_channel [channel] [min,max]" << std::endl;
    std::cerr << "  scale_channel [channel] [scaleFactor]" << std::endl;
}

// Helper function to remove square brackets from a string
std::string removeSquareBrackets(const std::string &str)
{
    std::string result = str;
    result.erase(std::remove_if(result.begin(), result.end(), [](char c)
                                { return c == '[' || c == ']'; }),
                 result.end());
    return result;
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        printUsage();
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    std::string optimizationFlag = argv[3];
    std::string functionName = argv[4];

    Video video(inputFile);

    try
    {
        if (optimizationFlag == "-M")
        {
            if (functionName == "reverse")
            {
                video.reverse(outputFile);
            }
            else if (functionName == "swap_channel")
            {
                if (argc < 6)
                {
                    printUsage();
                    return 1;
                }
                std::string channels = argv[5];
                std::istringstream iss(channels);
                std::string channel1Str, channel2Str;
                std::getline(iss, channel1Str, ',');
                std::getline(iss, channel2Str, ',');
                unsigned char channel1 = std::stoi(channel1Str);
                unsigned char channel2 = std::stoi(channel2Str);
                video.swapChannels(channel1, channel2, outputFile);
            }
            else if (functionName == "clip_channel")
            {
                if (argc < 7)
                {
                    printUsage();
                    return 1;
                }
                unsigned char channel = std::stoi(argv[5]);
                std::string range = removeSquareBrackets(argv[6]); // Remove square brackets
                std::istringstream iss(range);
                std::string minStr, maxStr;
                std::getline(iss, minStr, ',');
                std::getline(iss, maxStr, ',');
                unsigned char min = std::stoi(minStr);
                unsigned char max = std::stoi(maxStr);
                video.clipChannel(channel, min, max, outputFile);
            }
            else if (functionName == "scale_channel")
            {
                if (argc < 7)
                {
                    printUsage();
                    return 1;
                }
                unsigned char channel = std::stoi(argv[5]);
                float scaleFactor = std::stof(argv[6]);
                video.scaleChannel(channel, scaleFactor, outputFile);
            }
            else
            {
                printUsage();
                return 1;
            }
        }
        else if (optimizationFlag == "-S")
        {
            if (functionName == "reverse")
            {
                video.reverseMultiThreaded(outputFile);
            }
            else if (functionName == "swap_channel")
            {
                if (argc < 6)
                {
                    printUsage();
                    return 1;
                }
                std::string channels = argv[5];
                std::istringstream iss(channels);
                std::string channel1Str, channel2Str;
                std::getline(iss, channel1Str, ',');
                std::getline(iss, channel2Str, ',');
                unsigned char channel1 = std::stoi(channel1Str);
                unsigned char channel2 = std::stoi(channel2Str);
                video.swapChannelsMultiThreaded(channel1, channel2, outputFile);
            }
            else if (functionName == "clip_channel")
            {
                if (argc < 7)
                {
                    printUsage();
                    return 1;
                }
                unsigned char channel = std::stoi(argv[5]);
                std::string range = removeSquareBrackets(argv[6]); // Remove square brackets
                std::istringstream iss(range);
                std::string minStr, maxStr;
                std::getline(iss, minStr, ',');
                std::getline(iss, maxStr, ',');
                unsigned char min = std::stoi(minStr);
                unsigned char max = std::stoi(maxStr);
                video.clipChannelMultiThreaded(channel, min, max, outputFile);
            }
            else if (functionName == "scale_channel")
            {
                if (argc < 7)
                {
                    printUsage();
                    return 1;
                }
                unsigned char channel = std::stoi(argv[5]);
                float scaleFactor = std::stof(argv[6]);
                video.scaleChannelMultiThreaded(channel, scaleFactor, outputFile);
            }
            else
            {
                printUsage();
                return 1;
            }
        }
        else
        {
            printUsage();
            return 1;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}