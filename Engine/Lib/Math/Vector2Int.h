#pragma once

struct Vector2Int{
	int x;
	int y;

	constexpr Vector2Int(): x(0),y(0){}
	constexpr Vector2Int(int x = 0,int y = 0) : x(x),y(y){}

	constexpr Vector2Int operator+(const Vector2Int& obj) const{ return Vector2Int(x + obj.x,y + obj.y); }
	constexpr Vector2Int operator-(const Vector2Int& obj) const{ return Vector2Int(x - obj.x,y - obj.y); }
	constexpr Vector2Int operator-() const{ return Vector2Int(-x,-y); }
	constexpr Vector2Int operator*(const Vector2Int& obj) const{ return Vector2Int(x * obj.x,y * obj.y); }
	constexpr Vector2Int operator/(const Vector2Int& obj) const{ return Vector2Int(x / obj.x,y / obj.y); }

	Vector2Int& operator+=(const Vector2Int& obj){
		x += obj.x;
		y += obj.y;
		return *this;
	}
	Vector2Int& operator-=(const Vector2Int& obj){
		x -= obj.x;
		y -= obj.y;
		return *this;
	}
	Vector2Int& operator*=(const Vector2Int& obj){
		x *= obj.x;
		y *= obj.y;
		return *this;
	}
	Vector2Int& operator/=(const Vector2Int& obj){
		x /= obj.x;
		y /= obj.y;
		return *this;
	}

	bool operator==(const Vector2Int& obj) const{ return (x == obj.x) && (y == obj.y); }
	bool operator!=(const Vector2Int& obj) const{ return (x != obj.x) || (y != obj.y); }
};