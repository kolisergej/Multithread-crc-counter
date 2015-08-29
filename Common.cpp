#include "Common.h"

static const unsigned short crc_tbl[16] =
{
    0x0000, 0x1081, 0x2102, 0x3183,
    0x4204, 0x5285, 0x6306, 0x7387,
    0x8408, 0x9489, 0xa50a, 0xb58b,
    0xc60c, 0xd68d, 0xe70e, 0xf78f
};

unsigned short qChecksum(const char* pData, unsigned int length)
{
    unsigned short crc = 0xffff;
    unsigned char c;
    const unsigned char* p = reinterpret_cast<const unsigned char*>(pData);
    while (length--)
    {
        c = *p++;
        crc = ((crc >> 4) & 0x0fff) ^ crc_tbl[((crc ^ c) & 15)];
        c >>= 4;
        crc = ((crc >> 4) & 0x0fff) ^ crc_tbl[((crc ^ c) & 15)];
    }
    return ~crc & 0xffff;
}

bool isFileExists(const std::string& name)
{
    ifstream file(name.c_str());
    const bool isExists = file.good();
    file.close();
    return isExists;
}

void dumpResults(const char* pOutputFileName, const vector<vector<unsigned short> >& result, size_t threadCount)
{
    string outputFileName(pOutputFileName);
    ofstream outputFileHandler(outputFileName);
    for (size_t i = 0; i < result[i].size(); ++i)
    {
        for (size_t j = 0; j < threadCount; ++j)
        {
            if (!result[j].empty())
            {
                outputFileHandler << result[j][i] << " ";
            }
        }
    }

    if (result[0].size() > 1)
    {
        for (size_t i = 0; i < threadCount; ++i)
        {
            if (result[i].size() == result[0].size())
            {
                outputFileHandler << result[i][result[i].size() - 1] << " ";
            }
        }
    }
    outputFileHandler.close();
}
