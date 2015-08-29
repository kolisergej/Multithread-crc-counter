#ifndef COMMON
#define COMMON

#include <string>
#include <fstream>
#include <vector>

using std::ofstream;
using std::ifstream;
using std::string;
using std::vector;

unsigned short qChecksum(const char* pData, unsigned int length);
bool isFileExists(const string& name);
void dumpResults(const char* pOutputFileName, const vector<vector<unsigned short> >& result, size_t threadCount);

#endif // COMMON

