#include <iostream>
#include <thread>
#include <chrono>
#include <stdexcept>

#include "Common.h"

using std::exception;
using std::thread;
using std::cout;
using std::endl;

void work(const std::string& inputFileName, size_t workerNumber, size_t workersCount, int blockSize, vector<unsigned short>& resultVector)
{
    FILE* pFile = fopen(inputFileName.c_str(), "r");
    string str;
    str.resize(blockSize);
    long long iteration = 0;
    while (!feof(pFile))
    {
        fseek(pFile, (workerNumber + iteration * workersCount) * blockSize, SEEK_SET);
        const size_t readBytes = fread(&str[0], sizeof(char), blockSize, pFile);
        if ((long long)readBytes < blockSize)
        {
            str.resize(readBytes);
        }
        if (!str.empty())
        {
            resultVector.push_back(qChecksum(str.c_str(), str.size()));
            ++iteration;
        }
    }
    fclose(pFile);
}

int main(int argc, char** argv)
{
    if (argc == 3 || argc == 4)
    {
        try
        {
            const string inputFileName(argv[1]);
            if (!isFileExists(inputFileName))
            {
                throw std::runtime_error("Input file not found.");
            }
            int blockSize = 1024 * 1024;
            if (argc == 4)
            {
                blockSize = std::stoi(argv[3]);
                if (blockSize <= 0)
                {
                    throw std::invalid_argument("Block size in negatively or equal 0.");
                }
            }
            const size_t nthreads = thread::hardware_concurrency();
            vector<vector<unsigned short> > result;
            result.resize(nthreads);
            vector<thread> workingThreads;

            const std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
            for (size_t threadIndex = 0; threadIndex < nthreads; ++threadIndex)
            {
                workingThreads.push_back(thread(&work, inputFileName, threadIndex, nthreads, blockSize, std::ref(result[threadIndex])));
            }

            for (size_t threadIndex = 0; threadIndex < nthreads; ++threadIndex)
            {
                workingThreads[threadIndex].join();
            }
            const std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

            cout << "Thread count: " << nthreads << ", analyse time: " << (end-start).count() << endl;
            dumpResults(argv[2], result, nthreads);
        }
        catch(const std::out_of_range& outEx)
        {
            cout << "Block size argument out of range error: " << outEx.what() << endl;
        }
        catch(const std::invalid_argument& invEx)
        {
            cout << "Input block size invalid argument error: " << invEx.what() << endl;
        }
        catch(const std::exception& ex)
        {
            cout << ex.what() << endl;
        }
    }
    else
    {
        cout << "Invalid parameters count" << endl;
    }
    return 0;
}
