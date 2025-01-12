#include "VideoN.h"

VideoN::VideoN(const std::string &filename)
{
    readFile(filename);
    extractHeader();
}

void VideoN::readFile(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Unable to open file");
    }

    file.seekg(0, std::ios::end);
    std::streampos size = file.tellg();
    file.seekg(0, std::ios::beg);

    data.resize(size);
    file.read(reinterpret_cast<char *>(data.data()), size);
    file.close();
}

void VideoN::extractHeader()
{
    if (data.size() < 11)
    {
        throw std::runtime_error("File is too small to contain the header");
    }

    totalFrames = *reinterpret_cast<int *>(&data[0]);
    channels = data[8];
    width = data[9];
    height = data[10];

    // Remove the header from the data
    data.erase(data.begin(), data.begin() + 11);
}

void VideoN::removeHeaderAndSave(const std::string &outputFilename)
{
    saveToFile(outputFilename);
}

void VideoN::reverse(const std::string &outputFilename)
{
    std::vector<uint8_t> reversedData(data.size());
    int frameSize = width * height * channels;

    for (int i = 0; i < totalFrames; ++i)
    {
        std::copy(data.begin() + i * frameSize, data.begin() + (i + 1) * frameSize,
                  reversedData.begin() + (totalFrames - i - 1) * frameSize);
    }

    data = reversedData;
    saveToFile(outputFilename);
}

void VideoN::swapChannel(const std::string &outputFilename, unsigned char channel1, unsigned char channel2)
{
    int frameSize = width * height * channels;

    for (int i = 0; i < totalFrames; ++i)
    {
        for (int j = 0; j < frameSize; j += channels)
        {
            std::swap(data[i * frameSize + j + channel1], data[i * frameSize + j + channel2]);
        }
    }

    saveToFile(outputFilename);
}

void VideoN::clipChannel(const std::string &outputFilename, unsigned char channel, unsigned char minVal, unsigned char maxVal)
{
    int frameSize = width * height * channels;

    for (int i = 0; i < totalFrames; ++i)
    {
        for (int j = channel; j < frameSize; j += channels)
        {
            data[i * frameSize + j] = std::max(minVal, std::min(data[i * frameSize + j], maxVal));
        }
    }

    saveToFile(outputFilename);
}

void VideoN::scaleChannel(const std::string &outputFilename, unsigned char channel, float scaleFactor)
{
    int frameSize = width * height * channels;

    for (int i = 0; i < totalFrames; ++i)
    {
        for (int j = channel; j < frameSize; j += channels)
        {
            data[i * frameSize + j] = static_cast<uint8_t>(std::min(255.0f, data[i * frameSize + j] * scaleFactor));
        }
    }

    saveToFile(outputFilename);
}

void VideoN::saveToFile(const std::string &outputFilename)
{
    std::ofstream file(outputFilename, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Unable to open output file");
    }

    file.write(reinterpret_cast<const char *>(data.data()), data.size());
    file.close();
}
