#ifndef _UFEngine_ComponentManagerEx_h_
#define _UFEngine_ComponentManagerEx_h_

#include "Scene.h"

namespace Upp{

template <class T, class... Args> T& ComponentManager::Create(Args&&... args){
	const std::type_info& type = typeid(T);
	InstantiationConstraint IC = T::Constraint;
	if(IC == InstantiationConstraint::ONE_BY_OBJECT || IC == InstantiationConstraint::ONE_BY_SCENE){
		//Then we check in object if we already have it or inherrited
		if(Has<T>()) throw Exc("ComponentManager exception : Can't create object of type '" + String(type.name()) +"' with constraint of type "+ ((IC == InstantiationConstraint::ONE_BY_OBJECT)? "'One by object'" : "'One by scene'") +" because one already exist in this object");
	}
	if(IC == InstantiationConstraint::ONE_BY_SCENE){
		//Then we check in other objects if it dont already exist
		for(int i = 0; i <  object.GetScene().GetObjectManager().GetCount(); i++){
			Object& obj = object.GetScene().GetObjectManager().GetObjects()[i];
			if(&obj != &object){
				if(obj.GetComponentManager().Has<T>()){
					throw Exc("ComponentManager exception : Can't create object of type '" + String(type.name()) +"' with constraint of type "+ ((IC == InstantiationConstraint::ONE_BY_OBJECT)? "'One by object'" : "'One by scene'") +" because one already exist in this scene (Look at object number "+ AsString(i) +" which is name is '" + obj.GetName() + "')");
				}
			}
		}
	}
	try{
		T& freshComponent = components.Create<T>(std::forward<Args>(args)...);
		freshComponent.SetObject(object);
		freshComponent.OnCreation();
		return dynamic_cast<T&>(freshComponent);
	}catch(Exc& exc){
		throw exc;
	}
}
template <class T> T& ComponentManager::Copy(T& c){
	const std::type_info& type = typeid(T);
	InstantiationConstraint IC = T::Constraint;
	if(IC == InstantiationConstraint::ONE_BY_OBJECT || IC == InstantiationConstraint::ONE_BY_SCENE){
		//Then we check in object if we already have it or inherrited
		if(Has<T>()) throw Exc("ComponentManager exception : Can't copy object of type '" + String(type.name()) +"' with constraint of type "+ ((IC == InstantiationConstraint::ONE_BY_OBJECT)? "'One by object'" : "'One by scene'") +" because one already exist in this object");
	}
	if(IC == InstantiationConstraint::ONE_BY_SCENE){
		//Then we check in other objects if it dont already exist
		for(int i = 0; i <  object.GetScene().GetObjectManager().GetCount(); i++){
			Object& obj = object.GetScene().GetObjectManager().GetObjects()[i];
			if(&obj != &object){
				if(obj.GetComponentManager().Has<T>()){
					throw Exc("ComponentManager exception : Can't copy object of type '" + String(type.name()) +"' with constraint of type "+ ((IC == InstantiationConstraint::ONE_BY_OBJECT)? "'One by object'" : "'One by scene'") +" because one already exist in this scene (Look at object number "+ AsString(i) +" which is name is '" + obj.GetName() + "')");
				}
			}
		}
	}
	try{
		T& freshComponent = components.Create<T>(c);
		freshComponent.SetObject(object);
		freshComponent.OnCreation();
		return dynamic_cast<T&>(freshComponent);
	}catch(Exc& exc){
		throw exc;
	}
}


template <class T> bool ComponentManager::Has()const{
	for(const Component& component : components){
		if(dynamic_cast<const T*>(&component))
			return true;
	}
	return false;
}

template <class T> int ComponentManager::Find(int start)const{
	if( start < 0 || start >= components.GetCount()) return -1;
	for(int i = start; i < components.GetCount(); i++){
		if(dynamic_cast<T*>(&(components[i]))){
			return i;
		}
	}
	return -1;
}

template <class T> T& ComponentManager::Get(){
	for(Component& component : components){
		T* testPtr = dynamic_cast<T*>(&component);
		if(testPtr){
			return *testPtr;
		}
	}
	throw Exc("ComponentManager exception : no component of type " + String(typeid(T).name()) + " exist in this object");
}

template <class T> const T& ComponentManager::Get()const{
	for(const Component& component : components){
		T* testPtr = dynamic_cast<T*>(&component);
		if(testPtr){
			return *testPtr;
		}
	}
	throw Exc("ComponentManager exception : no component of type " + String(typeid(T).name()) + " exist in this object");
}

template <class T> VectorMap<int, T*> ComponentManager::GetAll(){
	if(!Has<T>()) return VectorMap<int, T*>{};
	VectorMap<int, T*> allComponents;
	for(int i = 0; i < components.GetCount(); i++){
		T* testPtr = dynamic_cast<T*>(&(components[i]));
		if(testPtr){
			allComponents.Add(i, testPtr);
		}
	}
	return allComponents;
}
template <class T> const VectorMap<int, T*> ComponentManager::GetAll()const{
	if(!Has<T>()) return VectorMap<int, T*>{};
	VectorMap<int, T*> allComponents;
	for(int i = 0; i < components.GetCount(); i++){
		T* testPtr = dynamic_cast<T*>(&(components[i]));
		if(testPtr){
			allComponents.Add(i, testPtr);
		}
	}
	return allComponents;
}


template <class T> int ComponentManager::GetPosition(int start)const{
	if( start < 0 || start >= components.GetCount()) return -1;
	const std::type_info& type = typeid(T);
	for(int i = start; i < components.GetCount(); i++){
		const Component& component = components[i];
		if(typeid(component) == type){
			return i;
		}
	}
	return -1;
}

template <class T> bool ComponentManager::Remove(){
	const std::type_info& type = typeid(T);
	for(int i = 0; i < components.GetCount(); i++){
		Component& component = components[i];
		if(typeid(component) == type){
			components.Remove(i);
			return true;
		}
	}
	return false;
}

template <class T, class M> bool ComponentManager::Swap(){
	const std::type_info& type = typeid(T);
	const std::type_info& type2 = typeid(M);
	
	for(int i = 0; i < components.GetCount(); i++){
		Component& component = components[i];
		if(typeid(component) == type){
			int iterator = 0;
			if(type == type2) iterator = i +1;
			for(int m = iterator; m < components.GetCount(); m++){
				Component& component2 = components[i];
				if(typeid(component2) == type2){
					components.Swap(i,m);
					return true;
				}
			}
			break;
		}
	}
	return false;
}

template <class T> bool ComponentManager::Swap(int newPosition){
	const std::type_info& type = typeid(T);
	if(newPosition >= 0 && newPosition < components.GetCount()){
		for(int i = 0; i < components.GetCount(); i++){
			Component& component = components[i];
			if(typeid(component) == type){
				components.Swap(i,newPosition);
				return true;
			}
		}
	}
	return false;
}
		
template <class T> Value ComponentManager::SendMessage(const String& message, const Value& args){
	for(Component& component : components){
		if(dynamic_cast<T*>(&component)){
			return component.ReceiveMessage(message,args);
		}
	}
	return Value();
}
	
}


#endif
