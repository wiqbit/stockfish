#include "stockfish.h"

#include "bitboard.h"
#include "misc.h"
#include "position.h"
#include "types.h"
#include "uci.h"
#include "tune.h"

using namespace Stockfish;

void getmoves(char* pPath, size_t pathSize, char* pForsythEdwardsNotation, size_t forsythEdwardsNotationSize, char* pResult, size_t resultSize)
{
    std::stringstream newIn;
    std::streambuf* oldIn = std::cin.rdbuf(newIn.rdbuf());

    std::stringstream newOut;
    std::streambuf* oldOut = std::cout.rdbuf(newOut.rdbuf());

    Bitboards::init();
    Position::init();

    std::string path = std::string(pPath, pathSize);
    char* args[] = { ((char*)path.c_str()) };
    char** argv = args;
    UCIEngine uci(1, argv);

    Tune::init(uci.engine_options());

    std::string forsythEdwardsNotation = std::string(pForsythEdwardsNotation, forsythEdwardsNotationSize);

    // 8 bytes for position
    // 1 byte for space
    // 3 bytes for fen keyword
    // 1 byte for space
    // up to 9 + 9 + 9 + 9 + 9 + 9 + 9 + 8 + 1 + 1 bytes (73) for fen value
    // 1 byte for null terminator
    size_t size1 = 8 + 1 + 3 + 1 + forsythEdwardsNotationSize + 1;
    char* pCommand1 = new char[size1];
    memset(pCommand1, 0, size1);
    sprintf_s(pCommand1, size1, "position fen %s", forsythEdwardsNotation.c_str());
    std::string command1 = std::string(pCommand1);
    uci.sendcommand(command1);
    
    std::string command2 = "go perft 1";
    uci.sendcommand(command2);

    std::string result = "";

    while (true)
    {
        std::string line;

        std::getline(newOut, line);

        if (line.length() == 0)
        {
            continue;
        }

        if (line.rfind("info ", 0) == 0)
        {
            continue;
        }

        if (line.rfind("Nodes searched: ", 0) == 0)
        {
            break;
        }

        if (result.length() > 0)
        {
            result += ";";
        }

        result += line.substr(0, 4);
    }

    std::string command3 = "quit";
    uci.sendcommand(command3);
    
    delete[] pCommand1;

    strcpy_s(pResult, resultSize, result.c_str());
}

void getmove(char* pPath, size_t pathSize, int skillLevel, char* pForsythEdwardsNotation, size_t forsythEdwardsNotationSize, char* pResult, size_t resultSize)
{
    std::stringstream newIn;
    std::streambuf* oldIn = std::cin.rdbuf(newIn.rdbuf());

    std::stringstream newOut;
    std::streambuf* oldOut = std::cout.rdbuf(newOut.rdbuf());

    Bitboards::init();
    Position::init();

    std::string path = std::string(pPath, pathSize);
    char* args[] = { ((char*)path.c_str()) };
    char** argv = args;
    UCIEngine uci(1, argv);

    Tune::init(uci.engine_options());

    // 9 bytes for setoption
    // 1 byte for space
    // 4 bytes for name
    // 1 byte for space
    // 11 bytes for Skill Level
    // 1 byte for space
    // 1 or 2 bytes for value
    // 1 byte for null terminator
    size_t size1 = 9 + 1 + 4 + 1 + 11 + 1 + (skillLevel < 10 ? 1 : 2) + 1;
    char* pCommand1 = new char[size1];
    memset(pCommand1, 0, size1);
    sprintf_s(pCommand1, size1, "setoption name Skill Level %d", skillLevel);
    std::string command1 = std::string(pCommand1);
    uci.sendcommand(command1);

    std::string forsythEdwardsNotation = std::string(pForsythEdwardsNotation, forsythEdwardsNotationSize);

    // 8 bytes for position
    // 1 byte for space
    // 3 bytes for fen keyword
    // 1 byte for space
    // up to forsythEdwardsNotationSize bytes for fen value
    // 1 byte for null terminator
    size_t size2 = 8 + 1 + 3 + 1 + forsythEdwardsNotationSize + 1;
    char* pCommand2 = new char[size2];
    memset(pCommand2, 0, size2);
    sprintf_s(pCommand2, size2, "position fen %s", forsythEdwardsNotation.c_str());
    std::string command2 = std::string(pCommand2);
    uci.sendcommand(command2);

    std::string command3 = "go depth 1";
    uci.sendcommand(command3);

    uci.wait_for_search_finished();

    std::string result = "";

    while (true)
    {
        std::string line;

        std::getline(newOut, line);

        // bestmove a7a6
        // bestmove a2a1q
        if (line.rfind("bestmove ", 0) == 0)
        {
            if (result.length() > 13
                && (result.substr(13, 1) == "q" || result.substr(13, 1) == "r" || result.substr(13, 1) == "n" || result.substr(13, 1) == "b"))
            {
                result += line.substr(9, 5);
            }
            else
            {
                result += line.substr(9, 4);
            }

            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::string command4 = "quit";
    uci.sendcommand(command4);

    delete[] pCommand1;
    delete[] pCommand2;

    strcpy_s(pResult, resultSize, result.c_str());
}