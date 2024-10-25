#include "Tile.h"

/*
    Tile class
    All the member functions for implementing a tile/chamber in the maze solving program.
    Author: Daniel Page
    Date: Winter 2022
*/

// constructor
Tile::Tile()
{
    tileType = 0; // assume it is a free chamber
    mark = false; // by default, all tiles are not marked.
    neighbours = new Tile *[4];
    id = 0;
    for (int i = 0; i < 4; i++)
    {
        neighbours[i] = nullptr;
    }
}

// deconstructor
Tile::~Tile()
{
    delete neighbours;
    // There is nothing to de-allocate, each neighbour Tile object will be deleted eventually.
}

// get the tile type
int Tile::getTileType()
{
    return tileType;
}

// update the tile type
void Tile::setTileType(int newType)
{
    tileType = newType;
}

// get whether the tile is marked or not.
bool Tile::isMarked()
{
    return mark;
}

// set the tile as marked
void Tile::markTile()
{
    mark = true; // mark the tile
}

// get a neighbour.  If indexed out of range, it returns null.
Tile *Tile::getNeighbour(int index)
{
    if (index > 3 || index < 0)
    {
        return nullptr;
    }
    else
    {
        return (neighbours[index]);
    }
}

// Set a neighbour...
void Tile::setNeighbour(int index, Tile *newNeighbour)
{
    if (index >= 0 && index < 4)
    { // ensure the index is in range.
        neighbours[index] = newNeighbour;
    }
}

// get its ID
int Tile::getID()
{
    return id;
}

// set its ID
void Tile::setID(int idNew)
{
    id = idNew;
}