#include <SFML/Graphics.hpp>
#include "ECSGame.h"

int main(int argc, char* argv[])
{
    //Start the game!
    ECSGame::Instance().Run("Simple Stellaris");
    return 0;
}