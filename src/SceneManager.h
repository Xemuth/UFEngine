#ifndef _UFEngine_SceneManager_h_
#define _UFEngine_SceneManager_h_

#include "Scene.h"

namespace Upp{

class UFEContext;

class SceneManager{
	public:
		Scene& Create(const String& name){
			for(Scene& scene : scenes){
				if(scene.GetName().IsEqual(name))
					throw Exc("SceneManager exception : a scene named '"+ name +"' already exist !");
			}
			if(GetActiveSceneIndice() == -1)  SetActiveSceneIndice(0);
			return GetScenes().Create(pick(Scene(context, name)));
		}
		
		bool Has(const String& name)const{
			for(const Scene& scene : scenes){
				if(scene.GetName().IsEqual(name))
					return true;
			}
			return false;
		}
		
		Scene& Get(const String& name){
			for(Scene& scene : scenes){
				if(scene.GetName().IsEqual(name))
					return scene;
			}
			throw Exc("SceneManager exception : Can't retrieve scene named '"+ name +"'");
		}
		const Scene& Get(const String& name)const{
			for(const Scene& scene : scenes){
				if(scene.GetName().IsEqual(name))
					return scene;
			}
			throw Exc("SceneManager exception : Can't retrieve scene named '"+ name +"'");
		}

		Scene& Get(int position){
			if(position >= 0 && position < scenes.GetCount())
				return scenes[position];
			throw Exc("SceneManager exception : Can't retrieve scene at position " + AsString(position));
		}
		const Scene& Get(int position)const{
			if(position >= 0 && position < scenes.GetCount())
				return scenes[position];
			throw Exc("SceneManager exception : Can't retrieve scene at position " + AsString(position));
		}
		
		Scene& GetActive(){
			if(activeScene >= 0)
				return scenes[activeScene];
			throw Exc("SceneManager exception : Can't retrieve active scene");
		}
		const Scene& GetActive()const{
			if(activeScene >= 0)
				return scenes[activeScene];
			throw Exc("SceneManager exception : Can't retrieve active scene");
		}
		
		bool SetActive(const Scene& scene){
			for(int i = 0; i < scenes.GetCount(); i++){
				if(&(scenes[i]) == &scene){
					activeScene = i;
					return true;
				}
			}
			return false;
		}
		
		bool SetActive(const String& name){
			int position = GetPosition(name);
			if(position != -1){
				activeScene = position;
				return true;
			}
			return false;
		}
		
		bool SetActive(int position){
			if(position >= 0 && position < scenes.GetCount()){
				activeScene = position;
				return true;
			}
			return false;
		}
		
		int GetPosition(const String& name){
			for(int i = 0; i < scenes.GetCount(); i++){
				if(scenes[i].GetName().IsEqual(name)){
					return i;
				}
			}
			return -1;
		}
		
		bool Remove(const Scene& scene){
			for(int i = 0; i < scenes.GetCount(); i++){
				if(&(scenes[i]) == &scene){
					if(activeScene == i){
						if((i - 1) >=0) activeScene--;
						else if((i +1) < scenes.GetCount()) activeScene++;
						else activeScene = -1;
					}
					scenes.Remove(i);
					activeScene--;
					return true;
				}
			}
			return false;
		}
		
		int GetCount()const{
			return scenes.GetCount();
		}
		
		Array<Scene>& GetScenes(){
			return scenes;
		}
		
		const Array<Scene>& GetScenes()const{
			return scenes;
		}
		
	protected:
		friend class UFEContext;
		SceneManager(UFEContext& _context) : context(_context){}
		
		int GetActiveSceneIndice(){return activeScene;}
		void SetActiveSceneIndice(int i){activeScene = i;}
	private:
		UFEContext& context;
		Array<Scene> scenes;
		int activeScene = -1;
};

}


#endif
