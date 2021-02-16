#ifndef _UFEngine_UFEContext_h_
#define _UFEngine_UFEContext_h_

#include <Core/Core.h>
#include <chrono>
#include <ctime>

#include "ServicesManager.h"
#include "SceneManager.h"

namespace Upp{

class UFEContext{
	public:
		UFEContext();
		~UFEContext();
		int GetID()const;
	
		void TimerStart();
		void TimerStop();
		void TimerReset();
		void TimerUpdateByHand(double timeStepInMS);
		double GetTimeEllapsed();
		double GetDeltaTime();
		
		
		
		SceneManager& GetSceneManager();
		const SceneManager& GetSceneManager()const;
		
		ServiceManager& GetServiceManager();
		const ServiceManager& GetServiceManager()const;
		
		void Update();
		
		String ToString()const;
		
	private:
		int ID;
		
		std::chrono::time_point<std::chrono::high_resolution_clock> timerBegin;
		std::chrono::time_point<std::chrono::high_resolution_clock> timerEnd;
		bool started = false;
		double timer = 0;
		double lastFrameTimer = 0;
		double deltaTime = 0;
		
		ServiceManager serviceManager;
		SceneManager sceneManager;
		

	    static int InstanceCpt;
};

}

#endif
