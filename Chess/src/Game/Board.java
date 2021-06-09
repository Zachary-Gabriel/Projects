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
    
    // Turns the board into the default starting position
    public void initial_position ()
    {
        // Pawns
        for (int i = 0; i < 8; ++i)
        {
            board[1][i] = new Pawn (1, i, Side.WHITE);
            board[6][i] = new Pawn (6, i, Side.BLACK);
        }

        // White Rooks
        board[0][0] = new Rook (0, 0, Side.WHITE);
        board[0][7] = new Rook (0, 7, Side.WHITE);

        // Black Rooks
        board[7][0] = new Rook (7, 0, Side.BLACK);
        board[7][7] = new Rook (7, 7, Side.BLACK);

        // White Knights
        board[0][1] = new Knight (0, 1, Side.WHITE);
        board[0][6] = new Knight (0, 6, Side.WHITE);

        // Black Knights
        board[7][1] = new Knight (7, 1, Side.BLACK);
        board[7][6] = new Knight (7, 6, Side.BLACK);

        // White Bishops
        board[0][2] = new Bishop (0, 2, Side.WHITE);
        board[0][5] = new Bishop (0, 5, Side.WHITE);

        // Black Bishops
        board[7][2] = new Bishop (7, 2, Side.BLACK);
        board[7][5] = new Bishop (7, 5, Side.BLACK);

        // Queens
        board[0][3] = new Queen (0, 3, Side.WHITE);
        board[7][3] = new Queen (7, 3, Side.BLACK);

        // Kings
        board[0][4] = new King (0, 4, Side.WHITE);
        board[7][4] = new King (7, 4, Side.BLACK);

        return;
    }

}
