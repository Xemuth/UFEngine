#include "Object.h"
#include "Scene.h"
#include "UFEContext.h"
#include "ComponentManager.h"

namespace Upp{

Object::Object(Scene& _scene) : scene(_scene), componentManager(*this) {}

Object::Object(Scene& _scene, const Object& object) : scene(_scene), componentManager(*this){
	componentManager = object.componentManager; //Can't write clone here because of compiler but it's do a pur copy
	transform = clone(object.transform);
}

Object::~Object(){
	componentManager.~ComponentManager();
}

String Object::GetName()const{
	return name;
}

Object& Object::SetName(const String& _name){
	name = _name;
	return *this;
}

bool Object::IsActive()const{
	return active;
}

Object& Object::SetActive(bool b){
	active = b;
	return *this;
}

Transform& Object::GetTransform(){
	return transform;
}

const Transform& Object::GetTransform()const{
	return transform;
}

Scene& Object::GetScene(){
	return scene;
}

const Scene& Object::GetScene()const{
	return scene;
}

ComponentManager& Object::GetComponentManager(){
	return componentManager;
}
const ComponentManager& Object::GetComponentManager()const{
	return componentManager;
}

void Object::Update(double timeEllapsed, double deltaTime){
	for(Component& component : componentManager.GetComponents()){
		component.BeforeUpdate();
		component.Update(timeEllapsed, deltaTime);
		component.AfterUpdate();
	}
}

String Object::ToString()const{
	String object;
	object << "Name: " << name << "\nScene: " << scene.GetName() << "\nContext ID: " << scene.GetContext().GetID() << "\nActive: " << active <<"\n---------------------\n";
	object << "Transform :\n\t" << Replace(transform.ToString(), VectorMap<String,String>{{"\n","\n\t"}});
	object << "---------------------\n";
	object << "Components :\n";
	for(const Component& component : componentManager.GetComponents()){
		object << "\t-" << component.GetName() << ":\n\t\t" << Replace(component.ToString(), VectorMap<String,String>{{"\n","\n\t\t"}});
		object << "---------------------\n";
	}
	return object;
}

}