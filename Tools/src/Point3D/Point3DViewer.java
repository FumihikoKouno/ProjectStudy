package Point3D;

import java.lang.IndexOutOfBoundsException;

import java.util.ArrayList;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Toolkit;
import java.awt.Point;

import java.awt.event.MouseMotionListener;
import java.awt.event.MouseWheelListener;
import java.awt.event.MouseListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;

import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JMenuItem;

import Data.*;

public class Point3DViewer extends JPanel implements MouseListener, MouseMotionListener, MouseWheelListener{

	public int currentID = 0;

	public double SIZE_UNIT_BY_PERSPECTIVE;
	public Vec3D SIZE_ORIGIN;

	public static final double MIN_ZOOM = 0;
	public double zoom = 1;

	public int mouseX, mouseY;

	public boolean endDrawnConvert = true;

	public ArrayList<Vec3D[]> point = new ArrayList<Vec3D[]>();
	public ArrayList<Vec3D[]> rotatedPoint = new ArrayList<Vec3D[]>();
	public ArrayList<Vec3D> drawnPoint= new ArrayList<Vec3D>();
	public ArrayList<Point[]> line = new ArrayList<Point[]>();
	public ArrayList<Point> ids = new ArrayList<Point>();
	public ArrayList<Color> color = new ArrayList<Color>();

	public ArrayList<Quaternion> qu = new ArrayList<Quaternion>();

	public ArrayList<Vec3D> origin = new ArrayList<Vec3D>();
	public Vec3D sharedOrigin;
	public static final Vec3D first = new Vec3D(0,0,-1);
	public ArrayList<String> names = new ArrayList<String>();
	public ArrayList<Boolean> selected = new ArrayList<Boolean>();

	public Quaternion testQu = new Quaternion(1,0,0,0);
	
	private JPopupMenu popupMenu;
	private JPanel menuPanel;

	private DrawnData drawnData;
	
	private int WIDTH;
	private int HEIGHT;

	public void reset(){
		point.clear();
		rotatedPoint.clear();
		color.clear();
		line.clear();
		qu.clear();
		names.clear();
		selected.clear();
		zoom = 1;
		repaint();
	}

	public void setDefaultView(){
		zoom = Double.MAX_VALUE;
		for(int i = 0; i < point.size(); i++){
			double minX = Double.MAX_VALUE;
			double minY = Double.MAX_VALUE;
			double maxX = Double.MIN_VALUE;
			double maxY = Double.MIN_VALUE;
			Vec3D[] v = point.get(i);
			for(int j = 0; j < v.length; j++){
				if(minX > v[j].getX()) minX = v[j].getX();
				if(maxX < v[j].getX()) maxX = v[j].getX();
				if(minY > v[j].getY()) minY = v[j].getY();
				if(maxY < v[j].getY()) maxY = v[j].getY();
			}
			double tmpX = (WIDTH-50)/(maxX-minX);
			double tmpY = (HEIGHT-50)/(maxY-minY);
			double tmpZoom = (tmpX < tmpY ? tmpX : tmpY);
			if(zoom > tmpZoom) zoom = tmpZoom;
			origin.set(i,new Vec3D(((-minX-maxX)*zoom+WIDTH)/2.0,((minY+maxY)*zoom+HEIGHT)/2.0,0));
			qu.set(i,new Quaternion(1,0,0,0));
		}
		repaint();
	}

	public void updatePoints(int i, Vec3D[] v){
		point.set(i,v);
		autoRotate();
		repaint();
	}

	public void updatePoints(String s, Vec3D[] v){
		int idx = nameToIdx(s);
		if(idx == -1) return;
		updatePoints(idx,v);
	}

	public void updatePoints(int i, Vec3D[] v, Point[] l, Color c){
		point.set(i,v);
		line.set(i,l);
		color.set(i,c);
		autoRotate();
		repaint();
	}

	public int nameToIdx(String s){
		for(int i = 0; i < point.size(); i++){
			if(s.equals(names.get(i))){
				return i;
			}
		}
		return -1;
	}

	public void updatePoints(String s, Vec3D[] v, Point[]l, Color c){
		int idx = nameToIdx(s);
		if(idx == -1) return;
		point.set(idx,v);
		line.set(idx,l);
		color.set(idx,c);
		autoRotate();
		repaint();
	}

