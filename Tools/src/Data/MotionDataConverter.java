package Data;

import java.awt.Dimension;

import javax.swing.JPanel;
import javax.swing.JLabel;
import javax.swing.JButton;
import javax.swing.JFileChooser;

import javax.swing.border.TitledBorder;
import javax.swing.JRadioButton;
import javax.swing.ButtonGroup;

import java.awt.GridLayout;
import java.awt.FlowLayout;
import java.awt.BorderLayout;
import java.awt.Color;

import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

import java.io.File;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.FileNotFoundException;

import java.util.ArrayList;
import Point3D.Point3DPlayer;
import Util.DataChangeListener;
import Util.DataChangeEvent;

public class MotionDataConverter extends JPanel implements ActionListener{
	public int WIDTH,HEIGHT;
	public JFileChooser fileChooser = new JFileChooser();

	public ArrayList<DataChangeListener> motionDataChangeListeners = new ArrayList<DataChangeListener>();
	
	public String selectedDir;
	public File[] selectedFile = new File[3];
	public MotionData[] data = new MotionData[3];
	public JButton[] selectFileButton = new JButton[3];
	public JLabel[] fileNameLabel = new JLabel[3];
	public JLabel[] fileLengthLabel = new JLabel[3];
	public JPanel[] panels = new JPanel[3];
	public JRadioButton[] showButtons = new JRadioButton[3];
	public JRadioButton[] movableButtons = new JRadioButton[3];

	public static final int COLOR_NUM = 3;
	public static Color[] colors =
	{
		Color.WHITE,
		Color.RED,
		Color.YELLOW,
	};

	public JRadioButton[][] colorButtons = new JRadioButton[3][COLOR_NUM];

	public JButton convertButton = new JButton("Convert");
	    public JButton[] filterButton = new JButton[3];

	public Point3DPlayer pp;

	public MotionDataConverter(Point3DPlayer p){
		pp = p;
		init();
	}

	public MotionDataConverter(int w, int h){
		WIDTH = w;
		HEIGHT = h;
		init();
		setPreferredSize(new Dimension(WIDTH,HEIGHT));
	}

	public void notifyMotionDataChangeListener(){
		for(int i = 0 ; i < motionDataChangeListeners.size(); i++)
			motionDataChangeListeners.get(i).dataChanged(new DataChangeEvent(this));
	}
	
	public void addMotionDataChangeListener(DataChangeListener listener){
		motionDataChangeListeners.add(listener);
	}

	public void init(){
		setLayout(new BorderLayout());
		JPanel parts = new JPanel();
		parts.setLayout(new GridLayout(3,1));
		String[] colorStrTmp = {"White","Red","Yellow"};
		for(int i = 0; i < 3; i++){
			ButtonGroup tmpGroup = new ButtonGroup();
			for(int j = 0; j < COLOR_NUM; j++){
				if(j == 0){
					colorButtons[i][j] = new JRadioButton(colorStrTmp[j],true);
				}else{
					colorButtons[i][j] = new JRadioButton(colorStrTmp[j]);
				}
				colorButtons[i][j].addActionListener(this);
				tmpGroup.add(colorButtons[i][j]);
			}
			if(i == 2){
				selectFileButton[i] = new JButton("Save File");
			}else{
				selectFileButton[i] = new JButton("Select File");
			}
			    filterButton[i] = new JButton("Filter");
			    filterButton[i].addActionListener(this);
			movableButtons[i] = new JRadioButton("Movable");
			movableButtons[i].addActionListener(this);
			selectFileButton[i].addActionListener(this);
			data[i] = new MotionData();
			fileNameLabel[i] = new JLabel("File Name : null");
			fileLengthLabel[i] = new JLabel("0");
			showButtons[i] = new JRadioButton("show",false);
			showButtons[i].addActionListener(this);
		}
		String[] strTmp = {"Model Data", "User Data", "Result Data"};

		for(int i = 0; i < 3; i++){
			JPanel tmpPanel = new JPanel();
			tmpPanel.setLayout(new GridLayout(3,1));
			tmpPanel.add(fileNameLabel[i]);
			tmpPanel.add(fileLengthLabel[i]);
			JPanel playerButtons = new JPanel();
			playerButtons.setLayout(new GridLayout(1,2));
			playerButtons.add(showButtons[i]);
			playerButtons.add(movableButtons[i]);
			tmpPanel.add(playerButtons);
			panels[i] = new JPanel();
			panels[i].setLayout(new BorderLayout());
			panels[i].add(tmpPanel,BorderLayout.NORTH);
			tmpPanel = new JPanel();
			tmpPanel.setLayout(new GridLayout(1,3));
			for(int j = 0; j < 3; j++){
				tmpPanel.add(colorButtons[i][j]);
			}
			tmpPanel.setBorder(new TitledBorder("Color"));
			panels[i].add(tmpPanel,BorderLayout.CENTER);
			    tmpPanel = new JPanel();
			    tmpPanel.setLayout(new GridLayout(2,1));
			    tmpPanel.add(filterButton[i]);
			    tmpPanel.add(selectFileButton[i]);
			panels[i].add(tmpPanel,BorderLayout.SOUTH);
			panels[i].setBorder(new TitledBorder(strTmp[i]));
		}

		for(int i = 0; i < 3; i++){
			parts.add(panels[i]);
		}

		convertButton.addActionListener(this);
		add(parts,BorderLayout.CENTER);
		add(convertButton,BorderLayout.SOUTH);
		/*
		add(parts);
		add(convertButton);
		*/
	}


