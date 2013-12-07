package Runner;

import java.util.ArrayList;
import java.util.Arrays;

import javax.swing.JPanel;
import javax.swing.JEditorPane;
import javax.swing.JLabel;
import javax.swing.JButton;
import javax.swing.ButtonGroup;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.JFileChooser;

import javax.swing.border.TitledBorder;

import java.awt.FlowLayout;
import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.Dimension;

import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

import java.io.IOException;
import java.io.BufferedInputStream;
import java.io.File;

import Data.MotionDataConverter;

public class Runner extends JPanel implements ActionListener, Runnable{
	private int WIDTH, HEIGHT;

	private MotionDataConverter mdc;

	public JFileChooser fc = new JFileChooser();

	public String selectedDir;
	public JTextField path = new JTextField("./main.exe");
	private JButton runButton = new JButton("Run");
	private JButton selectButton = new JButton("Select File");
	private boolean end = true;

	public Runner(MotionDataConverter mdc){
		this.mdc = mdc;
		init();
	}
	public void init(){
		setLayout(new BorderLayout());
		JPanel pathPanel = new JPanel();
		pathPanel.setLayout(new FlowLayout());
		pathPanel.add(new JLabel("MiMiK Path : "));
		pathPanel.add(path);
		pathPanel.add(selectButton);
		selectButton.addActionListener(this);
		JPanel runPanel = new JPanel();
		runPanel.setLayout(new FlowLayout(FlowLayout.RIGHT));
		runPanel.add(runButton);
		runButton.addActionListener(this);
		add(runPanel,BorderLayout.EAST);
		add(pathPanel,BorderLayout.WEST);
	}
	public void update(){
		revalidate();
		repaint();
	}


	public void run(){
		if(!end)return;
		end = false;
		try{
			Runtime rt = Runtime.getRuntime();
			String[] str = new String[1];
			str[0] = path.getText();
			Process process = rt.exec(str);
			String tmp = "";
			byte[] output = new byte[1];
			BufferedInputStream in = new BufferedInputStream(process.getInputStream());
			int charactor = 0;
			String[] line;
			while((charactor = in.read(output)) >= 0){
				if(charactor > 0){
					tmp += new String(output);
					if(tmp.indexOf("\r\n")>0){
						line = tmp.split("\r\n");
					}else if(tmp.indexOf("\r")>0){
						line = tmp.split("\r");
					}else if(tmp.indexOf("\n")>0){
						line = tmp.split("\n");
					}else{
						line = new String[1];
						line[0] = tmp;
					}
					for(int i = 0; i < line.length; i++){
						mdc.readFile(line[i]);
					}
				}
			}
		}catch(IOException e){
			e.printStackTrace();
		}
		end = true;
	}

	public void actionPerformed(ActionEvent e){
		String str = e.getActionCommand();
		if(str.equals("Run")){
		    new Thread(this).start();
		}
		if(str.equals("Select File")){
			if(fc.showOpenDialog(this)==JFileChooser.APPROVE_OPTION){
				File tmp = fc.getSelectedFile();
				selectedDir = tmp.getParent();
				path.setText(tmp.getAbsolutePath());
			}
		}
	}

}