	public void addPoints(String s, Vec3D[] v, Point[] l, Color c, boolean b){
		double minX = Double.MAX_VALUE;
		double minY = Double.MAX_VALUE;
		double maxX = Double.MIN_VALUE;
		double maxY = Double.MIN_VALUE;
		for(int i = 0; i < v.length; i++){
			if(minX > v[i].getX()) minX = v[i].getX();
			if(maxX < v[i].getX()) maxX = v[i].getX();
			if(minY > v[i].getY()) minY = v[i].getY();
			if(maxY < v[i].getY()) maxY = v[i].getY();
		}
		double tmpX = (WIDTH-50)/(maxX-minX);
		double tmpY = (HEIGHT-50)/(maxY-minY);
		double tmpZoom = (tmpX < tmpY ? tmpX : tmpY);
		if(zoom > tmpZoom || point.size() == 0) zoom = tmpZoom;
		names.add(s);
		point.add(v);
		rotatedPoint.add(v.clone());
		color.add(c);
		line.add(l);
		selected.add(b);
		origin.add(new Vec3D(((-minX-maxX)*zoom+WIDTH)/2.0,((minY+maxY)*zoom+HEIGHT)/2.0-10,0));
		qu.add(new Quaternion(1,0,0,0));
		repaint();
	}

	public void setColor(int i, Color c){
		color.set(i,c);
		repaint();
	}

	public void rmPoints(String s){
		int i = nameToIdx(s);
		if(i == -1) return;
		names.remove(i);
		point.remove(i);
		rotatedPoint.remove(i);
		color.remove(i);
		line.remove(i);
		selected.remove(i);
		origin.remove(i);
		qu.remove(i);
		repaint();
	}

	public Point3DViewer(){
		this(480,480);
	}

	public Point3DViewer(int width, int height) {
		WIDTH = width;
		HEIGHT = height;

		addMouseListener(this);
		addMouseMotionListener(this);
		addMouseWheelListener(this);
		
		drawnData = new DrawnData(width,height);

		setPreferredSize(new Dimension(WIDTH, HEIGHT));
		sharedOrigin = new Vec3D(WIDTH/2,HEIGHT/2,0);
		SIZE_UNIT_BY_PERSPECTIVE = 50;
		SIZE_ORIGIN = new Vec3D(0,0,300);
	}

	public void reSize(int w, int h){
		if(w<0||h<0)return;
		WIDTH = w;
		HEIGHT = h;
		drawnData.resize(w,h);
		setPreferredSize(new Dimension(WIDTH, HEIGHT));
		revalidate();
		repaint();
	}

	public void setSizeOrigin(Vec3D v){ SIZE_ORIGIN = v; }
	public void setSizeUnit(double d){ SIZE_UNIT_BY_PERSPECTIVE = d; }

	public void paintComponent(Graphics g){
		switch(Option.perspective){
		case Option.ONE_POINT_PERSPECTIVE:
			if(Option.viewMethod==Option.Z_BUFFER){
				drawnData.reset();
				drawnData.setZoom(zoom);
				/*
				 * test
				Vec3D[] a = new Vec3D[8];
				drawnData.setOrigin(new Vec3D(WIDTH/2,HEIGHT/2,0));
				drawnData.setZoom(zoom);
				a[0] = new Vec3D(0,50,-50);
				a[1] = new Vec3D(0,-50,-50);
				a[2] = new Vec3D(50,-50,0);
				a[3] = new Vec3D(50,50,0);
				a[4] = new Vec3D(0,50,50);
				a[5] = new Vec3D(0,-50,50);
				a[6] = new Vec3D(-50,-50,0);
				a[7] = new Vec3D(-50,50,0);
				for(int i = 0; i < a.length; i++){
					a[i] = testQu.rotate(a[i].sub(Option.rot));
					a[i] = a[i].add(Option.rot);
				}
				drawnData.setCircle(a[0], 10, Color.RED);
				drawnData.setCircle(a[1], 10, Color.RED);
				drawnData.setCircle(a[2], 10, Color.BLUE);
				drawnData.setCircle(a[3], 10, Color.BLUE);
				drawnData.setCircle(a[4], 10, Color.YELLOW);
				drawnData.setCircle(a[5], 10, Color.YELLOW);
				drawnData.setCircle(a[6], 10, Color.GREEN);
				drawnData.setCircle(a[7], 10, Color.GREEN);
				drawnData.setRect(a[0],a[1],a[2],a[3],Color.RED);
				drawnData.setRect(a[2],a[3],a[4],a[5],Color.BLUE);
				drawnData.setRect(a[4],a[5],a[6],a[7],Color.YELLOW);
				drawnData.setRect(a[6],a[7],a[0],a[1],Color.GREEN);
				drawnData.setRect(a[0],a[1],a[5],a[4],Color.WHITE);
				drawnData.setRect(a[2],a[3],a[7],a[6],Color.LIGHT_GRAY);
				*/
			}
			setPointByOnePointPerspective();
			break;
		case Option.NO_PERSPECTIVE:
			setPoint();
			break;
		}
		g.setColor(Color.BLACK);
		g.fillRect(0,0,WIDTH,HEIGHT);
		if(Option.perspective == Option.ONE_POINT_PERSPECTIVE && Option.viewMethod==Option.Z_BUFFER)
			drawnData.draw(g,zoom);
		else
			drawPointAndLine(g);
		if(Option.viewR) drawPoint(g,Option.rot,Color.RED);
		if(Option.viewO) drawPoint(g,Option.onePP,Color.BLUE);
	}
	