	public void readFile(String s){
		if(s.length() < 3) return;
		String[] idx = {"m","u","c"};
		File f = new File(s.substring(2));
		if(!f.isFile()) return;
		for(int i = 0; i < idx.length; i++){
			if(s.startsWith(idx[i])){
				if(!data[i].readFile(f)) return;
				selectedFile[i] = f;
				fileNameLabel[i].setText("File Name : " + f.getName());
				fileLengthLabel[i].setText("" + data[i].size()+ " frame");
				notifyMotionDataChangeListener();
				break;
			}
		}
	}

	public void actionPerformed(ActionEvent e){
		for(int i = 0; i < 2; i++){
			if(e.getSource() == selectFileButton[i]){
				if(fileChooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION){
					selectedFile[i] = fileChooser.getSelectedFile();
					selectedDir = selectedFile[i].getParent();
					if(!data[i].readFile(selectedFile[i])) return;
					fileNameLabel[i].setText("File Name : " + selectedFile[i].getName());
					fileLengthLabel[i].setText("" + data[i].size() + " frame");
					pp.updatePoints();
					notifyMotionDataChangeListener();
				}
			}
		}
		if(e.getSource() == selectFileButton[2]){
			try{
				if(fileChooser.showSaveDialog(this) == JFileChooser.APPROVE_OPTION){
					selectedFile[2] = fileChooser.getSelectedFile();
					selectedDir = selectedFile[2].getParent();
					BufferedWriter bw = new BufferedWriter(new FileWriter(selectedFile[2]));
					bw.write(""+data[2]);
					fileNameLabel[2].setText("File Name : " + selectedFile[2].getName());
					fileLengthLabel[2].setText("" + data[2].size() + " frame");
					bw.close();
					notifyMotionDataChangeListener();
				}
			}catch(NullPointerException npe){
			}catch(IOException ie){
			}
		}
		String[] strTmp = {"Model", "User", "Result"};
		for(int i = 0; i < 3; i++){
		    if(e.getSource() == filterButton[i]){
			if(data[i].size() == 0) return;
			else data[i].filter(0);
		    }
			if(e.getSource() == showButtons[i]){
				if(data[i].size() == 0) return;
				else{
					int colorIdx = -1;
					for(int j = 0; j < COLOR_NUM; j++){
						if(colorButtons[i][j].isSelected()){
							colorIdx = j;
							break;
						}
					}
					if(showButtons[i].isSelected()){
						pp.addData(strTmp[i],data[i],colors[colorIdx],movableButtons[i].isSelected());
					}else{
						pp.rmData(strTmp[i]);
					}
				}
			}
		}
		for(int i = 0; i < 3; i++){
			for(int j = 0; j < COLOR_NUM; j++){
				if(e.getSource() == colorButtons[i][j]){
					if(showButtons[i].isSelected()) pp.setColor(strTmp[i],colors[j]);
					break;
				}
			}
		}
		for(int i = 0; i < 3; i++){
			if(e.getSource() == movableButtons[i]){
				if(data[i].size() == 0) return;
				else{
					if(movableButtons[i].isSelected()){
						pp.setMovable(strTmp[i],true);
					}else{
						pp.setMovable(strTmp[i],false);
					}
				}
			}
		}
		if(e.getSource() == convertButton){
			try{
				data[2] = data[1].convertAll(data[0]);
				fileNameLabel[2].setText("File Name : New File");
				fileLengthLabel[2].setText("" + data[2].size() + " frame");
				pp.updatePoints();
			}catch(NullPointerException npe){
			}
		}
	}

	public void reSize(int w, int h){
		WIDTH = w;
		HEIGHT = h;
		setPreferredSize(new Dimension(WIDTH, HEIGHT));
		validate();
		repaint();
	}
}
