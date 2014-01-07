package Score;

import java.awt.Component;
import java.util.ArrayList;

import javax.swing.JPanel;

import Data.*;
import Util.*;

public class ScoreViewer extends JPanel implements DataChangeListener{
	private ScoreGraph graph;
	
	private MotionDataConverter mdc;
	private int WIDTH;
	private int HEIGHT;
	private ArrayList<DataChangeListener> dataChangeListeners = new ArrayList<DataChangeListener>();

	public ScoreViewer(MotionDataConverter mdc, int w, int h){
		this.mdc = mdc;
		mdc.addDataChangeListener(this);
		WIDTH = w;
		HEIGHT = h;
		graph = new ScoreGraph(mdc,w,h);
		graph.addDataChangeListener(this);
		add(graph);
	}
	public void reSize(int w, int h){
		graph.reSize(w,h);
	}	
	public void addDataChangeListener(DataChangeListener l){
		dataChangeListeners.add(l);
	}
	public void notifyDataChangeListener(){
		for(DataChangeListener l : dataChangeListeners){
			l.dataChanged(new DataChangeEvent(this));
		}
	}
	public void dataChanged(DataChangeEvent e){
		Component comp = e.getComponent();
		if(comp == mdc){
			graph.readData();
			graph.update();
		}
		if(comp == graph){
			if(graph.getClickStatus()){
				mdc.setViewFrame(graph.getFrame());
				notifyDataChangeListener();
			}
		}
	}
}
