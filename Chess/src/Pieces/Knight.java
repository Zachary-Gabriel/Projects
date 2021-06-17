package Pieces;
import Game.*;
import java.util.*;

public class Knight extends Piece 
{
    // constructor
    public Knight (int x, int y, Side side)
    {
        super (x, y, side, Piece_type.KNIGHT);
    }
    
    // implementing the move
    public void move (int x, int y)
    {
        Piece[][] brd = board.get_board();
        
        brd[this.x][this.y] = null;
        brd[x][y] = this;
        this.x = x;
        this.y = y;

        return;
    }

    // the set of all unique moves of the specific piece
    public Vector<Vector<Integer>> available_move ()
    {
        Vector<Vector<Integer>> moves = new Vector<Vector<Integer>>();
        Piece[][] brd = board.get_board ();

        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                if ((this.x-2 == i || this.x+2 == i) && (this.y -1 == j || this.y +1 == j) ||
                    (this.x-1 == i || this.x+1 == i) && (this.y -2 == j || this.y +2 == j))
                {
                    if (brd[i][j] != null)
                    {
                        if (brd[i][j].get_side() != this.side)
                        {
                            push_to_moves(moves, i, j);
                        }
                    }
                    else
                    {
                        push_to_moves(moves, i, j);
                    }
                }
            }
        }

        return moves;
    }
}
