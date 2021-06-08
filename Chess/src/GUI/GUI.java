package gui_package;

import processing.core.PApplet;

// The 'graphical user interface' so the user caan play the game
public class GUI extends PApplet
{
    // Driver for processing
    public static void main(String[] args) 
    {
        PApplet.main("gui_package.GUI");
    }
    
    // initial settings
    public void settings() 
    {
        size(800, 600);
    }
    
    // setup to the draw function
    public void setup() 
    {
        
    }
    
    // Executes this code once per frame
    public void draw() 
    {
        background(0);
    }
    
    // draws the board in ther terminal
    // void terminal_board (Board board)
    // {
    //     // columns (ranks)
    //     for (int j = 0; j < 8; ++j)
    //     {
    //         // rows (files)
    //         for (int i = 0; i < 8; ++i)
    //         {
    //             // blank space representing no piece
    //             char res = ' ';
                
    //             //checking if a piece exists on said square
    //             if (board.get_board()[j][i] != null) 
    //             {
    //                 // converting the piece into a letter
    //                 switch (board.get_board()[j][i].piece)
    //                 {
    //                     case BISHOP:
    //                     res = 'B';
    //                     break;
    //                     case KING:
    //                     res = 'K';
    //                     break;
    //                     case KNIGHT:
    //                     res = 'N';
    //                     break;
    //                     case PAWN:
    //                     res = 'P';
    //                     break;
    //                     case QUEEN:
    //                     res = 'Q';
    //                     break;
    //                     case ROOK:
    //                     res = 'R';
    //                 }
    //             }
    //             System.out.print (res);
    //         }
    //         System.out.println();
    //     }
    // }
}
