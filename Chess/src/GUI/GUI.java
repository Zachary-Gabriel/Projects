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
    
    // animation variables
    int clicked_x, clicked_y;
    int prev_cl_x, prev_cl_y;
    
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
        draw_board ();
    }
    
    // saves which tile was clicked
    public void mousePressed ()
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
                }
            }
        }
        if (prev_cl_x != -1 && prev_cl_y != -1 )
        move_piece ();
    }
    
    void move_piece ()
    {
        if (board.get_piece (prev_cl_x, prev_cl_y) != null)
        {
            if (board.get_piece (prev_cl_x, prev_cl_y).get_side () == turn)
            {
                Vector<Vector<Integer>> moves = board.get_piece (prev_cl_x, prev_cl_y).available_move();
                board.get_piece (prev_cl_x, prev_cl_y).checkmate_check(moves);
                System.out.println("");

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
    
    void draw_board ()
    {
        // rows (files)
        for (int i = 0; i < 8; ++i)
        {
            // columns (ranks)
            for (int j = 0; j < 8; ++j)
            {
                // default tiles
                if ((i+j) % 2 == 1)
                fill (30, 20, 10); // dark tiles
                else
                fill (101, 67, 33); // light tiles
                
                // highlights the clicked square
                if (i == clicked_x && j == clicked_y)
                {
                    fill (189, 144, 60); // light yellow
                }
                
                // if hovering
                if (i* (width /8) < mouseX && mouseX < (i+1)* (width /8) && 
                j* (height /8) < mouseY && mouseY < (j+1)* (height /8)) 
                {
                    fill (242, 214, 0); // yellow
                }
                
                // draws the rectangle
                rect (i* (width /8), j* (height /8), (i+1)* (width /8), (j+1)* (height /8));
                
                // if there's an image on the square, draw it
                draw_image (i, j);
                
                // highlight the available moves
                if (!(clicked_x == -1 || clicked_y == -1))
                {
                    if (board.get_piece (clicked_x, clicked_y) != null)
                    {
                        if (board.get_piece (clicked_x, clicked_y).get_side () == turn)
                        {
                            Vector<Vector<Integer>> moves = board.get_piece (clicked_x, clicked_y).available_move();
                            board.get_piece (clicked_x, clicked_y).checkmate_check(moves);
                            System.out.println("");

                            for (int k = 0; k < moves.size(); ++k)
                            {
                                if (moves.elementAt(k).elementAt(0).intValue() == i && 
                                moves.elementAt(k).elementAt(1).intValue() == j)
                                {
                                    fill (125); // grey
                                    noStroke();
                                    ellipse ((2*i+1)* (width /16), (2*j+1)* (height /16), (width /32), (height /32));
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
    
    void draw_image (int i, int j)
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
                image (images [index], i* (width/8), j* (height/8), (width/8) , (height/8));
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
