import java.awt.Container;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Component;

import javax.swing.JFrame;
import javax.swing.JTabbedPane;
import javax.swing.JSplitPane;
import javax.swing.JPanel;

import javax.swing.JMenuBar;
import javax.swing.JMenu;
import javax.swing.JMenuItem;

import java.awt.event.ComponentListener;
import java.awt.event.ComponentEvent;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import Point3D.*;
import Data.*;
import Runner.*;
import Score.*;
import Util.*;

public class Main extends JFrame implements ComponentListener, DataChangeListener{
	public Point3DPlayer pp;
	public MotionDataConverter mdc;
	public Runner runner;
	public Env env;
	public ScoreViewer sv;
	public JSplitPane splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT);
	public JSplitPane right = new JSplitPane(JSplitPane.VERTICAL_SPLIT);
	public JTabbedPane tab = new JTabbedPane();
	
	Container contentPane;
	public Point3D.Option playerOption;

	public Main() {
		setTitle("Main");
		playerOption = new Point3D.Option(this);
		pp = new Point3DPlayer(480,480);
		pp.addComponentListener(this);
		mdc = new MotionDataConverter(pp);
		mdc.addComponentListener(this);
		runner = new Runner(mdc);
		runner.addComponentListener(this);
		sv = new ScoreViewer(mdc,480,480);
		sv.addComponentListener(this);
		sv.addDataChangeListener(this);
		tab.add("Player",pp);
		tab.add("Score",sv);
		JPanel left = new JPanel();
		left.add(mdc);
		right.setLeftComponent(tab);
		right.setRightComponent(runner);
		splitPane.setLeftComponent(left);
		splitPane.setRightComponent(right);
		contentPane = getContentPane();
		contentPane.add(splitPane);
		makeMenu();
		pack();
	}

	public void makeMenu(){
		JMenuBar menuBar = new JMenuBar();
		JMenu menuFile = new JMenu("File");
		JMenuItem exit = new JMenuItem("Exit");
		exit.addActionListener(
			new ActionListener(){
				public void actionPerformed(ActionEvent e){
					System.exit(0);
				}
			}
		);
		menuFile.add(exit);
		JMenu menuOption = new JMenu("Option");
		JMenuItem playerOption = new JMenuItem("Player Option");
		//		JMenuItem kimoissOption = new JMenuItem("Kimoiss Option");
		playerOption.addActionListener(
			new ActionListener(){
				public void actionPerformed(ActionEvent e){
					callPlayerOption();
				}
			}
		);
		menuOption.add(playerOption);
		//		menuOption.add(kimoissOption);
		menuBar.add(menuFile);
		menuBar.add(menuOption);
		setJMenuBar(menuBar);
	}

	public void callPlayerOption(){
		playerOption.setVisible(true);
	}

	public void run(){
		right.setDividerLocation(right.getMaximumDividerLocation());
		splitPane.setDividerLocation(splitPane.getMinimumDividerLocation());
		env = Env.getInstance(this);
		env.readFile();
		int mspf;
		long time;
		while(true){
			mspf = 1000/Point3D.Option.fps;
			try{
				time = System.currentTimeMillis();
				update();
				time = System.currentTimeMillis()-time;
				if(mspf > time){
					Thread.sleep(mspf-time);
				}else{
					System.out.println("SlowDown");
				}
			}catch(InterruptedException e){
				e.printStackTrace();
			}
		}
	}

	public void load(String[] args){
		for(int i = 0; i < args.length; i++){
			pp.loadData(args[i]);
		}
	}

	public void update(){
		if(tab.getSelectedComponent() == pp) pp.update();
//		pp.update();
	}

	public void dataChanged(DataChangeEvent e){
		Component comp = e.getComponent();
		if(comp == sv){
			tab.setSelectedComponent(pp);
		}
	}
	
	public void componentResized(ComponentEvent e){
		Dimension frameD = getSize();
		Dimension mdcD = mdc.getSize();
		Dimension runnerD = runner.getSize();
		int newW = frameD.width-mdcD.width-38;
		int newH = frameD.height-runnerD.height-102;
		pp.reSize(newW,newH);
		sv.reSize(newW,newH);
		contentPane.validate();
		contentPane.repaint();
	}
	public void componentMoved(ComponentEvent e){
	}
	public void componentShown(ComponentEvent e){
	}
	public void componentHidden(ComponentEvent e){
	}

	public static void main(String[] args) {
		final Main frame = new Main();
		frame.load(args);
		frame.addWindowListener(
			new WindowAdapter(){
				public void windowClosing(WindowEvent e){
					frame.env.end();
					System.exit(0);
				}
			}
		);
		frame.setVisible(true);
		frame.run();
	}
}