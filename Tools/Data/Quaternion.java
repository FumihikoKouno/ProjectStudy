package Tools.Data;

public class Quaternion{
	private double t;
	private Vec3D v;
	public Quaternion(){
		this(1,0,0,0);
	}
	
	public double getT(){ return t; }
	public Vec3D getV(){ return v; }

	public Quaternion(double t, double x, double y, double z){
		this.t = t;
		v = new Vec3D(x,y,z);
	}
	public Quaternion(double t, Vec3D v){
		this.t = t;
		this.v = v;
	}
	public Quaternion(Vec3D from, Vec3D to){
		Vec3D axis = to.cross(from).unit();
		double cos = to.dot(from)/(to.abs()*from.abs());
		double cos2 = Math.sqrt((1.0+cos)/2.0);
		double sin2 = Math.sqrt((1.0-cos)/2.0);
		this.t = cos2;
		this.v = axis.times(sin2);
	}
	
	private Quaternion conjugate(){ return new Quaternion(t,v.times(-1.0)); }
	
	public Vec3D rotate(Vec3D from){
		Quaternion p = new Quaternion(0,from);
		p = conjugate().mul(p);
		return p.mul(this).getV();
	}
	
	public Quaternion mul(Quaternion q){
		Vec3D tmpV = q.getV();
		double tmpT = q.getT();
		return new Quaternion(t*tmpT-v.dot(tmpV), tmpV.times(t).add(v.times(tmpT).add(v.cross(tmpV))));
	}
	
	public String toString(){
		return (t + " ; " + v);
	}
	
}
