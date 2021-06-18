package Pieces;

import Game.*;
import java.util.*;

public class Pawn extends Piece 
{
    /* these get sliced in C++ */
    boolean first_move = true; // used to see if a pawn can move twice
    public boolean can_enpassant = false; // used to see if a pawn has moved twice at the start
    
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
            can_enpassant = true;
        }
        
        // en passant
        if ((this.side == Side.BLACK) && (this.y == 3) ||(this.side == Side.WHITE) && (this.y == 4))
        {
            if (this.x+1 < 8)
            {
                if ((brd[this.x+1][this.y] != null))
                {
                    if (brd[this.x+1][this.y].get_side() != this.side && brd[this.x+1][this.y].get_piece () == Piece_type.PAWN)
                    {
                        Pawn pawn = (Pawn) brd[this.x+1][this.y];
                        if (pawn.can_enpassant)
                        {
                            if (x == this.x+1)
                            {
                                if ((this.side == Side.BLACK && y == this.y-1) || (this.side == Side.WHITE && y == this.y+1))
                                {
                                    brd[this.x+1][this.y] = null;
                                }
                            }
                        }
                    }
                } 
            }
            if (0 <= this.x-1)
            {
                if ((brd[this.x-1][this.y] != null))
                {
                    if (brd[this.x-1][this.y].get_side() != this.side && brd[this.x-1][this.y].get_piece () == Piece_type.PAWN)
                    {
                        Pawn pawn = (Pawn) brd[this.x-1][this.y];
                        if (pawn.can_enpassant)
                        {
                            if (x == this.x-1)
                            {
                                if ((this.side == Side.BLACK && y == this.y-1) || (this.side == Side.WHITE && y == this.y+1))
                                {
                                    brd[this.x-1][this.y] = null;
                                }
                            }
                        }
                    }
                } 
            }
        }

        brd[x][y] = this;
        brd[this.x][this.y] = null;
        this.x = x;
        this.y = y;
        this.first_move = false;
        return;
    }
    
    public void update_enpassant (boolean toggle)
    {
        this.first_move = toggle;
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
                
                if (this.x+1 < 8)
                {
                    // if can take diagonally
                    if ((brd[this.x+1][this.y-1] != null))
                    {
                        if (brd[this.x+1][this.y-1].get_side() != this.side)
                        {
                            push_to_moves (moves, this.x+1, this.y-1);
                        }
                    }
                    // en passant
                    if (this.y == 3)
                    {
                        if ((brd[this.x+1][this.y] != null))
                        {
                            if (brd[this.x+1][this.y].get_side() != this.side && brd[this.x+1][this.y].get_piece () == Piece_type.PAWN)
                            {
                                Pawn pawn = (Pawn) brd[this.x+1][this.y];
                                if (pawn.can_enpassant)
                                push_to_moves (moves, this.x+1, this.y-1);
                            }
                        } 
                    }
                }
                
                if (0 <= this.x-1)
                {
                    // if can take diagonally
                    if ((brd[this.x-1][this.y-1] != null))
                    {
                        if (brd[this.x-1][this.y-1].get_side() != this.side)
                        {
                            push_to_moves (moves, this.x-1, this.y-1);
                        }
                    }
                    // en passant
                    if (this.y == 3)
                    {
                        if ((brd[this.x-1][this.y] != null))
                        {
                            if (brd[this.x-1][this.y].get_side() != this.side && brd[this.x-1][this.y].get_piece () == Piece_type.PAWN)
                            {
                                Pawn pawn = (Pawn) brd[this.x-1][this.y];
                                if (pawn.can_enpassant)
                                push_to_moves (moves, this.x-1, this.y-1);
                            }
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
                
                if (this.x+1 < 8)
                {
                    // if can take diagonally
                    if ((brd[this.x+1][this.y+1] != null))
                    {
                        if (brd[this.x+1][this.y+1].get_side() != this.side)
                        {
                            push_to_moves (moves, this.x+1, this.y+1);
                        }
                    }
                    
                    // en passant
                    if (this.y == 4)
                    {
                        if ((brd[this.x+1][this.y] != null))
                        {
                            if (brd[this.x+1][this.y].get_side() != this.side && brd[this.x+1][this.y].get_piece () == Piece_type.PAWN)
                            {
                                Pawn pawn = (Pawn) brd[this.x+1][this.y];
                                if (pawn.can_enpassant)
                                push_to_moves (moves, this.x+1, this.y+1);
                            }
                        } 
                    }
                }
                
                if (0 <= this.x-1)
                {
                    // if can take diagonally
                    if ((brd[this.x-1][this.y+1] != null))
                    {
                        if (brd[this.x-1][this.y+1].get_side() != this.side)
                        {
                            push_to_moves (moves, this.x-1, this.y+1);
                        }
                    }
                    
                    // en passant
                    if (this.y == 4)
                    {
                        if ((brd[this.x-1][this.y] != null))
                        {
                            if (brd[this.x-1][this.y].get_side() != this.side && brd[this.x-1][this.y].get_piece () == Piece_type.PAWN)
                            {
                                Pawn pawn = (Pawn) brd[this.x-1][this.y];
                                if (pawn.can_enpassant)
                                push_to_moves (moves, this.x-1, this.y+1);
                            }
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
