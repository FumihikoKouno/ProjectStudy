package Score;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.GridLayout;
import java.awt.Image;
import java.awt.Toolkit;
import java.awt.Point;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseWheelListener;
import java.awt.event.MouseListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;

import javax.swing.JPanel;


import Data.*;

public class ScoreViewer extends JPanel implements MouseMotionListener, MouseListener, MouseWheelListener{
	private MotionDataConverter mdc;
	private double[] score;
	private double sum;

	private Image dbImage;
	private Graphics dbg;
	
	private final int pointSize = 5;

	private final int xOffset = 20;
	private final int yOffset = 120;
	
	private int dataSize;
	
	private int WIDTH;
	private int HEIGHT;
	// TODO : 将来はオプション化して指定できるように
	private int model = 0;
	private int user = 1;
	public ScoreViewer(MotionDataConverter converter, int width, int height){
		mdc = converter;
		score = new double[dataSize];
		WIDTH = width;
		HEIGHT = height;

		addMouseListener(this);
		addMouseMotionListener(this);
		addMouseWheelListener(this);
		
		setPreferredSize(new Dimension(WIDTH, HEIGHT));
	}
	
	public void readData(){
		dataSize = (mdc.data[model].size() < mdc.data[user].size() ? mdc.data[model].size() : mdc.data[user].size());
		score = new double[dataSize];
		sum = 0;
		for(int i = 0; i < dataSize; i++){
			score[i] = mdc.data[user].getFrameScore(mdc.data[model], i);
			sum += score[i];
		}
	}
	
	public void reSize(int w, int h){
		if(w<0||h<0)return;
		WIDTH = w;
		HEIGHT = h;
		setPreferredSize(new Dimension(WIDTH, HEIGHT));

		revalidate();
		repaint();
	}
	
	public void update(){
		readData();
		draw();
	}
	
	public Point convertToDraw(double x, double y){
		double maxScore = Double.MIN_VALUE;
		double minScore = Double.MAX_VALUE;
		for(int i = 0; i < dataSize; i++){
			maxScore = (maxScore > score[i] ? maxScore : score[i]);
			minScore = (minScore < score[i] ? minScore : score[i]);
		}
		double xUnit = 1.0*(WIDTH-(xOffset<<1))/(1.0*dataSize);
		double yUnit = 1.0*(HEIGHT-(yOffset<<1))/(maxScore);
		return new Point((int)((x*xUnit)+xOffset),(int)(HEIGHT-yOffset-(y*yUnit)));
	}
	
	public void draw(){
		if(dbImage == null){
			dbImage = createImage(WIDTH,HEIGHT);
			if(dbImage == null){
				System.out.println("dbImage is null");
				return;
			}else{
				dbg = dbImage.getGraphics();
			}
		}
		dbg.setColor(Color.WHITE);
		dbg.fillRect(0,0,WIDTH,HEIGHT);
		
		dbg.setColor(Color.BLACK);
		dbg.drawLine(xOffset,0,xOffset,HEIGHT);
		dbg.drawLine(0,HEIGHT-yOffset,WIDTH,HEIGHT-yOffset);

		for(int i = 1; i <= 10; i++){
			double y = (1.0*i)/10.0;
			Point p = convertToDraw(0,y);
			dbg.drawLine(p.x-5, p.y, p.x+5, p.y);
		}
		for(int i = 0; i < 10; i++){
			double x = (dataSize/10.0)*i;
			Point p = convertToDraw(x,0);
			dbg.drawLine(p.x, p.y-5, p.x, p.y+5);
		}
		
		dbg.setColor(Color.BLUE);
		for(int i = 0; i < dataSize-1; i++){
			Point p1 = convertToDraw(i,score[i]);
			Point p2 = convertToDraw(i+1,score[i+1]);
			dbg.fillOval(p1.x-(pointSize>>1), p1.y-(pointSize>>1), pointSize, pointSize);
			dbg.fillOval(p2.x-(pointSize>>1), p2.y-(pointSize>>1), pointSize, pointSize);
			dbg.drawLine(p1.x, p1.y, p2.x, p2.y);
		}
		
		try{
			Graphics g = getGraphics();
			if((g != null) && (dbImage != null)){
				g.drawImage(dbImage,0,0,null);
			}
			Toolkit.getDefaultToolkit().sync();
			if(g != null){
				g.dispose();
			}
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	
	public void mousePressed(MouseEvent e){
	}
	public void mouseDragged(MouseEvent e){
	}
	public void mouseWheelMoved(MouseWheelEvent e){
	}
	public void mouseEntered(MouseEvent e){
	}
	public void mouseExited(MouseEvent e){
	}
	public void mouseReleased(MouseEvent e){
	}
	public void mouseMoved(MouseEvent e){
	}
	public void mouseClicked(MouseEvent e){
	}
}
