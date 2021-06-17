package Game;

import Pieces.*;

// represents the board - this may be where most of the chess comes together
public class Board 
{
    Piece[][] board; // represents the chess board with a 2d pieces array

    // Constructor
    public Board ()
    {
        board = new Piece[8][8];
        return;
    }

    // fetches the board
    public Piece[][] get_board ()
    {
        return board;
    }

    // updates the board
    public void set_board (Piece[][] board)
    {
        this.board = board;
        return;
    }

    // gets the piece at the corresponding coordinates
    public Piece get_piece (int x, int y)
    {
        return board[x][y];
    }
    
    // evaulates whether or not the 
    // public boolean can_move (int startx, int starty, int endx, int endy)
    // {
    //     Piece
    //     return true;
    // }

    // Turns the board into the default starting position
    public void initial_position ()
    {
        // Pawns
        for (int i = 0; i < 8; ++i)
        {
            board[i][1] = new Pawn (i, 1, Side.WHITE);
            board[i][6] = new Pawn (i, 6, Side.BLACK);
        }

        // White Rooks
        board[0][0] = new Rook (0, 0, Side.WHITE);
        board[7][0] = new Rook (7, 0, Side.WHITE);

        // Black Rooks
        board[0][7] = new Rook (0, 7, Side.BLACK);
        board[7][7] = new Rook (7, 7, Side.BLACK);

        // White Knights
        board[1][0] = new Knight (1, 0, Side.WHITE);
        board[6][0] = new Knight (6, 0, Side.WHITE);

        // Black Knights
        board[1][7] = new Knight (1, 7, Side.BLACK);
        board[6][7] = new Knight (6, 7, Side.BLACK);

        // White Bishops
        board[2][0] = new Bishop (2, 0, Side.WHITE);
        board[5][0] = new Bishop (5, 0, Side.WHITE);

        // Black Bishops
        board[2][7] = new Bishop (2, 7, Side.BLACK);
        board[5][7] = new Bishop (5, 7, Side.BLACK);

        // Queens
        board[3][0] = new Queen (3, 0, Side.WHITE);
        board[3][7] = new Queen (3, 7, Side.BLACK);

        // Kings
        board[4][0] = new King (4, 0, Side.WHITE);
        board[4][7] = new King (4, 7, Side.BLACK);

        return;
    }

}
