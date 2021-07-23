package GUI;

import Game.*;
import Pieces.*;

import java.util.*;
import processing.core.PApplet;
import processing.core.PImage;

// The 'graphical user interface' so the user caan play the game
public class GUI extends PApplet
{
    // private variables
    Board board;
    PImage[] images = new PImage[13]; // saves the images for faster performance
    Side turn;
    Side pov;
    
    // animation variables
    int clicked_x, clicked_y;
    int prev_cl_x, prev_cl_y;
    
    // scene variables
    boolean game_over = false;
    boolean title_screen = true;
    
    // the main function that runs the settings(), then setup() then draw() functions.
    public static void main (String[] args) { PApplet.main(new String[]{"GUI.GUI"}); }
    
    // initial settings
    public void settings () 
    {
        size(800, 600);
    }
    
    // setup to the draw function
    public void setup () 
    {
        // preloading images whilst runniong other instructions
        preload_images ();
        
        // makes the new board
        board = new Board ();
        board.initial_position ();
        update_pieces_to_board ();
        turn = Side.WHITE;
        
        // shows the board in the terminal
        Terminal_GUI tgui = new Terminal_GUI (board);
        tgui.terminal_board ();
        
        // resetting the drawing variables
        clicked_x = -1;
        clicked_y = -1;
        prev_cl_x = -1;
        prev_cl_y = -1;
    }
    
    // Executes this code once per frame
    public void draw () 
    {
        if (title_screen)
        {
            draw_title_screen ();
        }
        else
        {
            draw_board ();
            if (game_over)
            {
                draw_game_over ();
            }
            return ;
        }
    }
    
    // Displays game over message
    void draw_game_over ()
    {
        textSize (50);
        fill (255, 0, 0); // text colour
        
        if (turn == Side.WHITE)
        {
            text ("Black Wins!", width/2 - 150, height/2);
        }
        else
        {
            text ("White Wins!", width/2 - 150, height/2);
        }
    }
    
    // Displays title screen
    void draw_title_screen ()
    {
        // resets scene
        for (int i = 0; i < height; ++ i)
        {
            stroke (255 - i*255/height, 0, i*255/height);
            line(0, i, width, i);
        }
        
        // Title
        textSize (50);
        fill (0); // text colour
        text ("Chess!", width/2 - 75, height/4);
        
        // Boxes
        stroke (0);
        fill (200);
        rect (width/4, 6*height/16, width/2, height/4);
        rect (width/4, 11*height/16, width/2, height/4);
        
        // top box hover
        if (width/4 < mouseX && mouseX < 3*width/4 && 6*height/16 < mouseY && mouseY < 10*height/16)
        {
            fill (125);
            // top box click
            if (mousePressed)
            {
                fill (75);
                pov = Side.BLACK;
                title_screen = false;
            }
            rect (width/4, 6*height/16, width/2, height/4);
        }
        
        // bottom box hover
        else if (width/4 < mouseX && mouseX < 3*width/4 && 11*height/16 < mouseY && mouseY < 15*height/16)
        {
            fill (125);
            // bottom box click
            if (mousePressed)
            {
                fill (75);
                pov = Side.WHITE;
                title_screen = false;
            }
            rect (width/4, 11*height/16, width/2, height/4);
        }
        
        // Box filling
        fill (0, 0, 0); // text colour
        text ("Play as Black", width/4 + 35, height/2 + 15);
        text ("Play as White", width/4 + 35, 7*height/8 - 15);
        
        return ;
    }
    
