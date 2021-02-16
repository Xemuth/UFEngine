#include "Transform.h"

namespace Upp{

glm::vec3 Transform::GetPosition()const{
	return Position;
}

glm::quat Transform::GetRotation()const{
	return Rotation;
}

glm::vec3 Transform::GetScale()const{
	return Scale;
}

Transform& Transform::SetPosition(const glm::vec3& vec3){
	Position = vec3;
	return *this;
}

Transform& Transform::SetPosition(float x, float y, float z){
	Position.x = x;
	Position.y = y;
	Position.z = z;
	return *this;
}

Transform& Transform::SetRotation(const glm::quat& quat){
	Rotation = quat;
	RecalculateFrontUpRight();
	return *this;
}

Transform& Transform::SetRotation(float Yaw, float Pitch, float Roll){
	Rotation = glm::quat(glm::vec3(Pitch, Yaw , Roll));
	RecalculateFrontUpRight();
	return *this;
}

Transform& Transform::SetRotation(float angleDegree, const glm::vec3& axis){
	float radHalfAngle =(float) glm::radians(angleDegree) / 2.0f;
	float sinVal = glm::sin(radHalfAngle);
	float cosVal = glm::cos(radHalfAngle);
	float xVal = axis.x * sinVal;
	float yVal = axis.y * sinVal;
	float zVal = axis.z * sinVal;
	Rotation = glm::quat(cosVal,xVal, yVal, zVal);
	RecalculateFrontUpRight();
	return *this;
}

Transform& Transform::Move(const glm::vec3& vec3){
	Position += vec3;
	return *this;
}

Transform& Transform::Move(float x, float y, float z){
	Position += glm::vec3(x,y,z);
	return *this;
}

Transform& Transform::Rotate(const glm::quat& quat){
	Rotation *= quat;
	RecalculateFrontUpRight();
	return *this;
}

Transform& Transform::Rotate(float Yaw, float Pitch, float Roll){
	Rotation *= glm::quat(glm::vec3(Pitch, Yaw , Roll));
	RecalculateFrontUpRight();
	return *this;
}

Transform& Transform::Rotate(float angleDegree, const glm::vec3& axis){
	float radHalfAngle =(float) glm::radians(angleDegree) / 2.0f;
	float sinVal = glm::sin(radHalfAngle);
	float cosVal = glm::cos(radHalfAngle);
	float xVal = axis.x * sinVal;
	float yVal = axis.y * sinVal;
	float zVal = axis.z * sinVal;
	Rotation *= glm::quat(cosVal,xVal, yVal, zVal);
	RecalculateFrontUpRight();
	return *this;
}

Transform& Transform::SetScale(const glm::vec3& vec3){
	Scale = vec3;
	return *this;
}

Transform& Transform::LookAt(const glm::vec3& LookAt, glm::vec3 customUp){
	glm::vec3 direction = LookAt - Position;
	float directionLength = glm::length(direction);
	if(directionLength > 0.0001){
		direction /= directionLength;
		if(customUp != glm::vec3(0.0f,0.0f,0.0f))
			SetRotation(glm::quatLookAt(glm::normalize(direction), customUp));
		else
			SetRotation(glm::quatLookAt(glm::normalize(direction), Up));
	}
	return *this;
}

glm::mat4 Transform::GetViewMatrix()const{
	return glm::lookAt(Position,Position + Front, Up);
}

glm::mat4 Transform::GetModelMatrix()const{
	glm::mat4 ModelMatrix = glm::mat4(1.0f);
	ModelMatrix = glm::translate(ModelMatrix, Position); //position of cube
    ModelMatrix = glm::scale(ModelMatrix, Scale); //Scale (not used so glm::vec3(1.0f))
	ModelMatrix *= glm::mat4_cast(Rotation);
//	Cout() << String(glm::to_string(ModelMatrix)) << EOL;
	return ModelMatrix;
}

glm::quat Transform::GetQuaterion(float Yaw, float Pitch, float Roll){
	return glm::quat(glm::vec3(Pitch, Yaw , Roll));
}

glm::quat Transform::GetQuaterion(float angleDegree, const glm::vec3& axis){
	float radHalfAngle =(float) glm::radians(angleDegree) / 2.0f;
	float sinVal = glm::sin(radHalfAngle);
	float cosVal = glm::cos(radHalfAngle);
	float xVal = axis.x * sinVal;
	float yVal = axis.y * sinVal;
	float zVal = axis.z * sinVal;
	return glm::quat(cosVal,xVal, yVal, zVal);
}

glm::vec3 Transform::GetEulerAngle(const glm::quat& quaterion){
	return glm::eulerAngles(quaterion);
}
		
glm::vec3 Transform::TransformVectorByMatrix(const glm::vec3& vector,const glm::mat4& matrix){
	float w = vector[0]* matrix[0][3] + vector[1] * matrix[1][3] + vector[2] * matrix[2][3] + matrix[3][3];
	glm::vec3 ret;
	ret.x = (vector[0]*matrix[0][0]+vector[1]*matrix[1][0]+vector[2]*matrix[2][0]+matrix[3][0])/w;
	ret.y = (vector[0]*matrix[0][1]+vector[1]*matrix[1][1]+vector[2]*matrix[2][1]+matrix[3][1])/w;
	ret.z = (vector[0]*matrix[0][2]+vector[1]*matrix[1][2]+vector[2]*matrix[2][2]+matrix[3][2])/w;
	return ret;
}

glm::vec3 Transform::GetFront()const{
	return Front;
}

glm::vec3 Transform::GetUp()const{
	return Up;
}

glm::vec3 Transform::GetRight()const{
	return Right;
}

glm::vec3 Transform::GetWorldUp()const{
	return WorldUp;
}

glm::vec3 Transform::GetWorldFront()const{
	return WorldFront;
}

glm::vec3 Transform::GetWorldRight()const{
	return WorldRight;
}

Transform& Transform::SetFront(const glm::vec3& vec3){
	Front = vec3;
	return *this;
}

Transform& Transform::SetUp(const glm::vec3& vec3){
	Up = vec3;
	return *this;
}

Transform& Transform::SetRight(const glm::vec3& vec3){
	Right = vec3;
	return *this;
}

Transform& Transform::SetWorldUp(const glm::vec3& vec3){
	WorldUp = vec3;
	return *this;
}

Transform& Transform::SetWorldFront(const glm::vec3& vec3){
	WorldFront = vec3;
	return *this;
}

Transform& Transform::SetWorldRight(const glm::vec3& vec3){
	WorldRight = vec3;
	return *this;
}

Transform::Transform(){

}

Transform::Transform(const Transform& _transform){
	*this = _transform;
}

Transform& Transform::operator=(const Transform& _transform){
	Front = _transform.Front;
	Up = _transform.Up;
	Right = _transform.Right;
	WorldUp = _transform.WorldUp;
	WorldFront = _transform.WorldFront;
	WorldRight = _transform.WorldRight;
	Position = _transform.Position;
	Rotation = _transform.Rotation;
	Scale = _transform.Scale;
	return *this;
}

Transform::~Transform(){

}

void Transform::RecalculateFrontUpRight(){
	Front = glm::rotate(Rotation, WorldFront);
    Right = glm::rotate(Rotation, WorldRight);
    Up = glm::rotate(Rotation, WorldUp);
}

String Transform::ToString()const{
	String transform;
	
	transform << "Position: " << String(glm::to_string(Position)) << "\n";
	transform << "Rotation(quaternion): " << String(glm::to_string(Rotation)) << "\n";
	transform << "Scale: " << String(glm::to_string(Scale)) << "\n";
	transform << "Front: " << String(glm::to_string(Front)) << "\n";
	transform << "Up: " << String(glm::to_string(Up)) << "\n";
	transform << "Right: " << String(glm::to_string(Right)) << "\n";
	transform << "WorldFront: " << String(glm::to_string(WorldFront)) << "\n";
	transform << "WorldUp: " << String(glm::to_string(WorldUp)) << "\n";
	transform << "WorldRight: " << String(glm::to_string(WorldRight)) << "\n";
	
	return transform;
}


}
