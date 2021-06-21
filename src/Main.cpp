#include <iostream>
#include "./Constants.h"
#include "./Game.h"
using namespace std;

int main(int argc, char *args[])
{
    //Pointer to new Game class memory address
    //Pointers are used to avoid making copies of something
    //One Game instance only
    Game *game = new Game(); //"new" object created in heap

    game->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT);

    while (game->IsRunning())
    {
        game->ProcessInput();
        game->Update();
        game->Render();
    }

    game->Destroy();

    return 0;
}

//stack is a limited memory size - things declared "in scope" go here
//heaps store things long term
//allocated memory needs to be freed so that other programs can use it

//objects in stack are automatically deleted when they're no longer in scope
//objects in heap need to be deleted using "delete" keyword

//technically the "limit" of the size of the heap is equal to the amount of RAM you have

//an example of all this is closing tabs in a web browser, you expect those tabs to no longer be in memory and so they have to be deallocated