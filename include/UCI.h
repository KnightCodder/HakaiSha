#ifndef UCI_H
#define UCI_H

#include "engine.h"
#include <set>

std::set<std::string> Keys = {
    // general UCI commands
    "uci",
    "isready",
    "setoption",
    "name",
    "UCI_AnalyseMode",
    "value",
    "ucinewgame",
    "quit",

    // position setup commands
    "position",
    "startpos",
    "fen",
    "moves",

    // search commands
    "go",
    "searchmoves",
    "ponder",
    "wtime",
    "btime",
    "winc",
    "binc",
    "movestogo",
    "depth",
    "nodes",
    "mate",
    "movetime",
    "infinite",
    "stop",
    "ponderhit",

    // optional commands
    "debug",
    "register",
    "name",
    "code",
    "savegame"};

bool isKey(const std::string &str) { return Keys.find(str) != Keys.end(); }

std::set<std::string> InfoKeys = {
    // information commands
    "info",
    "depth",
    "seldepth",
    "time",
    "nodes",
    "pv",
    "score",
    "cp",
    "mate",
    "currmove",
    "currmovenumber",
    "hashfull",
    "nps",
    "string",
    "refutation",
    "bestmove",
    "ponder",
};

struct Command
{
    std::string key;
    std::vector<std::string> value;

    Command(std::string k = "", std::vector<std::string> v = {})
    {
        key = k;
        value = v;
    }
};

struct Commands
{
    std::vector<Command> commands;

    Commands(std::string line)
    {
        std::istringstream iss(line);
        std::string token;

        while (iss >> token)
        {
            if (isKey(token))
            {
                commands.push_back(Command(token, {}));
            }
            else if (!commands.empty())
            {
                commands.back().value.push_back(token);
            }
            else
            {
                std::cerr << "Warning: Ignoring unrecognized token: " << token << "\n";
            }
        }
    }
};

class UCI
{
public:
    ChessEngine engine;

    void UCI_Loop();
    
};

#endif