
// The 'graphical user interface' so the user caan play the game
public class GUI_Board 
{
    // Constructor - Draws the chess board using characters to represent the pieces. 'N' stands for knight as per chess notation
    GUI_Board (Board board)
    {
        // columns (ranks)
        for (int j = 0; j < 8; ++j)
        {
            // rows (files)
            for (int i = 0; i < 8; ++i)
            {
                // blank space representing no piece
                char res = ' ';

                //checking if a piece exists on said square
                if (board.get_board()[j][i] != null) 
                {
                    // converting the piece into a letter
                    switch (board.get_board()[j][i].piece)
                    {
                        case BISHOP:
                            res = 'B';
                            break;
                        case KING:
                            res = 'K';
                            break;
                        case KNIGHT:
                            res = 'N';
                            break;
                        case PAWN:
                            res = 'P';
                            break;
                        case QUEEN:
                            res = 'Q';
                            break;
                        case ROOK:
                            res = 'R';
                    }
                }
                System.out.print (res);
            }
            System.out.println();
        }
    }
}
