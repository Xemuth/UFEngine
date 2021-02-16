#ifndef _UFEngine_Transform_h_
#define _UFEngine_Transform_h_

#include <Core/Core.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <plugin/glm/glm.hpp>
#include <plugin/glm/gtc/matrix_transform.hpp>
#include <plugin/glm/gtc/type_ptr.hpp>
#include <plugin/glm/gtx/quaternion.hpp>
#include <plugin/glm/ext/quaternion_trigonometric.hpp>
#include <plugin/glm/gtc/constants.hpp>
#include <plugin/glm/gtx/norm.hpp>
#include <plugin/glm/gtx/compatibility.hpp>
#include <plugin/glm/gtx/string_cast.hpp>
#include <plugin/glm/gtx/vector_angle.hpp>

namespace Upp{

class Transform{
	public:
		glm::vec3 GetPosition()const;
		glm::quat GetRotation()const;
		glm::vec3 GetScale()const;
	
		Transform& SetPosition(const glm::vec3& vec3);
		Transform& SetPosition(float x, float y, float z);
		
		Transform& SetRotation(const glm::quat& quat);
		Transform& SetRotation(float Yaw, float Pitch, float Roll);
		Transform& SetRotation(float angleDegree, const glm::vec3& axis);
		
		Transform& Move(const glm::vec3& vec3);
		Transform& Move(float x, float y, float z);
		
		Transform& Rotate(const glm::quat& quat);
		Transform& Rotate(float Yaw, float Pitch, float Roll);
		Transform& Rotate(float angleDegree, const glm::vec3& axis);
		
		Transform& SetScale(const glm::vec3& vec3);
		Transform& LookAt(const glm::vec3& LookAt, glm::vec3 customUp = glm::vec3(0.0f,0.0f,0.0f));
		
		glm::mat4 GetViewMatrix()const;
		glm::mat4 GetModelMatrix()const;
		
		static glm::quat GetQuaterion(float Yaw, float Pitch, float Roll);
		static glm::quat GetQuaterion(float angleDegree, const glm::vec3& axis);
		static glm::vec3 GetEulerAngle(const glm::quat& quaterion);
		static glm::vec3 TransformVectorByMatrix(const glm::vec3& vector,const glm::mat4& matrix);
		
		Transform& SetFront(const glm::vec3& vec3);
		Transform& SetUp(const glm::vec3& vec3);
		Transform& SetRight(const glm::vec3& vec3);
		Transform& SetWorldUp(const glm::vec3& vec3);
		Transform& SetWorldFront(const glm::vec3& vec3);
		Transform& SetWorldRight(const glm::vec3& vec3);
		
		glm::vec3 GetFront()const;
		glm::vec3 GetUp()const;
		glm::vec3 GetRight()const;
		glm::vec3 GetWorldUp()const;
		glm::vec3 GetWorldFront()const;
		glm::vec3 GetWorldRight()const;
		
		String ToString()const;
		
		Transform();
		Transform(const Transform& _transform);
		Transform& operator=(const Transform& _transform);
		~Transform();
	private:
		glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);
		
		glm::vec3 WorldFront = glm::vec3(0.0, 0.0, -1.0);
		glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 WorldRight = glm::vec3(1.0f, 0.0f, 0.0f);
		
		glm::vec3 Position = glm::vec3(0.0f);
		glm::quat Rotation = Transform::GetQuaterion(0.0f,glm::vec3(0.0f,0.0f,0.0f));
		glm::vec3 Scale = glm::vec3(1.0f);
		
		void RecalculateFrontUpRight();
};

}

#endif
