#ifndef _UFEngine_Scene_h_
#define _UFEngine_Scene_h_
#include <Core/Core.h>

#include "ObjectManager.h"

namespace Upp{
	
class UFEContext;

class Scene : public Moveable<Scene>{
	public:
		Scene(Scene&& scene);
		~Scene();
		
		String GetName()const;
		
		UFEContext& GetContext();
		const UFEContext& GetContext()const;
		
		ObjectManager& GetObjectManager();
		const ObjectManager& GetObjectManager()const;
		
		void Update(double timeEllapsed, double deltaTime = 0.0);
		
		Scene& SetActive();
		
		String ToString()const;
	private:
		friend class SceneManager;
		Scene(UFEContext& context, const String& _name);
		
		
		String name;
		UFEContext& context;

		ObjectManager objectManager;
};

}

#endif