	public synchronized void update() {
		repaint();
	}

	public void autoRotate(){
		for(int i = 0; i < point.size(); i++){
			rotatePoints(qu.get(i), point.get(i), i);
		}
	}

	private void setPointByOnePointPerspective(){
		drawnPoint.clear();
		ids.clear();
		try{
			for(int i = 0; i < rotatedPoint.size(); i++){
				Vec3D[] rp = rotatedPoint.get(i).clone();
				if(Option.viewMethod == Option.Z_BUFFER){
					drawnData.setOrigin(origin.get(i));
					for(int j = 0; j < rp.length; j++){
						Vec3D tmp = new Vec3D(rp[j].getX(),rp[j].getY(),0);
						rp[j] = tmp.add(Option.onePP.sub(tmp).times(rp[j].getZ()/Option.onePP.getZ()));	
					}
		
					for(int j = 0; j < rp.length; j++){
						int size = (int)((SIZE_ORIGIN.getZ()-rp[j].getZ())/SIZE_UNIT_BY_PERSPECTIVE);
						drawnData.setCircle(rp[j], size, Color.BLUE);//color.get(i));
						for(int k = 0; k < line.get(i).length; k++){
							if(j==line.get(i)[k].x){
								int size2 = (int)((SIZE_ORIGIN.getZ()-rp[line.get(i)[k].y].getZ())/SIZE_UNIT_BY_PERSPECTIVE);
								drawnData.setLine(rp[j], size<<1, rp[line.get(i)[k].y], size2<<1, color.get(i));
							}
						}
					}
				}else{
					for(int j = 0; j < rp.length; j++){
						//Vec3D tmp = rp[j].sub(Option.onePP);
						//tmp = tmp.times(1.0-(rp[j].getZ()/Option.onePP.getZ()));
						//tmp = tmp.add(Option.onePP);	
						Vec3D tmp = new Vec3D(rp[j].getX(),rp[j].getY(),0);
						tmp = tmp.add(Option.onePP.sub(tmp).times(rp[j].getZ()/Option.onePP.getZ()));
						boolean insert = false;
						for(int k = 0; k < drawnPoint.size(); k++){
							if(tmp.getZ() > drawnPoint.get(k).getZ()){
								drawnPoint.add(k,tmp);
								ids.add(k,new Point(i,j));
								insert = true;
								break;
							}
						}
						if(!insert){
							drawnPoint.add(tmp);
							ids.add(new Point(i,j));
						}
					}
				}
			}
		}catch(IndexOutOfBoundsException e){
			
		}
	}

	private void setPoint(){
		drawnPoint.clear();
		ids.clear();
		for(int i = 0; i < rotatedPoint.size(); i++){
			Vec3D[] rp = rotatedPoint.get(i);
			for(int j = 0; j < rp.length; j++){
				boolean insert = false;
				for(int k = 0; k < drawnPoint.size(); k++){
					if(rp[j].getZ() > drawnPoint.get(k).getZ()){
						drawnPoint.add(k,rp[j]);
						ids.add(k,new Point(i,j));
						insert = true;
						break;
					}
				}
				if(!insert){
					drawnPoint.add(rp[j]);
					ids.add(new Point(i,j));
				}
			}
		}
	}

	public void drawPoint(Graphics g, Vec3D p, Color c){
		g.setColor(c);
		int x,y;
		x = (int)(sharedOrigin.getX()+p.getX()*zoom);
		y = (int)(sharedOrigin.getY()-p.getY()*zoom);
		g.fillOval(x-2,y-2,8,8);
	}

	public void setMovable(String s, boolean b){
		int i = nameToIdx(s);
		if(i == -1) return;
		selected.set(i,b);
	}

