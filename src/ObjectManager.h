#ifndef _UFEngine_ObjectManager_h_
#define _UFEngine_ObjectManager_h_

#include "Object.h"

namespace Upp{

class ObjectManager{
	public:
		Object& Create(const String& objectName, bool active = true){
			if(FindObjectByName(objectName) == -1){
				Object& object = objects.Create(scene);
				object.SetName(objectName);
				object.SetActive(active);
				return object;
			}
			throw Exc("ObjectManager exception : Object named " + objectName +" already exist !");
		}
		
		Object& Copy(const Object& object,String newName = "", short active = -5){
			if(newName.IsEmpty()) newName = object.GetName();
			if(FindObjectByName(newName) == -1){
				Object& freshObject = objects.Create(scene,object);
				freshObject.SetName(newName);
				if(active != -5) freshObject.SetActive((bool)active);
				return freshObject;
			}
			throw Exc("ObjectManager exception : Object named " + newName +" already exist !");
		}
		
		Object& Get(const String& objectName){
			int position = FindObjectByName(objectName);
			if(position != -1){
				return objects[position];
			}
			throw Exc("ObjectManager exception : Object named " + objectName +" can't be found !");
		}
		const Object& Get(const String& objectName)const{
			int position = FindObjectByName(objectName);
			if(position != -1){
				return objects[position];
			}
			throw Exc("ObjectManager exception : Object named " + objectName +" can't be found !");
		}
		
		Object& Get(int position){
			if(position  >= 0 && position < objects.GetCount()){
				return objects[position];
			}
			throw Exc("ObjectManager exception : Can't retrieve object by position, position "+ AsString(position) +" don't exist");
		}
		const Object& Get(int position)const{
			if(position  >= 0 && position < objects.GetCount()){
				return objects[position];
			}
			throw Exc("ObjectManager exception : Can't retrieve object by position, position "+ AsString(position) +" don't exist");
		}
		
		bool Has(const String& objectName)const{
			return (FindObjectByName(objectName) != -1);
		}
		
		bool Remove(const String& objectName){
			int position = FindObjectByName(objectName);
			if(position != -1){
				objects.Remove(position);
				return true;
			}
			return false;
		}
		
		int GetCount()const{
			return objects.GetCount();
		}

		Array<Object>& GetObjects(){
			return objects;
		}
		
		const Array<Object>& GetObjects()const{
			return objects;
		}
		
		Vector<Object*> GetObjectsDependingOnFunction(bool (*lambda)(Object& object, Scene& scene)){
			Vector<Object*> allObjectCorresponding;
			for(Object& object : objects){
				if(lambda(object,scene))
					allObjectCorresponding.Add(&object);
			}
			return allObjectCorresponding;
		}
		
	protected:
		friend class Scene;
		ObjectManager(Scene& _scene) : scene(_scene){}
		~ObjectManager(){}
		
	private:
		Scene& scene;
		Array<Object> objects;

		int FindObjectByName(const String& objectName)const{
			for(int i = 0; i < objects.GetCount(); i++){
				if(objects[i].GetName().IsEqual(objectName))
					return i;
			}
			return -1;
		}
};

}

#endif
