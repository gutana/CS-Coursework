#ifndef _TILE
#define _TILE

/*
    Tile class -Represents a chamber/tile.
    Author: Daniel Page
    Date: Winter 2022
*/
class Tile
{
private:
    /*
        TILE TYPES:
        0 - Free chamber/tile
        1 - Wall/solid
        2 - start tile/chamber
        3 - end tile/chamber
        4 - In the path...
    */
    int tileType;      // tile type
    bool mark;         // is the tile marked?
    Tile **neighbours; // Neighbouring tiles, nullptr when there is no neighbour.
    int id;            // an id for the tile.
public:
    Tile();  // constructor
    ~Tile(); // deconstructor

    // Tile type member functions...
    int getTileType();             // get the type of tile
    void setTileType(int newType); // assign the type of the tile

    // Marking member functions...
    bool isMarked(); // check if a tile is marked
    void markTile(); // set a tile as marked.

    // getting and setting neighbouring tiles
    Tile *getNeighbour(int index); // get the neighbour at an index (0 <= index < 3)
    void setNeighbour(int index, Tile *newNeighbour);

    // setting or getting its identifier
    int getID();
    void setID(int idNew);
};

#endif