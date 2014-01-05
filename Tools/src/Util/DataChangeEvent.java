package Util;

import java.awt.Component;

public class DataChangeEvent {
	private Component c;
	public DataChangeEvent(Component c){
		this.c = c;
	}
	public Component getComponent(){
		return c;
	}
}
