
// represents the 'skeleton' of all the pieces
abstract class Piece 
{
    Coords coords; // coordinates for the pieces representing the rows
    Side side; // the side i.e. white / black
    Piece_type piece; // the piece i.e. bishop

    // constructor
    public Piece (Coords coords, Side side, Piece_type piece)
    {
        this.coords = coords;
        this.side = side;
        this.piece = piece;
    }

    // fetches the coordinates of the piece
    public Coords get_coords ()
    {
        return this.coords;
    }

    // updates the coordinates of the piece
    public void set_coords (Coords coords)
    {
        this.coords = coords;
    }
    
    // fetches the side that the piece is on
    public Side get_side ()
    {
        return this.side;
    }

    // fetches the coords of the piece
    public void set_piece (Coords coords)
    {
        this.coords = coords;
    }

    // fetches the name of the piece
    public Piece_type get_piece ()
    {
        return this.piece;
    }

    // each piece has their own set of moves
    abstract void move ();
}
