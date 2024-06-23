#ifndef STOCKFISH_NATIVE_LIB_H
#define STOCKFISH_NATIVE_LIB_H

#define DLLExport __declspec(dllexport)

#include <chrono>
#include <thread>

extern "C"
{
    DLLExport void getmoves(char* pPath, size_t pathSize, char* pForsythEdwardsNotation, size_t forsythEdwardsNotationSize, char* pResult, size_t resultSize);
    DLLExport void getmove(char* pPath, size_t pathSize, int skillLevel, char* pForsythEdwardsNotation, size_t forsythEdwardsNotationSize, char* pResult, size_t resultSize);
}
#endif