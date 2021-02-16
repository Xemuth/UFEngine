#include "Scene.h"
#include "UFEContext.h"
namespace Upp{

Scene::Scene(UFEContext& _context, const String& _name) : context(_context), name(_name), objectManager(*this){

}

Scene::Scene(Scene&& scene) : context(scene.context), name(scene.name), objectManager(*this){
	scene.name ="";
}

Scene::~Scene(){
	//objectManager.~ObjectManager();
}

String Scene::GetName()const{
	return name;
}

Scene& Scene::SetActive(){
	context.GetSceneManager().SetActive(*this);
	return *this;
}

UFEContext& Scene::GetContext(){
	return context;
}

const UFEContext& Scene::GetContext()const{
	return context;
}

ObjectManager& Scene::GetObjectManager(){
	return objectManager;
}
const ObjectManager& Scene::GetObjectManager()const{
	return objectManager;
}

void Scene::Update(double timeEllapsed, double deltaTime){
	for(Object& object : GetObjectManager().GetObjects()){
		if(object.IsActive())
			object.Update(timeEllapsed,deltaTime);
	}
}

String Scene::ToString()const{
	String scene;
	scene << "name: " << name << "\ncontext ID: " << context.GetID() << "\n---------------------\n";
	scene << "Objects :\n";
	for(const Object& object : objectManager.GetObjects()){
		scene << "\t-" << object.GetName() << "\n\t\t" << Replace(object.ToString(), VectorMap<String,String>{{"\n","\n\t\t"}});
		scene << "---------------------\n";
	}
	return scene;
}

}