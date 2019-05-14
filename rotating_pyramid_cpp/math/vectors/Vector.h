#ifndef _VETORES
#define _VETORES

class Vector {
	public:
		float x, y, z;

		Vector(): x(0.0f), y(0.0f), z(0.0f) {  }
		Vector(const float xa, const float ya, const float za): x(xa), y(ya), z(za) {  }
		Vector(const Vector& other): x(other.x), y(other.y), z(other.z) {  }

		//operador unario para reverter o vetor
		void  operator- (void);
		
		//operacoes que envolvem outros vetores
		Vector& operator= (const Vector&);
		Vector  operator+ (const Vector&) const;
		Vector  operator- (const Vector&) const;
		Vector& operator+=(const Vector&);
		Vector& operator-=(const Vector&);
		float   operator* (const Vector&) const;
		
		//operacoes com escalares
		Vector  operator* (const float)   const;
		Vector  operator/ (const float)   const;
		Vector& operator*=(const float);
		Vector& operator/=(const float);

		//utilidades
		void   normalize();
		void   reverse  ();
		float  magnitude();

};

		Vector cross    (const Vector&, const Vector&);
#endif
