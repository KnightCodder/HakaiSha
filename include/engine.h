#include "structure.h"
#include <random>
#include <algorithm> // For std::shuffle
#include <chrono>

struct Parameters
{
    std::vector<std::string> movestosearch = {"all"};
    bool infinite = false;
    int wtime, btime, winc, binc, movestogo, depth, nodes, mate, movetime;

    void reset()
    {
        movestosearch = {"all"};
        infinite = false;
        wtime = btime = winc = binc = movestogo = depth = nodes = mate = movetime = NULL;
    }
};

struct InfoParameters
{
    std::string currmove;

    int currmovenumber, depth, nodes, nps, score_cp, score_mate, time, seldepth, lowerbound, upperbound, hashfull, tbhits, sbhits, cpuload, cpunr;

    std::vector<std::pair<std::vector<std::string>, int>> multipv; // line, score
    std::vector<std::string> currline;
    std::vector<std::string> refutation;

    void reset()
    {
        currmove = NAN;
        currmovenumber = depth = nodes = nps = score_cp = score_mate = time = seldepth = lowerbound = upperbound = hashfull = tbhits = sbhits = cpuload = cpunr = NULL;
        currline = refutation = {};
        multipv = {};
    }
};

class ChessEngine
{
public:
    ChessBoard board;
    std::atomic<bool> stopSearch; // Atomic flag to stop search
    std::thread searchThread;
    bool ponder = false;
    bool debug = false;

    bool UCI_ShowRefutation = false;
    bool UCI_ShowCurrLine = false;
    bool UCI_AnalyseMode = false;

    Parameters parameters;
    InfoParameters infoparameters;

    ChessEngine() : stopSearch(false) {}

    void reset()
    {
        board = ChessBoard();
        parameters.reset();
        infoparameters.reset();
    }

    std::string getBestMove()
    {
    }

    std::string getPonderMove()
    {
    }

    Score negamax(int depth, Score alpha, Score beta);

    Score quiescenceCaptures(Score alpha, Score beta, int depth);

    Score quiescenceChecks(Score alpha, Score beta, int depth);

    Move findBestMove(int depth)
    {
        std::vector<Move> moves = board.generateLegalMoves();

        // Random number generator
        std::random_device rd;
        std::mt19937 gen(rd());

        // Shuffle the vector
        std::shuffle(moves.begin(), moves.end(), gen);

        Move bestMove = moves[0];

        Score alpha = {0, -INF};
        Score beta = {0, INF};
        Score bestValue = {0, -INF};

        for (const Move &move : moves)
        {
            if (stopSearch.load())
                break;

            board.makeMove(move);

            Score moveValue = -negamax(depth - 1, -beta, -alpha);
            board.unmakeMove();

            if (moveValue > bestValue)
            {
                bestValue = moveValue;
                bestMove = move;
            }

            alpha = std::max(alpha, moveValue); // Update alpha for pruning

            if (alpha >= beta)
            // if (stopSearch.load())
            {
                // Beta cut-off or stop search if requested
                break;
            }
        }

        return bestMove;
    }

    void startSearch(int depth)
    {
        stop();
        stopSearch.store(false);

        // Calculate allocated time
        int allocatedTime = ((board.colorToMove == Color::WHITE) ? parameters.wtime : parameters.btime) / 30;
        auto startTime = std::chrono::steady_clock::now();

        searchThread = std::thread(
            [this, depth, allocatedTime, startTime]()
            {
                Move bestMove;
                infoparameters.depth = 1;

                while (infoparameters.depth <= depth && !stopSearch.load())
                {
                    bestMove = findBestMove(infoparameters.depth);

                    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::steady_clock::now() - startTime);

                    if (elapsed.count() >= allocatedTime)
                        break;

                    infoparameters.depth++;
                }

                if (!stopSearch.load())
                {
                   std::cout << "bestmove " << bestMove.toString() << "\n";
                }
            });
    }

    void stop()
    {
        stopSearch.store(true);
        if (searchThread.joinable())
        {
            searchThread.join(); // Ensure the search thread completes
        }
    }
};