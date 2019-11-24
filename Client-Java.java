package com.company;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.*;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Base64;

import org.json.simple.JSONObject;

import javax.imageio.ImageIO;
import javax.imageio.stream.ImageInputStream;
import javax.swing.*;
import javax.swing.border.EmptyBorder;

public class Client {


    public static void main(String[] args) {
        try {
            InputStream imageIn;
            Socket client = new Socket("IP", PORT);

            PrintWriter out = new PrintWriter(client.getOutputStream(),true);

            ImageInputStream inputStream = ImageIO.createImageInputStream(client.getInputStream());

            
            //Static size of incoming image
            byte [] imageSize = new byte[64000];

            
            //Initial a message to server.
            out.println("Send stream");



            //Create panel for displaying images
            JPanel contentPanel = new JPanel();
            contentPanel.setBorder(new EmptyBorder(0, 0, 0, 0));
            contentPanel.setLayout(null);

            JFrame frame = new JFrame();
            frame.setLayout(new FlowLayout());
            frame.setSize(800,600);
            JLabel lbl = new JLabel();
            frame.add(lbl);
            frame.setVisible(true);
            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);


            
            while (true) {
    
                //Read data coming from server
                inputStream.read(imageSize);

                //Convert the data into an image and display it onto the panel.
                imageIn = new ByteArrayInputStream(imageSize);
                BufferedImage image = ImageIO.read(imageIn);
                if(image !=null) {
                    ImageIcon icon = new ImageIcon(image);

                    lbl.setIcon(icon);

                    image.flush();


                }



                frame.revalidate();
                frame.repaint();



            }


        } catch (UnknownHostException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
