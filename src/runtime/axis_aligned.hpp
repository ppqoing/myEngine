#include"reflection.hpp"
#include"vector3.hpp"
#include<limits>
namespace myengine
{
    REFLECTION_TYPE(AxisAlignedBox)
        CLASS(AxisAlignedBox, Fields)
    {
        REFLECTION_BODY(AxisAlignedBox)
    public:
        AxisAlignedBox() {}
        AxisAlignedBox(const Vector3 & center, const Vector3 & half_extent);

        void merge(const Vector3 & new_point);
        void update(const Vector3 & center, const Vector3 & half_extent);

        const Vector3& getCenter() const { return m_center; }
        const Vector3& getHalfExtent() const { return m_half_extent; }
        const Vector3& getMinCorner() const { return m_min_corner; }
        const Vector3& getMaxCorner() const { return m_max_corner; }

    private:
        Vector3 m_center{ Vector3::ZERO };
        Vector3 m_half_extent{ Vector3::ZERO };

        Vector3 m_min_corner{
            std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
        Vector3 m_max_corner{
            -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max() };
    };
}