package Pieces;
import Game.*;
import java.util.*;

public class King extends Piece {
    // constructor
    public King (int x, int y, Side side)
    {
        super (x, y, side, Piece_type.KING);
    }
    
    // unique move of the specific piece
    public Vector<Vector<Integer>> move ()
    {
        Vector<Vector<Integer>> moves = new Vector<Vector<Integer>>();
        Vector<Integer> temp_vec;

        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                if ((i-1<= this.x && this.x <= i+1 &&
                    j-1<= this.y && this.y <= j+1) && 
                    !(this.x == i && this.y == j))
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
