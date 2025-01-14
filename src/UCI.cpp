#include "UCI.h"

void UCI::UCI_Loop()
{
    std::string line;
    while (std::getline(std::cin, line))
    {
        Commands comms(line);

        for (Command comm : comms.commands)
        {
            if (comm.key == "uci")
            {
                // Handle the "uci" command here
            }
            else if (comm.key == "isready")
            {
                // Handle the "isready" command here
            }
            else if (comm.key == "setoption")
            {
                // Handle the "setoption" command here
            }
            else if (comm.key == "name")
            {
                // Handle the "name" command here
            }
            else if (comm.key == "UCI_AnalyseMode")
            {
                // Handle the "UCI_AnalyseMode" command here
            }
            else if (comm.key == "value")
            {
                // Handle the "value" command here
            }
            else if (comm.key == "ucinewgame")
            {
                // Handle the "ucinewgame" command here
            }
            else if (comm.key == "quit")
            {
                // Handle the "quit" command here
            }
            else if (comm.key == "position")
            {
                // Handle the "position" command here
            }
            else if (comm.key == "startpos")
            {
                // Handle the "startpos" command here
            }
            else if (comm.key == "fen")
            {
                // Handle the "fen" command here
            }
            else if (comm.key == "moves")
            {
                // Handle the "moves" command here
            }
            else if (comm.key == "go")
            {
                // Handle the "go" command here
            }
            else if (comm.key == "searchmoves")
            {
                // Handle the "searchmoves" command here
            }
            else if (comm.key == "ponder")
            {
                // Handle the "ponder" command here
            }
            else if (comm.key == "wtime")
            {
                // Handle the "wtime" command here
            }
            else if (comm.key == "btime")
            {
                // Handle the "btime" command here
            }
            else if (comm.key == "winc")
            {
                // Handle the "winc" command here
            }
            else if (comm.key == "binc")
            {
                // Handle the "binc" command here
            }
            else if (comm.key == "movestogo")
            {
                // Handle the "movestogo" command here
            }
            else if (comm.key == "depth")
            {
                // Handle the "depth" command here
            }
            else if (comm.key == "nodes")
            {
                // Handle the "nodes" command here
            }
            else if (comm.key == "mate")
            {
                // Handle the "mate" command here
            }
            else if (comm.key == "movetime")
            {
                // Handle the "movetime" command here
            }
            else if (comm.key == "infinite")
            {
                // Handle the "infinite" command here
            }
            else if (comm.key == "stop")
            {
                // Handle the "stop" command here
            }
            else if (comm.key == "ponderhit")
            {
                // Handle the "ponderhit" command here
            }
            else if (comm.key == "debug")
            {
                // Handle the "debug" command here
            }
            else if (comm.key == "register")
            {
                // Handle the "register" command here
            }
            else if (comm.key == "code")
            {
                // Handle the "code" command here
            }
            else if (comm.key == "savegame")
            {
                // Handle the "savegame" command here
            }
            else
            {
                std::cerr << "Unrecognized command: " << comm.key << "\n";
            }
        }
    }
}