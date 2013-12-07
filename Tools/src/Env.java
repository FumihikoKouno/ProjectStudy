import java.util.HashMap;
import java.util.Iterator;

import java.io.IOException;
import java.io.File;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.FileWriter;
import java.io.BufferedWriter;
import java.io.PrintWriter;

import Data.Vec3D;

public class Env{
	private static Env env;
	private HashMap<String,String> map;
	private Main main;
	
	private Env(Main main){
		this.main = main;
		map = new HashMap<String,String>();
	}
	
	public void readFile(){
		File envFile = new File("env.txt");
		if(envFile.exists()){
			try{
				BufferedReader reader = new BufferedReader(new FileReader(envFile));
				String line;
				while((line=reader.readLine())!=null){
					String[] entry = line.split("=");
					for(int i = 2; i < entry.length; i++){
						entry[1] += entry[i];
					}
					map.put(entry[0],entry[1]);
				}
				reader.close();
				setEnv();
			}catch(IOException e){
				e.printStackTrace();
			}
		}
	}
	
	public void end(){
		getCurrentEnv();
		write();
	}
	
	private void setEnv(){
		try{
			main.setBounds(Integer.parseInt(map.get("X")),Integer.parseInt(map.get("Y")),Integer.parseInt(map.get("WIDTH")),Integer.parseInt(map.get("HEIGHT")));
			main.splitPane.setDividerLocation(Integer.parseInt(map.get("VerticalSplit")));
			main.right.setDividerLocation(Integer.parseInt(map.get("HorizontalSplit")));
			
			main.mdc.readFile("m:"+map.get("ModelData"));
			main.mdc.readFile("u:"+map.get("UserData"));
			main.mdc.readFile("c:"+map.get("ConvertData"));
			main.mdc.fileChooser.setCurrentDirectory(new File(map.get("MotionDataConverterDir")));
			main.mdc.selectedDir = map.get("MotionDataConverterDir");
			
			main.runner.path.setText(map.get("MiMiKPath"));
			main.runner.fc.setCurrentDirectory(new File(map.get("RunnerDir")));
			main.runner.selectedDir = map.get("RunnerDir");
			
			main.playerOption.perspective = Integer.parseInt(map.get("PerspectiveMethod"));
			main.playerOption.fps = Integer.parseInt(map.get("FPS"));
			main.playerOption.rot = new Vec3D(map.get("RotateOrigin"));
			main.playerOption.onePP = new Vec3D(map.get("OneVanishmentPoint"));
			main.playerOption.viewR = map.get("ShowRotateOrigin").equals("true");
			main.playerOption.viewO = map.get("ShowOneVanishmentPoint").equals("true");
			main.playerOption.setValues();
			
		}catch(NullPointerException e){
			System.out.println("NULLPO");
		}
	}
	
	private void getCurrentEnv(){
		String currentDir = System.getProperty("user.dir");
		map.put("X",""+main.getBounds().x);
		map.put("Y",""+main.getBounds().y);
		map.put("WIDTH",""+main.getBounds().width);
		map.put("HEIGHT",""+main.getBounds().height);
		map.put("VerticalSplit",""+main.splitPane.getDividerLocation());
		map.put("HorizontalSplit",""+main.right.getDividerLocation());
		if(main.mdc.selectedFile[0] != null){
			map.put("ModelData",main.mdc.selectedFile[0].getAbsolutePath());
		}else{
			map.put("ModelData","null");
		}
		if(main.mdc.selectedFile[1] != null){
			map.put("UserData",main.mdc.selectedFile[1].getAbsolutePath());
		}else{
			map.put("UserData","null");
		}
		if(main.mdc.selectedFile[2] != null){
			map.put("ConvertData",main.mdc.selectedFile[2].getAbsolutePath());
		}else{
			map.put("ConvertData","null");
		}
		if(main.mdc.selectedDir != null){
			map.put("MotionDataConverterDir",main.mdc.selectedDir);
		}else{
			map.put("MotionDataConverterDir",currentDir);
		}
		File tmp = new File(main.runner.path.getText());
		map.put("MiMiKPath",tmp.getAbsolutePath());
		if(main.runner.selectedDir != null){
			map.put("RunnerDir",main.runner.selectedDir);
		}else{
			map.put("RunnerDir",currentDir);
		}
		
		map.put("PerspectiveMethod",""+main.playerOption.perspective);
		map.put("FPS",""+main.playerOption.fps);
		map.put("RotateOrigin",""+main.playerOption.rot);
		map.put("OneVanishmentPoint",""+main.playerOption.onePP);
		map.put("ShowRotateOrigin",""+main.playerOption.viewR);
		map.put("ShowOneVanishmentPoint",""+main.playerOption.viewO);
	}
	
	private void write(){
		try{
			PrintWriter pw = new PrintWriter(new BufferedWriter(new FileWriter(new File("env.txt"))));
			Iterator it = map.keySet().iterator();
			while (it.hasNext()) {
				String key = (String)it.next();
				pw.println(key+"="+map.get(key));
			}
			pw.close();
		}catch(IOException e){
			e.printStackTrace();
		}
	}
	
	public String get(String s){
		return "test";
	}
	
	public static Env getInstance(){
		return env;
	}
	
	public static Env getInstance(Main main){
		if(env == null) env = new Env(main);
		return env;
	}
}
