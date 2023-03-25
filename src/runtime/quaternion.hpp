#pragma once
#include"math.hpp"
#include"reflection.hpp"
#include<cassert>
namespace myengine
{
    class Matrix3x3;
    class Vector3;

    REFLECTION_TYPE(Quaternion)
        CLASS(Quaternion, Fields)
    {
        REFLECTION_BODY(Quaternion);

    public:
        float w{ 1.f }, x{ 0.f }, y{ 0.f }, z{ 0.f };

    public:
        Quaternion() = default;
        Quaternion(float w_, float x_, float y_, float z_) : w{ w_ }, x{ x_ }, y{ y_ }, z{ z_ } {}

        
        explicit Quaternion(const Matrix3x3 & rot) { this->fromRotationMatrix(rot); }
       
        Quaternion(const Radian & angle, const Vector3 & axis) { this->fromAngleAxis(angle, axis); }
        
        Quaternion(const Vector3 & xaxis, const Vector3 & yaxis, const Vector3 & zaxis)
        {
            this->fromAxes(xaxis, yaxis, zaxis);
        }

        float* ptr() { return &w; }

        
        const float* ptr() const { return &w; }

        void fromRotationMatrix(const Matrix3x3 & rotation);
        void toRotationMatrix(Matrix3x3 & rotation) const;
        void toRotationMatrix(Matrix4x4 & rotation) const;

        void fromAngleAxis(const Radian & angle, const Vector3 & axis);

        static Quaternion getQuaternionFromAngleAxis(const Radian & angle, const Vector3 & axis);

        void fromDirection(const Vector3 & direction, const Vector3 & up_direction);

        static Quaternion getQuaternionFromDirection(const Vector3 & direction, const Vector3 & up_direction);

        void toAngleAxis(Radian & angle, Vector3 & axis) const;

       
        void fromAxes(const Vector3 & x_axis, const Vector3 & y_axis, const Vector3 & z_axis);
        
        void toAxes(Vector3 & x_axis, Vector3 & y_axis, Vector3 & z_axis) const;

    
        Vector3 xAxis() const;

       
        Vector3 yAxis() const;

     
        Vector3 zAxis() const;

        Quaternion operator+(const Quaternion & rhs) const
        {
            return Quaternion(w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z);
        }

        Quaternion operator-(const Quaternion & rhs) const
        {
            return Quaternion(w - rhs.w, x - rhs.x, y - rhs.y, z - rhs.z);
        }

        Quaternion mul(const Quaternion & rhs) const { return (*this) * rhs; }
        Quaternion operator*(const Quaternion & rhs) const;

        Quaternion operator*(float scalar) const { return Quaternion(w * scalar, x * scalar, y * scalar, z * scalar); }

        
        Vector3 operator*(const Vector3 & rhs) const;

        Quaternion operator/(float scalar) const
        {
            assert(scalar != 0.0f);
            return Quaternion(w / scalar, x / scalar, y / scalar, z / scalar);
        }

        friend Quaternion operator*(float scalar, const Quaternion & rhs)
        {
            return Quaternion(scalar * rhs.w, scalar * rhs.x, scalar * rhs.y, scalar * rhs.z);
        }

        Quaternion operator-() const { return Quaternion(-w, -x, -y, -z); }

        bool operator==(const Quaternion & rhs) const
        {
            return (rhs.x == x) && (rhs.y == y) && (rhs.z == z) && (rhs.w == w);
        }

        bool operator!=(const Quaternion & rhs) const
        {
            return (rhs.x != x) || (rhs.y != y) || (rhs.z != z) || (rhs.w != w);
        }

      
        bool isNaN() const { return Math::isNan(x) || Math::isNan(y) || Math::isNan(z) || Math::isNan(w); }

        float getX() const { return x; }
        float getY() const { return y; }
        float getZ() const { return z; }
        float getW() const { return w; }

        float dot(const Quaternion & rkQ) const { return w * rkQ.w + x * rkQ.x + y * rkQ.y + z * rkQ.z; }

        float length() const { return std::sqrt(w * w + x * x + y * y + z * z); }

       
        void normalise(void)
        {
            float factor = 1.0f / this->length();
            *this = *this * factor;
        }

        Quaternion inverse() const 
        {
            float norm = w * w + x * x + y * y + z * z;
            if (norm > 0.0)
            {
                float inv_norm = 1.0f / norm;
                return Quaternion(w * inv_norm, -x * inv_norm, -y * inv_norm, -z * inv_norm);
            }
            else
            {
              
                return ZERO;
            }
        }

        Radian getRoll(bool reproject_axis = true) const;

        Radian getPitch(bool reproject_axis = true) const;

        Radian getYaw(bool reproject_axis = true) const;


        static Quaternion sLerp(float t, const Quaternion & kp, const Quaternion & kq, bool shortest_path = false);


        static Quaternion nLerp(float t, const Quaternion & kp, const Quaternion & kq, bool shortest_path = false);

        Quaternion conjugate() const { return Quaternion(w, -x, -y, -z); }

        static const Quaternion ZERO;
        static const Quaternion IDENTITY;

        static const float k_epsilon;
    };
}