	private void drawPointAndLine(Graphics g){
		try{
			for(int i = 0; i < drawnPoint.size(); i++){
				Point id = new Point(ids.get(i).x, ids.get(i).y);
				Vec3D dp = drawnPoint.get(i).clone();
				int x,y;
				x = (int)(origin.get(id.x).getX()+dp.getX()*zoom);
				y = (int)(origin.get(id.x).getY()-dp.getY()*zoom);
				int size = (int)((SIZE_ORIGIN.getZ()-dp.getZ())/SIZE_UNIT_BY_PERSPECTIVE);
				g.setColor(color.get(id.x));
				g.fillOval(x-size,y-size,size<<1,size<<1);
				for(int j = 0; j < line.get(id.x).length; j++){
					Vec3D tmpV;
					Point tmpL = line.get(id.x)[j];
					if(tmpL.x == id.y) tmpV = drawnPoint.get(ids.indexOf(new Point(id.x,tmpL.y))).clone();
					else continue;
					int x2,y2;
					x2 = (int)(origin.get(id.x).getX()+tmpV.getX()*zoom);
					y2 = (int)(origin.get(id.x).getY()-tmpV.getY()*zoom);
					int size2 = (int)((SIZE_ORIGIN.getZ()-tmpV.getZ())/SIZE_UNIT_BY_PERSPECTIVE);
					if(Option.viewMethod==Option.LINE_AND_POINT)
						g.drawLine(x,y,x2,y2);
					else{
						Vec3D verticle = new Vec3D(y-y2,x2-x,0);
						verticle = verticle.unit();
						verticle = verticle.times(size);
						int[] xPoint = new int[4];
						int[] yPoint = new int[4];
						xPoint[0] = (int)(x+verticle.getX());
						xPoint[1] = (int)(x-verticle.getX());
						yPoint[0] = (int)(y+verticle.getY());
						yPoint[1] = (int)(y-verticle.getY());
						verticle = verticle.unit();
						verticle = verticle.times(size2);
						xPoint[2] = (int)(x2-verticle.getX());
						xPoint[3] = (int)(x2+verticle.getX());
						yPoint[2] = (int)(y2-verticle.getY());
						yPoint[3] = (int)(y2+verticle.getY());
						g.fillPolygon(xPoint,yPoint,4);
					}
				}
			}
		}catch(IndexOutOfBoundsException e){
			System.out.println("IndexOut in drawPointAndLine");
		}catch(NullPointerException e){
			System.out.println("NullPointer in drawPointAndLine");
		}
	}

	private void rotatePoints(Quaternion q, Vec3D[] p, int id){
		Vec3D[] rp = rotatedPoint.get(id);
		if(rp == null || rp.length != p.length) rp = new Vec3D[p.length];
		for(int i = 0; i < p.length; i++){
			rp[i] = q.rotate(p[i].sub(Option.rot));
			rp[i] = rp[i].add(Option.rot);
		}
	}

	/**
	 * mouse methods
	 */
	public void mousePressed(MouseEvent e){
		mouseX = e.getX();
		mouseY = e.getY();
	}
	public void mouseDragged(MouseEvent e){
		int mx = e.getX();
		int my = e.getY();
		double xDiff = mx-mouseX;
		double yDiff = my-mouseY;
		if((e.getModifiers() & MouseEvent.BUTTON1_MASK) != 0){
			double abs = Math.max(WIDTH,HEIGHT);
			Vec3D from = new Vec3D(0,0,-1);
			Vec3D to = new Vec3D(xDiff,-yDiff,-Math.sqrt(abs*abs-(xDiff*xDiff+yDiff*yDiff)));
			Quaternion rot = new Quaternion(from,to);
			testQu = testQu.mul(rot);
			for(int i = 0; i < selected.size(); i++){
				if(selected.get(i)){
					qu.set(i,qu.get(i).mul(rot));
				}
			}
			autoRotate();
		}
		if((e.getModifiers() & MouseEvent.BUTTON3_MASK) != 0){
			for(int i = 0; i < selected.size(); i++){
				if(selected.get(i)){
					origin.set(i,origin.get(i).add(new Vec3D(xDiff,yDiff,0)));
				}
			}
		}
		mouseX = mx;
		mouseY = my;
		repaint();
	}

	public void mouseWheelMoved(MouseWheelEvent e){
		zoom -= e.getWheelRotation()/10.0;
		if(zoom < MIN_ZOOM) zoom = MIN_ZOOM;
		repaint();
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
