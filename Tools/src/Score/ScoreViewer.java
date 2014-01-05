package Score;

import java.awt.Component;

import javax.swing.JPanel;

import Data.*;
import Util.*;

public class ScoreViewer extends JPanel implements DataChangeListener{
	private ScoreGraph graph;
	
	private MotionDataConverter mdc;
	private int WIDTH;
	private int HEIGHT;

	public ScoreViewer(MotionDataConverter mdc, int w, int h){
		this.mdc = mdc;
		mdc.addMotionDataChangeListener(this);
		WIDTH = w;
		HEIGHT = h;
		graph = new ScoreGraph(mdc,w,h);
		add(graph);
	}
	public void reSize(int w, int h){
		graph.reSize(w,h);
	}	
	public void dataChanged(DataChangeEvent e){
		Component comp = e.getComponent();
		if(comp == mdc){
			graph.readData();
			graph.update();
		}
	}
}
