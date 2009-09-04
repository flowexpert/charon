class Point4D
{
	public:
		unsigned int x, y, z, t;
		
		///default constructor
		Point4D(int x, int y, int z, int t) : x(x), y(y), z(z), t(t) {}
		
		///copy constructor
		Point4D(Point4D rhs) {
			this->x = rhs.x;
			this->y = rhs.y;
			this->z = rhs.z;
			this->t = rhs.t;
		}
		
		///assignment operator
		Point4D& operator= (const Point4D &rhs) {
			if(&rhs == this) {return *this;}
			this->x = rhs.x;
			this->y = rhs.y;
			this->z = rhs.z;
			this->t = rhs.t;
			return *this;
		}
		
		///compound addition operator
		Point4D& operator+= (const Point4D &rhs) {
			this->x += rhs.x;
			this->y += rhs.y;
			this->z += rhs.z;
			this->t += rhs.t;
			return *this;
		}
		
		///addition operator
		Point4D& operator+ (const Point4D &rhs) {
			return Point4D(*this) += rhs;
		}
		
		unsigned int volume() {
			unsigned int v = x * y * z * t;
			return v;
		}
		
		~Point4D();
}