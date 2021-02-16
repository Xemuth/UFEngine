#include "UFEContext.h"
#include "Scene.h"

namespace Upp{

int UFEContext::InstanceCpt = 0;

UFEContext::UFEContext() : sceneManager(*this), serviceManager(*this){
	ID = InstanceCpt;
	UFEContext::InstanceCpt++;
}
UFEContext::~UFEContext(){}

int UFEContext::GetID()const{
	return ID;
}

ServiceManager& UFEContext::GetServiceManager(){
	return serviceManager;
}
const ServiceManager& UFEContext::GetServiceManager()const{
	return serviceManager;
}
SceneManager& UFEContext::GetSceneManager(){
	return sceneManager;
}
const SceneManager& UFEContext::GetSceneManager()const{
	return sceneManager;
}

void UFEContext::TimerStart(){
	started = true;
	timerBegin = std::chrono::high_resolution_clock::now();
}

void UFEContext::TimerStop(){
	started = false;
}

void UFEContext::TimerReset(){
	timerBegin = std::chrono::high_resolution_clock::now();
	timer = 0;
	deltaTime = 0;
}

void UFEContext::TimerUpdateByHand(double timeStepInMS){
	timer += (timeStepInMS / 1000.0);
}

double UFEContext::GetTimeEllapsed(){
	if(started){
		timerEnd = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> difference = timerEnd-timerBegin;
		timerBegin = std::chrono::high_resolution_clock::now();
		timer += difference.count();
		deltaTime = timer - lastFrameTimer;
		lastFrameTimer = timer;
	}
	return timer;
}

double UFEContext::GetDeltaTime(){
	return deltaTime;
}

void UFEContext::Update(){
	for(Service& service : GetServiceManager().GetServices()){
		service.BeforeUpdate();
	}
	GetSceneManager().GetActive().Update(GetTimeEllapsed(), deltaTime);
	for(Service& service : GetServiceManager().GetServices()){
		service.AfterUpdate();
	}
}

String UFEContext::ToString()const{
	String context;
	context << "UFEContext:\nID: " << ID <<"\n---------------------\n";
	context << "Services:\n";
	for(const Service& service : serviceManager.GetServices()){
		context << "\t-" << service.GetName() <<":\n\t\t" << Replace(service.ToString(), VectorMap<String,String>{{"\n","\n\t\t"}});
	}
	context <<"\n---------------------\n";
	
	context << "Scenes:\n";
	for(const Scene& scene : sceneManager.GetScenes()){
		context << "\t-" << scene.GetName() << ":\n\t\t" << Replace(scene.ToString(), VectorMap<String,String>{{"\n","\n\t\t"}});
		context << "---------------------\n";
	}
	context << "---------------------\n";
	return context;
}

}