package Pieces;
import Game.*;

import java.util.*;

public class Bishop extends Piece 
{
    // constructor
    public Bishop (int x, int y, Side side)
    {
        super (x, y, side, Piece_type.BISHOP);
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
    
    // finds the moves for one diagonal in one direction
    private void one_diagonal (Vector<Vector<Integer>> moves, Piece[][] brd,  boolean incr_x, boolean incr_y)
    {
        int i = this.x;
        int j = this.y;
        
        while (0 <= i && 0 <= j && i < 8 && j < 8) // bounds of the board
        {
            if (((this.x - i == this.y - j) || (i - this.x == this.y - j)) && !(this.x == i && this.y == j)) // diagonal moves
            {
                if (brd[i][j] == null) // nothing in the tile
                {
                    push_to_moves (moves, i, j);
                }
                else if (brd[i][j].side != this.side) // taking an enemy piece
                {
                    push_to_moves (moves, i, j);
                    break;
                }
                else // cant push through the allied piece
                {
                    break;
                }
            }
            
            if (incr_x)
            i++;
            else
            i--;
            
            if (incr_y)
            j++;
            else
            j--;
        }
    }

    // the set of all unique moves of the specific piece
    public Vector<Vector<Integer>> available_move ()
    {
        Vector<Vector<Integer>> moves = new Vector<Vector<Integer>>(); 
        Piece[][] brd = board.get_board();

        one_diagonal (moves, brd, true, true);
        one_diagonal (moves, brd, false, true);
        one_diagonal (moves, brd, true, false);
        one_diagonal (moves, brd, false, false);

        return moves;
    }

    public Vector<Vector<Integer>> available_move (Piece[][] brd)
    {
        Vector<Vector<Integer>> moves = new Vector<Vector<Integer>>();

        one_diagonal (moves, brd, true, true);
        one_diagonal (moves, brd, false, true);
        one_diagonal (moves, brd, true, false);
        one_diagonal (moves, brd, false, false);

        return moves;
    }
}
