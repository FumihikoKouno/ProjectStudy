package Data;

public class Vec3D{
	private double x, y, z;
	public Vec3D(){ this(0,0,0); }
	public Vec3D(double x, double y, double z){
		set(x,y,z);
	}
	public Vec3D(Vec3D v){
		set(v.getX(),v.getY(),v.getZ());
	}
	public Vec3D(double[] d){
		try{
			set(d[0],d[1],d[2]);
		}catch(IndexOutOfBoundsException e){
			set(0,0,0);
		}
	}
	public Vec3D(String s){
		try{
			String[] list = s.split(" ");
			double[] d = new double[3];
			for(int i = 0; i < 3; i++){
				d[i] = Double.parseDouble(list[i]);
			}
			set(d[0],d[1],d[2]);
		}catch(NumberFormatException e){
			set(0,0,0);
		}catch(IndexOutOfBoundsException e){
			set(0,0,0);
		}
	}
	public Vec3D clone(){
		return new Vec3D(this);
	}
	public Vec3D unit(){
		if(abs() == 0) return new Vec3D();
		else return times(1.0/abs());
	}
	public boolean equals(Vec3D v){
		return x == v.x && y == v.y && z == v.z;
	}
	public Vec3D times(double n){
		return new Vec3D(n*x, n*y, n*z);
	}
	public double dot(Vec3D v){
		return x*v.x+y*v.y+z*v.z;
	}
	public Vec3D cross(Vec3D v){
		return new Vec3D(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x);
	}
	public double getAngle(Vec3D v){
		return Math.acos(unit().dot(v.unit()));
	}
	public Vec3D add(Vec3D v){
		return new Vec3D(x+v.x,y+v.y,z+v.z);
	}
	public Vec3D sub(Vec3D v){
		return new Vec3D(x-v.x,y-v.y,z-v.z);
	}
	public void set(double x, double y, double z){
		setX(x);
		setY(y);
		setZ(z);
	}
	public double getX(){ return x; }
	public double getY(){ return y; }
	public double getZ(){ return z; }
	public double abs(){ return Math.sqrt(x*x + y*y + z*z); }
	public void setX(double x){ this.x = x; }
	public void setY(double y){ this.y = y; }
	public void setZ(double z){ this.z = z; }

	public String toString(){
		return x + " " + y + " " + z;
	}
}
