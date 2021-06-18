package Pieces;

import Game.*;
import java.util.*;

public class King extends Piece {
    // constructor
    public King (int x, int y, Side side)
    {
        super (x, y, side, Piece_type.KING);
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
    
    void move_on_board (Vector<Vector<Integer>> moves, Piece[][] brd)
    {
        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                if ((i-1<= this.x && this.x <= i+1 &&
                j-1<= this.y && this.y <= j+1) && 
                !(this.x == i && this.y == j))
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
    }
    
    // the set of all unique moves of the specific piece
    public Vector<Vector<Integer>> available_move ()
    {
        Vector<Vector<Integer>> moves = new Vector<Vector<Integer>>();
        Piece[][] brd = board.get_board ();
        move_on_board (moves, brd);
        
        return moves;
    }

    // the set of all unique moves of the specific piece using an input board
    public Vector<Vector<Integer>> available_move (Piece[][] brd)
    {
        Vector<Vector<Integer>> moves = new Vector<Vector<Integer>>();
        move_on_board (moves, brd);
        
        return moves;
    }
}
