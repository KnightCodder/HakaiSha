#include "UCI.h"

void initializeZobrist();
void compilingPieceVision();

int main()
{
    initializeZobrist();
    compilingPieceVision();

    UCI uci;
    uci.UCI_Loop();
    
    return 0;
}