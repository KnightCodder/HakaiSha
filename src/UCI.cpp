#include "UCI.h"

void UCI::UCI_Loop()
{
    bool setoption = false;
    bool position = false;
    bool go = false;
    std::string name = "";

    std::string line;
    while (std::getline(std::cin, line))
    {
        Commands comms(line);

        for (Command comm : comms.commands)
        {
            if (comm.key == "uci")
            {
                std::cout << "id name " << id::name << "\n";
                std::cout << "id author " << id::author << "\n";
                std::cout << "uciok\n";
            }
            else if (comm.key == "isready")
            {
                std::cout << "readyok\n";
            }
            else if (comm.key == "setoption")
            {
                setoption = true;
            }
            else if (comm.key == "name")
            {
                name = comm.value[0];
            }
            else if (comm.key == "UCI_AnalyseMode")
            {
                AnalyseMode = true;
            }
            else if (comm.key == "value")
            {
                // Handle the "value" command here
            }
            else if (comm.key == "ucinewgame")
            {
                engine.reset();
            }
            else if (comm.key == "quit")
            {
                quit();
            }
            else if (comm.key == "position")
            {
                position = true;
            }
            else if (comm.key == "startpos")
            {
                engine.board = ChessBoard();
            }
            else if (comm.key == "fen")
            {
                std::string fen = "";
                for (std::string s : comm.value)
                    fen += s + " ";
                engine.board = ChessBoard(fen);
            }
            else if (comm.key == "moves")
            {
                for (std::string m : comm.value)
                    engine.board.makeMoveFromUCI(m);
            }
            else if (comm.key == "go")
            {
                go = true;
            }
            else if (comm.key == "searchmoves" && go)
            {
                engine.parameters.movestosearch = comm.value;
            }
            else if (comm.key == "ponder" && go)
            {
                engine.ponder = true;
            }
            else if (comm.key == "wtime" && go)
            {
                engine.parameters.wtime = stoi(comm.value[0]);
            }
            else if (comm.key == "btime" && go)
            {
                engine.parameters.btime = stoi(comm.value[0]);
            }
            else if (comm.key == "winc" && go)
            {
                engine.parameters.winc = stoi(comm.value[0]);
            }
            else if (comm.key == "binc" && go)
            {
                engine.parameters.binc = stoi(comm.value[0]);
            }
            else if (comm.key == "movestogo" && go)
            {
                engine.parameters.movestogo = stoi(comm.value[0]);
            }
            else if (comm.key == "depth" && go)
            {
                engine.parameters.depth = stoi(comm.value[0]);
            }
            else if (comm.key == "nodes" && go)
            {
                engine.parameters.nodes = stoi(comm.value[0]);
            }
            else if (comm.key == "mate" && go)
            {
                engine.parameters.mate = stoi(comm.value[0]);
            }
            else if (comm.key == "movetime" && go)
            {
                engine.parameters.movetime = stoi(comm.value[0]);
            }
            else if (comm.key == "infinite" && go)
            {
                engine.parameters.infinite = true;
            }
            else if (comm.key == "stop")
            {
                engine.stop();
            }
            else if (comm.key == "ponderhit")
            {
                engine.ponder = false;
            }
            else if (comm.key == "debug")
            {
                engine.debug = (comm.value[0] == "on") ? true : false;
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
        if (go)
        {
            std::string bestmove = engine.getBestMove();
            std::string ponder = engine.getPonderMove();

            std::cout << "bestmove " << bestmove << " ponder " << ponder << "\n";

            engine.ponder = false; // make all parameters reset
        }
    }
}