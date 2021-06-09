package Pieces;
import Game.*;
import java.util.*;

public class Queen extends Piece 
{
    // constructor
    public Queen (int x, int y, Side side)
    {
        super (x, y, side, Piece_type.QUEEN);
    }
    
    // unique move of the specific piece
    public Vector<Vector<Integer>> move ()
    {
        Vector<Vector<Integer>> moves = new Vector<Vector<Integer>>();
        
        Bishop b = new Bishop(this.x, this.y, side);
        Rook r = new Rook(this.x, this.y, side);

        moves = b.move();
        for (int i = 0; i < r.move().size(); ++i)
        {
            moves.addElement(r.move().get(i));
        }

        return moves;
    }
}
