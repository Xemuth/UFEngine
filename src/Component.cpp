#include "Component.h"
#include "Object.h"

namespace Upp{
	
Component::Component(){

}
Component::~Component(){

}
		
Object& Component::GetObject(){
	if(object)
		return *object;
	throw Exc("Component exception : Object is nullptr !");
}

const Object& Component::GetObject()const{
	if(object)
		return *object;
	throw Exc("Component exception : Object is nullptr !");
}

void Component::OnCreation(){

}

void Component::OnDestruction(){

}

void Component::BeforeUpdate(){
	
}

void Component::Update(double timeEllapsed, double deltaTime){

}

void Component::AfterUpdate(){
	
}

Component& Component::SetObject(Object& _object){
	object = &_object;
	return *this;
}

String Component::GetName()const{
	return "DefaultComponent";
}

Value Component::ReceiveMessage(const String& message,const Value& args){
	return Value();
}

String Component::ToString()const{
	String component;
	component << "Object name: " << object->GetName() << "\n";
	component << "No more data to retrieve, ToString function have not been overloaded\n";
	return component;
}

}