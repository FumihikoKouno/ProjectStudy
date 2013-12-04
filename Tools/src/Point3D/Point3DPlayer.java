package Point3D;

import java.util.ArrayList;

import java.awt.Point;
import java.awt.Color;
import java.awt.Dimension;
/*
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Toolkit;
*/
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JButton;
import javax.swing.JScrollBar;
import javax.swing.JOptionPane;
import javax.swing.JFileChooser;

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.FlowLayout;

import java.awt.event.ActionListener;
import java.awt.event.AdjustmentListener;
import java.awt.event.ActionEvent;
import java.awt.event.AdjustmentEvent;

import java.io.IOException;
import java.io.File;

import Data.*;

public class Point3DPlayer extends JPanel implements ActionListener, AdjustmentListener{

	public int WIDTH;
	public int HEIGHT;

	public static final int BUTTON_HEIGHT = 52;

	public Point3DViewer pv;

	public JPanel scrollPanel = new JPanel();
	public ArrayList<JLabel> labelList = new ArrayList<JLabel>();
	public ArrayList<JScrollBar> sb = new ArrayList<JScrollBar>();
	public ArrayList<JPanel> sbPanel = new ArrayList<JPanel>();

	public ArrayList<Integer> playPosition = new ArrayList<Integer>();

	private JFileChooser jfc = new JFileChooser();

	public boolean playing;
	public ArrayList<Boolean> sentFlag = new ArrayList<Boolean>();

	public ArrayList<MotionData> mdList = new ArrayList<MotionData>();
	public ArrayList<Point[]> line = new ArrayList<Point[]>();

	public void adjustmentValueChanged(AdjustmentEvent e){
		for(int i = 0; i < mdList.size(); i++){
			if(sb.get(i).getValue() != playPosition.get(i)){
				pause();
				playPosition.set(i,sb.get(i).getValue());
			}
		}
		updatePoints();
	}

	public void setColor(String s, Color c){
		try{
			String searchStr = " " + s + ": ";
			int idx = -1;
			for(int i = 0; i < labelList.size(); i++){
				if(labelList.get(i).getText().equals(searchStr)){
					idx = i;
					break;
				}
			}
			if(idx < 0) return;
			JLabel tmp = labelList.get(idx);
			pv.setColor(idx,c);
			tmp.setForeground(c);
			labelList.set(idx,tmp);
		}catch(IndexOutOfBoundsException e){
			e.printStackTrace();
		}
	}

	public void actionPerformed(ActionEvent e){
		String s = e.getActionCommand();
		if(s == "Play"){
			play();
		}
		if(s == "Stop"){
			stop();
		}
		if(s == "Pause"){
			pause();
		}
		if(s == "Reset View"){
			pv.setDefaultView();
		}
	}

	public Point3DPlayer(){
		this(640,640);
	}

	public Point3DPlayer(int width, int height) {
		WIDTH = width;
		HEIGHT = height;
		initGUI();
	}

	public void initGUI(){
		JButton pl = new JButton("Play");
		pl.addActionListener(this);
		JButton pa = new JButton("Pause");
		pa.addActionListener(this);
		JButton st = new JButton("Stop");
		st.addActionListener(this);
		JPanel playButtons = new JPanel();
		playButtons.setLayout(new GridLayout(1,3));

		playButtons.add(pl);
		playButtons.add(pa);
		playButtons.add(st);

		JButton reset = new JButton("Reset View");
		reset.addActionListener(this);
		JPanel buttons = new JPanel();
		buttons.setLayout(new GridLayout(2,1));
		buttons.add(playButtons);
		buttons.add(reset);
		setLayout(new BorderLayout());

		pv = new Point3DViewer(WIDTH, HEIGHT-BUTTON_HEIGHT);
		add(pv,BorderLayout.NORTH);
		add(scrollPanel,BorderLayout.CENTER);
		add(buttons,BorderLayout.SOUTH);

		setPreferredSize(new Dimension(WIDTH, HEIGHT));
	}

	public void reSize(int w, int h){
		if(w<0||h<0)return;
		WIDTH = w;
		HEIGHT = h;
		setPreferredSize(new Dimension(WIDTH, HEIGHT));
		pv.reSize(WIDTH,HEIGHT-BUTTON_HEIGHT-sb.size()*20-70);

		scrollPanel.setLayout(new GridLayout(sb.size(),1));
		scrollPanel.revalidate();
		scrollPanel.repaint();
		revalidate();
		repaint();
	}

	public void setMovable(String s, boolean b){
		pv.setMovable(" "+s+": ",b);
	}

	public void loadData(String s){
		addData(new File("./"+s));
	}

