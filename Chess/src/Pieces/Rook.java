package Pieces;
import Game.*;
import java.util.*;

public class Rook extends Piece 
{    
    // constructor
    public Rook (int x, int y, Side side)
    {
        super (x, y, side, Piece_type.ROOK);
    }
    
    // unique move of the specific piece
    public Vector<Vector<Integer>> move ()
    {
        Vector<Vector<Integer>> moves = new Vector<Vector<Integer>>();
        Vector<Integer> temp_vec = new Vector<Integer>();
        
        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                if ((this.x == i || this.y == j) && !(this.x == i && this.y == j))
                {
                    temp_vec = new Vector<Integer>();
                    temp_vec.add (i);
                    temp_vec.add (j);
                    moves.add (temp_vec);
                }
            }
        }
        
        return moves;
    }
}