    void dealing_pov (int i, int j, int x, int y)
    {
        // default tiles
        if ((i+j) % 2 == 1)
        fill (30, 20, 10); // dark tiles
        else
        fill (101, 67, 33); // light tiles
        
        // if hovering
        if (x* (width /8) < mouseX && mouseX < (x+1)* (width /8) && 
        y* (height /8) < mouseY && mouseY < (y+1)* (height /8)) 
        {
            fill (242, 214, 0); // yellow
        }
        
        // draws the rectangle
        rect (x* (width /8), y* (height /8), (width /8), (height /8));

        // if there's an image on the square, draw it
        draw_image (i, j, x, y);

        // highlight the available moves
        if (!(clicked_x == -1 || clicked_y == -1))
        {
            if (board.get_piece (clicked_x, clicked_y) != null)
            {
                if (board.get_piece (clicked_x, clicked_y).get_side () == turn)
                {
                    Vector<Vector<Integer>> moves = board.get_piece (clicked_x, clicked_y).available_move();
                    board.get_piece (clicked_x, clicked_y).checkmate_check(moves);
                    
                    for (int k = 0; k < moves.size(); ++k)
                    {
                        if (moves.elementAt(k).elementAt(0).intValue() == i && 
                        moves.elementAt(k).elementAt(1).intValue() == j)
                        {
                            fill (125); // grey
                            noStroke();
                            ellipse ((2*x+1)* (width /16), (2*y+1)* (height /16), (width /32), (height /32));
                            stroke(0);
                        }
                    }
                }
                else
                {
                    // resetting variables
                    prev_cl_x = -1;
                    prev_cl_y = -1;
                    clicked_x = -1;
                    clicked_y = -1;
                }
            }
        }
    }
    
    // Draws the Board
    void draw_board ()
    {
        // rows (files)
        for (int i = 0; i < 8; ++i)
        {
            // columns (ranks)
            for (int j = 0; j < 8; ++j)
            {
                if (pov == Side.BLACK)
                {
                    dealing_pov (i, j, i, j);
                }
                if (pov == Side.WHITE)
                {
                    dealing_pov (i, j, 7-i, 7-j);
                }
            }
        }
    }
    
    // saves which tile was clicked
    public void mousePressed ()
    {
        if (!title_screen)
        {
            for (int i = 0; i < 8; ++i)
            {
                for (int j = 0; j < 8; ++j)
                {
                    if (i* (width /8) < mouseX && mouseX < (i+1)* (width /8) && 
                    j* (height /8) < mouseY && mouseY < (j+1)* (height /8))
                    {
                        prev_cl_x = clicked_x;
                        prev_cl_y = clicked_y;
                        clicked_x = i;
                        clicked_y = j;
                        if (pov == Side.WHITE)
                        {
                                clicked_y = 7 - clicked_y;
                                clicked_x = 7 - clicked_x;
                            }
                        }
                    }
                }
                if (prev_cl_x != -1 && prev_cl_y != -1 )
                move_piece ();
            }
        }
        
        // moves piece if valid move
        void move_piece ()
        {
            if (board.get_piece (prev_cl_x, prev_cl_y) != null)
            {
                if (board.get_piece (prev_cl_x, prev_cl_y).get_side () == turn)
                {
                    Vector<Vector<Integer>> moves = board.get_piece (prev_cl_x, prev_cl_y).available_move();
                    game_over = board.get_piece (prev_cl_x, prev_cl_y).checkmate_check(moves);
                    
                    // for terminal
                    if (game_over) System.out.println ("Game Over");
                    
                    for (int i = 0; i < moves.size(); ++i)
                    {
                        Vector<Integer> coords = moves.get(i);
                        if (coords.get(0) == clicked_x && coords.get(1) == clicked_y)
                        {
                            board.get_board ()[prev_cl_x][prev_cl_y].move (clicked_x, clicked_y);
                            
                            // prints to terminal
                            Terminal_GUI tgui = new Terminal_GUI (board);
                            tgui.terminal_board ();
                            
                            // swapping turn
                            if (turn == Side.WHITE)
                            {
                                turn = Side.BLACK;
                            }
                            else
                            {
                                turn = Side.WHITE;
                            }
                            
                            // each completed turn disables enpassant
                            reset_enpassant ();
                        }   
                    }
                }
            }
            
            // resetting variables
            prev_cl_x = -1;
            prev_cl_y = -1;
            clicked_x = -1;
            clicked_y = -1;
            
            return ;
        }
        
        // resets the chess piece variables for enpassant
        void reset_enpassant ()
        {
            for (int i = 0; i < 8; ++i)
            {
                for (int j = 0; j < 8; ++j)
                {
                    if (board.get_piece (i, j) != null)
                    {
                        if (board.get_piece (i, j).get_piece () == Piece_type.PAWN)
                        {
                            Pawn pawn = (Pawn) board.get_piece (i, j);
                            if (board.get_piece (i, j).get_side() == turn)
                            {
                                pawn.can_enpassant = false;
                            }
                        }
                    }
                }
            }
        }
        