	public void addData(String l, MotionData md, Color c, boolean b){
		JScrollBar sbTmp = new JScrollBar(JScrollBar.HORIZONTAL);
		sbTmp.setMaximum(md.size()-1+sbTmp.getVisibleAmount());
		sbTmp.addAdjustmentListener(this);
		JLabel label = new JLabel(" "+l+": ");
		label.setForeground(c);
		label.setBackground(Color.BLACK);
		label.setOpaque(true);
		labelList.add(label);
		JPanel paneTmp = new JPanel();
		paneTmp.setLayout(new BorderLayout());
		paneTmp.add(label,BorderLayout.WEST);
		paneTmp.add(sbTmp,BorderLayout.CENTER);
		sb.add(sbTmp);
		scrollPanel.add(paneTmp);
		playPosition.add(new Integer(0));
		mdList.add(md);
		line.add(md.getLine());
		sentFlag.add(false);
		reSize(WIDTH,HEIGHT);
		sendPoints(c,b);
	}

	public void rmData(String l){
		String tmp = " " + l + ": ";
		for(int i = 0; i < labelList.size(); i++){
			if(tmp.equals(labelList.get(i).getText())){
				scrollPanel.remove(i);
				labelList.remove(i);
				playPosition.remove(i);
				mdList.remove(i);
				line.remove(i);
				sentFlag.remove(i);
				sb.remove(i);
				pv.rmPoints(tmp);
				reSize(WIDTH,HEIGHT);
				return;
			}
		}
	}

	public void addData(File f){
		MotionData tmp = new MotionData();
		if(!tmp.readFile(f)) return;
		JScrollBar sbTmp = new JScrollBar(JScrollBar.HORIZONTAL);
		sbTmp.setMaximum(tmp.size()-1+sbTmp.getVisibleAmount());
		sbTmp.addAdjustmentListener(this);
		sb.add(sbTmp);
		scrollPanel.setLayout(new GridLayout(sb.size(),1));
		scrollPanel.add(sbTmp);
		playPosition.add(new Integer(0));
		mdList.add(tmp);
		line.add(tmp.getLine());
		sentFlag.add(false);
		sendPoints();
		scrollPanel.validate();
		scrollPanel.repaint();
		reSize(WIDTH,HEIGHT);
	}

	public void pause(){
		playing = false;
	}

	public void stop(){
		playing = false;
		for(int i = 0; i < mdList.size(); i++){
			playPosition.set(i,0);
			sb.get(i).setValue(0);
		}
		updatePoints();
	}

	public void updatePoints(){
		for(int i = 0; i < mdList.size(); i++){
			Vec3D[] tmp = mdList.get(i).get(playPosition.get(i));
			if(tmp == null){ continue; }
			pv.updatePoints(labelList.get(i).getText(),tmp);
		}
	}

	public void printStatus(){
		for(int i = 0; i < mdList.size(); i++){
			System.out.println(i+"= position:"+playPosition.get(i)+"/"+mdList.get(i).size()+", bar:"+sb.get(i).getValue()+"/"+sb.get(i).getMaximum());
		}
	}

	public void next(){
		boolean end = true;
		for(int i = 0; i < mdList.size(); i++){
			if(playPosition.get(i) < mdList.get(i).size()-1){
				end = false;
				playPosition.set(i,playPosition.get(i)+1);
				sb.get(i).setValue(playPosition.get(i));
			}
		}
		updatePoints();
		if(end) pause();
	}

	public void sendPoints(){
		for(int i = 0; i < mdList.size(); i++){
			if(playPosition.get(i) >= mdList.get(i).size()) continue; //playPosition = 0;
			if(!sentFlag.get(i)){
				pv.addPoints(labelList.get(i).getText(),mdList.get(i).get(playPosition.get(i)),line.get(i),Color.WHITE,true);
				sentFlag.set(i,true);
			}
		}
	}

	public void sendPoints(Color c, boolean b){
		for(int i = 0; i < mdList.size(); i++){
			if(playPosition.get(i) >= mdList.get(i).size()) continue; //playPosition = 0;
			if(!sentFlag.get(i)){
				pv.addPoints(labelList.get(i).getText(),mdList.get(i).get(playPosition.get(i)),line.get(i),c,b);
				sentFlag.set(i,true);
			}
		}
	}

	public void play(){
		boolean allEnd = true;
		for(int i = 0; i < mdList.size(); i++){
			if(playPosition.get(i) < mdList.get(i).size()-1){
				allEnd = false;
				break;
			}
		}
		if(allEnd){
			for(int i = 0; i < mdList.size(); i++){
				playPosition.set(i,0);
				sb.get(i).setValue(0);
			}
		}
		playing = true;
	}

	public void update(){
		if(playing){
			next();
		}
		pv.update();
	}

}
