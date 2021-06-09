package Pieces;
import Game.*;

import java.util.*;

// represents the 'skeleton' of all the pieces
public abstract class Piece 
{
    int x, y; // coordinates for the pieces representing the rows
    Side side; // the side i.e. white / black
    Piece_type piece; // the piece i.e. bishop

    // constructor
    public Piece (int x, int y, Side side, Piece_type piece)
    {
        this.x = x;
        this.y = y;
        this.side = side;
        this.piece = piece;
    }

    // updates the coordinates of the piece
    public void set_x (int x)
    {
        this.x = x;
    }

    // returns the coordinates of the piece
    public int get_x ()
    {
        return this.x;
    }

    // updates the coordinates of the piece
    public void set_y (int y)
    {
        this.y = y;
    }

    // returns the coordinates of the piece
    public int get_y ()
    {
        return this.y;
    }
    
    // fetches the side that the piece is on
    public Side get_side ()
    {
        return this.side;
    }

    // fetches the coords of the piece
    public void set_piece (Piece_type piece)
    {
        this.piece = piece;
    }

    // fetches the name of the piece
    public Piece_type get_piece ()
    {
        return this.piece;
    }

    // each piece has their own set of moves
    public abstract Vector<Vector<Integer>> move ();
}
