package Pieces;

import Game.*;
import java.util.*;

public class King extends Piece 
{    
    // constructor
    public King (int x, int y, Side side)
    {
        super (x, y, side, Piece_type.KING);
    }
    
    void update_first_move (boolean toggle)
    {
        this.first_move = toggle;
        return;
    }
    
    // implementing the move
    public void move (int x, int y)
    {
        Piece[][] brd = board.get_board();
        
        if (this.y == y)
        {
            if (this.x + 2 == x)
            {
                Rook r = (Rook) brd[7][y];
                r.move (5, this.y);
            }

            if (this.x - 2 == x)
            {
                Rook r = (Rook) brd[0][y];
                r.move (3, this.y);
            }
        }

        brd[this.x][this.y] = null;
        brd[x][y] = this;
        this.x = x;
        this.y = y;
        first_move = false;

        return;
    }
    
    private Vector<Vector<Integer>> all_enemy_moves ()
    {
        Vector<Vector<Integer>> ret = new Vector<Vector<Integer>>();
        Piece[][] brd = board.get_board();

        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                if (brd[i][j] != null)
                {
                    if (brd[i][j].get_side() != this.side)
                    {
                        if (brd[i][j].get_piece() != Piece_type.KING)
                        {
                            ret.addAll (brd[i][j].available_move());
                        }
                        else
                        {
                            King k = (King) brd[i][j];
                            k.king_simple_moves(ret, brd);
                        }
                    }
                }
            }
        }
        return ret;
    }

    public void king_simple_moves (Vector<Vector<Integer>> moves, Piece[][] brd)
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

    void move_on_board (Vector<Vector<Integer>> moves, Piece[][] brd)
    {
        king_simple_moves (moves, brd);

        // castles
        if (this.first_move)
        {
            // O-O-O
            int x = this.x - 1;
            boolean space_to_castle = true;
            Vector<Vector<Integer>> castle_moves = new Vector<Vector<Integer>>();
            push_to_moves (castle_moves, this.x, this.y);
            Vector<Vector<Integer>> all_enemy_moves = this.all_enemy_moves();

            // checks if space is clear
            while (1 <= x)
            {
                if (brd[x][this.y] != null)
                {
                    space_to_castle = false;
                    break;
                }
                if (this.x-3 < x && x < this.x)
                {
                    push_to_moves (castle_moves, x, this.y);
                }
                --x;
            }

            // checks if king doesnt move through check
            if (space_to_castle)
            {
                if (brd[0][this.y] != null)
                {
                    if (brd[0][this.y].get_piece () == Piece_type.ROOK && brd[0][this.y].get_side () == this.side)
                    {
                        boolean toggle = true;
                        Rook r = (Rook) brd[0][this.y];
                        if (r.first_move)
                        {
                            for (int i = 0; i < all_enemy_moves.size(); ++i)
                            {
                                for (int j = 0; j < castle_moves.size(); ++j)
                                {
                                    if (all_enemy_moves.get(i).get(0) == castle_moves.get(j).get(0) && all_enemy_moves.get(i).get(1) == castle_moves.get(j).get(1))
                                    {
                                        toggle = false;
                                    }
                                }
                            }
                            if (toggle)
                                push_to_moves (moves, this.x-2, this.y);
                        }
                    }
                }
            }
            
            // O-O
            x = this.x + 1;
            space_to_castle = true;
            castle_moves = new Vector<Vector<Integer>>();
            push_to_moves (castle_moves, this.x, this.y);

            // checks if space is clear
            while (x < 7)
            {
                if (brd[x][this.y] != null)
                {
                    space_to_castle = false;
                    break;
                }
                if (this.x < x && x < this.x + 3)
                {
                    push_to_moves (castle_moves, x, this.y);
                }
                ++x;
            }

            // checks if king doesnt move through check
            if (space_to_castle)
            {
                if (brd[7][this.y] != null)
                {
                    if (brd[7][this.y].get_piece () == Piece_type.ROOK && brd[7][this.y].get_side () == this.side)
                    {
                        Rook r = (Rook) brd[7][this.y];
                        if (r.first_move)
                        {
                            push_to_moves (moves, this.x+2, this.y);
                        }
                    }
                }
            }
        }

        return ;
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
