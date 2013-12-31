package Point3D;

import java.awt.Color;
import java.awt.Color;
import java.awt.Graphics;
import Data.*;

public class DrawnData {
	private Vec3D origin;
	private DrawnPointData[][] data;
	private double zoom;
	private int width, height;
	public DrawnData(int w, int h){
		data = new DrawnPointData[h][w];
		width = w;
		height = h;
	}
	
	public void setOrigin(Vec3D o){
		origin = o;
	}
	public void setZoom(double z){
		zoom = z;
	}
	
	public void reset(){
		data = new DrawnPointData[height][width];
	}
	
	public void resize(int w, int h){
		data = new DrawnPointData[h][w];
		width = w;
		height = h;
	}
	
	public void setRect(Vec3D p1, Vec3D p2, Vec3D p3, Vec3D p4, Color c){
		Vec3D[] tmp = new Vec3D[4];
		tmp[0] = p1;
		tmp[1] = p2;
		tmp[2] = p3;
		tmp[3] = p4;
		setRect(tmp,c);
	}
	public void setRect(Vec3D[] p, Color c){
		Vec3D[] line = new Vec3D[2];
		for(int i = 0; i < 2; i++) line[i] = p[i+2].sub(p[i]);
	
		int minX = Integer.MAX_VALUE;
		int maxX = Integer.MIN_VALUE;
		int minY = Integer.MAX_VALUE;
		int maxY = Integer.MIN_VALUE;
		for(int i = 0; i < 4; i++){
			int tmpX = (int)(p[i].getX()*zoom);
			int tmpY = (int)(p[i].getY()*zoom);
			minX = (minX<tmpX ? minX:tmpX);
			maxX = (maxX>tmpX ? maxX:tmpX);
			minY = (minY<tmpY ? minY:tmpY);
			maxY = (maxY>tmpY ? maxY:tmpY);
		}

		double[] vx = new double[2];
		double[] vy = new double[2];
		for(int i = 0; i < 2; i++){
			vx[i] = line[i].getX();
			vy[i] = line[i].getY();
		}
		double d = vx[0]*vy[1]-vy[0]*vx[1];
		double epsilon = 0.00001;
		if(Math.abs(d)<epsilon){
			for(int i = 0; i < p.length; i++){
				for(int j = i+1; j < p.length; j++){
					line[1] = p[j].sub(p[i]);
					vx[1] = line[1].getX();
					vy[1] = line[1].getY();
					d = vx[0]*vy[1]-vy[0]*vx[1];
					if(Math.abs(d)>epsilon)break;
				}
				if(Math.abs(d)>epsilon)break;
			}
			if(Math.abs(d)<epsilon) return;
		}
		/*
		 * 二つのlineを含む平面上のベクトル
		 * xAxis = (1,0,z1), yAxis = (0,1,z2)を算出
		 */
		double[] xk = new double[2];
		double[] yk = new double[2];
		xk[0] = vy[1]/d;
		xk[1] = -vy[0]/d;
		yk[0] = -vx[1]/d;
		yk[1] = vx[0]/d;
 		Vec3D xAxis = line[0].times(xk[0]).add(line[1].times(xk[1]));
		Vec3D yAxis = line[0].times(yk[0]).add(line[1].times(yk[1]));

		Vec3D tmpP0 = xAxis.times(p[0].getX());
		tmpP0 = tmpP0.add(yAxis.times(p[0].getY()));
		tmpP0 = p[0].sub(tmpP0);

		Vec3D[] rectangle = new Vec3D[4];
		for(int i = 0; i < p.length; i++)
			rectangle[i] = p[i].times(zoom);
		
		for(int y = minY; y <= maxY; y++){
			int absY = (int)(origin.getY()-y);
			if(absY < 0 || absY >= height) continue;
			for(int x = minX; x <= maxX; x++){
				int absX = (int)(origin.getX()+x);
				if(absX < 0 || absX >= width) continue;

				if(!inRect(x,y,rectangle)){
					continue;
				}
				/*
				 *  二つのベクトルの一時結合でその点が表せれば
				 *  その二つのベクトルがなす平面上に点が存在することになる
				 */
				double z = (x*xAxis.getZ()
						  + y*yAxis.getZ()
						  + tmpP0.getZ());

				if(data[absY][absX] == null) data[absY][absX] = new DrawnPointData(z,c);
				else data[absY][absX].set(z,c);
			}
		}
	}
	
	public void setLine(Vec3D v1, int r1, Vec3D v2, int r2, Color c){
		Vec3D diff = v2.sub(v1);
		Vec3D verticle = new Vec3D(-diff.getY(),diff.getX(),0);
		verticle = verticle.unit();
		Vec3D[] p = new Vec3D[4];
		/*
		 * |p1x|   |v1x| +       |v1y-v2y |
		 * |p1y| = |v1y|    r1 * |-v1x+v2x|
		 * |p1z|   |v1z| -       |   0    |
		 */
		p[0] = v1.add(verticle.times(r1));
		p[1] = v1.sub(verticle.times(r1));
		/*
		 * |p2x|   |v2x| +       |v1y-v2y |
		 * |p2y| = |v2y|    r2 * |-v1x+v2x|
		 * |p2z|   |v2z| -       |   0    |
		 */
		p[2] = v2.sub(verticle.times(r2));
		p[3] = v2.add(verticle.times(r2));
		setRect(p,c);
	}
	
