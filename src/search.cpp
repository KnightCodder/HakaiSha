#include "engine.h"

Score ChessEngine::quiescenceCaptures(Score alpha, Score beta, int depth)
{
    Score bestValue = {board.evaluateBoard() * colorValues[board.colorToMove], 0}; // anticipating that its not mate

    if (depth <= 0 || stopSearch.load())
        return bestValue;

    alpha = std::max(alpha, bestValue);

    if (alpha >= beta)
        return bestValue;

    std::vector<Move> captureMoves = board.generateCapturePromotionMoves();

    for (const Move &move : captureMoves)
    {
        board.makeMove(move);
        Score eval = -quiescenceCaptures(-beta, -alpha, depth - 1);
        board.unmakeMove();

        bestValue = std::max(eval, bestValue);
        alpha = std::max(alpha, bestValue);

        if (alpha >= beta)
        {
            break;
        }
    }

    return bestValue;
}

Score ChessEngine::quiescenceChecks(Score alpha, Score beta, int depth)
{
    Score bestValue = {board.evaluateBoard() * colorValues[board.colorToMove], 0}; // anticipating that its not mate

    if (depth <= 0 || stopSearch.load())
        return bestValue;

    alpha = std::max(alpha, bestValue);

    if (alpha >= beta)
        return bestValue;

    std::vector<Move> checkMoves = board.generateCheckMoves();

    for (const Move &move : checkMoves)
    {
        board.makeMove(move);
        Score eval = -quiescenceChecks(-beta, -alpha, depth - 1);
        board.unmakeMove();

        bestValue = std::max(eval, bestValue);
        alpha = std::max(alpha, bestValue);

        if (alpha >= beta)
        {
            break;
        }
    }

    return bestValue;
}

Score ChessEngine::negamax(int depth, Score alpha, Score beta)
{
    std::vector<Move> moves = board.generateLegalMoves();

    if (moves.empty())
    {
        if (board.checkAt(board.kingPosition[board.colorToMove], board.colorToMove))
        {
            // Checkmate condition: return a large negative value if in check
            return {0, depth - parameters.depth};
        }
        else
        {
            // Stalemate condition: return 0
            return 0;
        }
    }
    
    if (depth == 0 || stopSearch.load())
    {
        // return std::max(quiescenceChecks(alpha, beta, 1), quiescenceCaptures(alpha, beta, 1));
        return quiescenceCaptures(alpha, beta, 1);
        // return board.evaluateBoard() * colorValues[board.colorToMove];
    }

    Score maxEval = {0,-INF};

    for (const Move &move : moves)
    {
        if (stopSearch.load())
            break;
        board.makeMove(move);
        Score eval = -negamax(depth - 1, -beta, -alpha);
        board.unmakeMove();

        maxEval = std::max(maxEval, eval);
        alpha = std::max(alpha, eval);
        if (alpha >= beta)
        {
            break;
        }
    }

    return maxEval;
}
