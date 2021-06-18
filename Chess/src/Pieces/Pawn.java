package Pieces;

import Game.*;
import java.util.*;

public class Pawn extends Piece 
{
    /* these get sliced in C++ */
    boolean first_move = true; // used to see if a pawn can move twice
    boolean double_jumped = false; // used to see if a pawn has moved twice at the start
    
    // constructor
    public Pawn (int x, int y, Side side)
    {
        super (x, y, side, Piece_type.PAWN);
    }
    
    // implementing the move
    public void move (int x, int y)
    {
        Piece[][] brd = board.get_board();
        
        if (this.y - y == -2 || this.y - y == 2)
        {
            double_jumped = true;
        }
        
        brd[this.x][this.y] = null;
        brd[x][y] = this;
        this.x = x;
        this.y = y;
        this.first_move = false;
        
        return;
    }
    
    private void move_on_board (Vector<Vector<Integer>> moves, Piece[][] brd)
    {
        if (this.side == Side.BLACK)
        {
            // if first move, can double move
            if (first_move && (brd[this.x][this.y-2] == null))
            {
                push_to_moves (moves, this.x, this.y-2);
            }
            
            if (0 <= this.y-1)
            {
                // if can move up
                if ((brd[this.x][this.y-1] == null))
                {
                    push_to_moves (moves, this.x, this.y-1);
                }
                
                // if can take diagonally
                if (this.x+1 < 8)
                {
                    if ((brd[this.x+1][this.y-1] != null))
                    {
                        if (brd[this.x+1][this.y-1].get_side() != this.side)
                        {
                            push_to_moves (moves, this.x+1, this.y-1);
                        }
                    }
                }
                
                // if can take diagonally
                if (0 <= this.x-1)
                {
                    if ((brd[this.x-1][this.y-1] != null))
                    {
                        if (brd[this.x-1][this.y-1].get_side() != this.side)
                        {
                            push_to_moves (moves, this.x-1, this.y-1);
                        }
                    }
                }
            }
        }
        else // this.side == white
        {
            // if first move, can double move
            if (first_move && (brd[this.x][this.y+2] == null))
            {
                push_to_moves (moves, this.x, this.y+2);
            }
            
            if (this.y+1 < 8)
            {
                // if can move up
                if ((brd[this.x][this.y+1] == null))
                {
                    push_to_moves (moves, this.x, this.y+1);
                }
                
                // if can take diagonally
                if (this.x+1 < 8)
                {
                    if ((brd[this.x+1][this.y+1] != null))
                    {
                        if (brd[this.x+1][this.y+1].get_side() != this.side)
                        {
                            push_to_moves (moves, this.x+1, this.y+1);
                        }
                    }
                }
                
                // if can take diagonally
                if (0 <= this.x-1)
                {
                    if ((brd[this.x-1][this.y+1] != null))
                    {
                        if (brd[this.x-1][this.y+1].get_side() != this.side)
                        {
                            push_to_moves (moves, this.x-1, this.y+1);
                        }
                    }
                }
            }
        }
    }
    
    // the set of all unique moves of the specific piece
    public Vector<Vector<Integer>> available_move ()
    {
        Vector<Vector<Integer>> moves = new Vector<Vector<Integer>>();
        Piece[][] brd = board.get_board();
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