        void update_pieces_to_board ()
        {
            for (int i = 0; i < 8; ++i)
            {
                for (int j = 0; j < 8; ++j)
                {
                    if (board.get_piece(i, j) != null)
                    {
                        board.get_piece(i, j).update_board (board);
                    }
                }
            }
            return ;
        }
        
        void draw_image (int i, int j, int x, int y)
        {
            // the image name. 'null' represents nothing
            String img_name = "null";
            
            // checking if a piece exists on said square
            if (board.get_board()[i][j] != null) 
            {
                // converting the piece into a letter
                switch (board.get_board()[i][j].get_piece())
                {
                    case BISHOP:
                    img_name = "Bishop";
                    if (board.get_board()[i][j].get_side() == Side.BLACK)
                    img_name += "_b";
                    else
                    img_name += "_w";
                    break;
                    
                    case KING:
                    img_name = "King";
                    if (board.get_board()[i][j].get_side() == Side.BLACK)
                    img_name += "_b";
                    else
                    img_name += "_w";
                    break;
                    
                    case KNIGHT:
                    img_name = "Knight";
                    if (board.get_board()[i][j].get_side() == Side.BLACK)
                    img_name += "_b";
                    else
                    img_name += "_w";
                    break;
                    
                    case PAWN:
                    img_name = "Pawn";
                    if (board.get_board()[i][j].get_side() == Side.BLACK)
                    img_name += "_b";
                    else
                    img_name += "_w";
                    break;
                    
                    case QUEEN:
                    img_name = "Queen";
                    if (board.get_board()[i][j].get_side() == Side.BLACK)
                    img_name += "_b";
                    else
                    img_name += "_w";
                    break;
                    
                    case ROOK:
                    img_name = "Rook";
                    if (board.get_board()[i][j].get_side() == Side.BLACK)
                    img_name += "_b";
                    else
                    img_name += "_w";
                }
                
                int index = name_to_index (img_name);
                if (index != -1)
                {
                    image (images [index], x* (width/8), y* (height/8), (width/8) , (height/8));
                }
            }
            return ;
        }
        
        // Returns the index of the 'images array' where the image name is stored.
        int name_to_index (String img_name)
        {
            switch (img_name)
            {
                case "null": 
                return 0;
                case "Bishop_b":
                return 1;
                case "Bishop_w": 
                return 2;
                case "King_w": 
                return 3;
                case "King_b": 
                return 4;
                case "Knight_w": 
                return 5;
                case "Knight_b": 
                return 6;
                case "Pawn_w": 
                return 7;
                case "Pawn_b": 
                return 8;
                case "Queen_w": 
                return 9;
                case "Queen_b": 
                return 10;
                case "Rook_w": 
                return 11;
                case "Rook_b": 
                return 12;
            }
            return -1;
        }
        
        // Stores the images in an array so that theyre only loaded once.
        void preload_images ()
        {
            String img_name = "null.png";
            PImage img = loadImage (img_name);
            images[0] = img;
            
            img_name = "Bishop_b.png";
            img = loadImage (img_name);
            images[1] = img;
            
            img_name = "Bishop_w.png";
            img = loadImage (img_name);
            images[2] = img;
            
            img_name = "King_w.png";
            img = loadImage (img_name);
            images[3] = img;
            
            img_name = "King_b.png";
            img = loadImage (img_name);
            images[4] = img;
            
            img_name = "Knight_w.png";
            img = loadImage (img_name);
            images[5] = img;
            
            img_name = "Knight_b.png";
            img = loadImage (img_name);
            images[6] = img;
            
            img_name = "Pawn_w.png";
            img = loadImage (img_name);
            images[7] = img;
            
            img_name = "Pawn_b.png";
            img = loadImage (img_name);
            images[8] = img;
            
            img_name = "Queen_w.png";
            img = loadImage (img_name);
            images[9] = img;
            
            img_name = "Queen_b.png";
            img = loadImage (img_name);
            images[10] = img;
            
            img_name = "Rook_w.png";
            img = loadImage (img_name);
            images[11] = img;
            
            img_name = "Rook_b.png";
            img = loadImage (img_name);
            images[12] = img;
            
            return ;
        }
    }
    