	public void setLine(double x1, double y1, double z1, int r1, int x2, int y2, int z2, int r2, Color c){
		setLine(new Vec3D(x1,y1,z1),r1, new Vec3D(x2,y2,z2), r2, c);
	}
	
	private boolean is2DCloss(Vec3D[] v1, Vec3D[] v2){
		double x1 = v1[0].getX();
		double x2 = v1[1].getX();
		double x3 = v2[0].getX();
		double x4 = v2[1].getX();
		double y1 = v1[0].getY();
		double y2 = v1[1].getY();
		double y3 = v2[0].getY();
		double y4 = v2[1].getY();
		double ta = (x3-x4)*(y1-y3)+(y3-y4)*(x3-x1);
		double tb = (x3-x4)*(y2-y3)+(y3-y4)*(x3-x2);
		double tc = (x1-x2)*(y3-y1)+(y1-y2)*(x1-x3);
		double td = (x1-x2)*(y4-y1)+(y1-y2)*(x1-x4);
		return (ta*tb < 0 && tc*td < 0);
	}
	
	private boolean inRect(int x, int y, Vec3D[] q){
		Vec3D[] in = new Vec3D[2];
		in[0] = new Vec3D(x,y,0);
		in[1] = new Vec3D(Integer.MAX_VALUE,y,0);
		int count = 0;
		for(int i = 0; i < q.length; i++){
			Vec3D[] tmp = new Vec3D[2];
			tmp[0] = q[i];
			tmp[1] = q[(i+1)%q.length];
			if(is2DCloss(in,tmp)) count++;
		}
		return ((count&1)>0);
		/*
		Vec3D[] p = q.clone();
		double minDot = Double.MAX_VALUE;
		int[] axises = new int[2];
		int against = 0;
		for(int i = 1; i < p.length; i++){
			for(int j = i+1; j < p.length; j++){
				Vec3D pi = p[i].clone();
				Vec3D pj = p[j].clone();
				pi.setZ(0);
				pj.setZ(0);
				pi = pi.unit();
				pj = pj.unit();
				double tmpDot = pi.dot(pj);
				if(minDot > tmpDot){
					minDot = tmpDot;
					axises[0] = i;
					axises[1] = j;
					for(int k = 1; k < p.length; k++){
						if(k != i && k != j) against = k;
					}
 				}
			}
		}		
		// p[against]のp[axises[0]],p[axises[1]]のを軸としたときの係数をaで表わす
		// x,yを表すための係数が2つとも小さいかつ0以上ならtrue
		// それ以外はfalse
		// zは考えなくてよい
		double[] a = getXYCoefficient(p[against],p[axises[0]],p[axises[1]]);
		double[] tmpC = getXYCoefficient(new Vec3D(x,y,0),p[axises[0]],p[axises[1]]);
		if(a==null || tmpC == null) return false;
		return (tmpC[0]<=a[0]&&tmpC[1]<=a[1]);
		*/
	}
	
	private double[] getXYCoefficient(Vec3D z, Vec3D v1, Vec3D v2){
		double[] ret = new double[2];
		double d = (v1.getX()*v2.getY()-v1.getY()*v2.getX());
		if(d == 0) return null;
		ret[0] =(v2.getY()*z.getX() - v2.getX()*z.getY())/d;
		ret[1] =-(v1.getY()*z.getX() - v1.getX()*z.getY())/d;
		return ret;
	}
	
	public void setCircle(Vec3D v, int size, Color c){
		setCircle(v.getX(), v.getY(), v.getZ(), size, c);
	}
	public void setCircle(double x, double y, double z, int r, Color c){
		int ix = (int)(x*zoom);
		int iy = (int)(y*zoom);
		for(int h = -r; h <= r; h++){
			int absY = (int)origin.getY()-(iy+h);
			if(absY < 0 || absY >= height) continue;
			for(int w = -r; w <= r; w++){
				int absX = (int)origin.getX()+(ix+w);
				if(absX < 0 || absX >= width) continue;
				if(w*w+h*h<=r*r){
					if(data[(int)origin.getY()-(iy+h)][(int)origin.getX()+(ix+w)]==null) data[(int)origin.getY()-(iy+h)][(int)origin.getX()+(ix+w)] = new DrawnPointData(z,c);
					else data[(int)origin.getY()-(iy+h)][(int)origin.getX()+(ix+w)].set(z,c);
				}
			}
		}
	}
	
	private class DrawnPointData{
		private double z;
		private Color color;
		public DrawnPointData(double z, Color c){
			this.z = z;
			this.color = c;
		}
		public void set(double z, Color c){
			if(this.z > z){
				this.z = z;
				this.color = c;
			}
		}
		public Color getColor(){
			return color;
		}
	}
	
	public void draw(Graphics g, double zoom){		
		try{
			for(int y = 0; y < height; y++){
				for(int x = 0; x < width; x++){
					if(data[y][x] != null){
						g.setColor(data[y][x].getColor());
						g.fillRect(x, y, 1, 1);
					}
				}
			}
		}catch(NullPointerException e){
		}catch(IndexOutOfBoundsException e){
			
		}
	}
}
