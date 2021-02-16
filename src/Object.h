#ifndef _UFEngine_Object_h_
#define _UFEngine_Object_h_

#include <Core/Core.h>
#include "Transform.h"

#include "ComponentManager.h"


namespace Upp{

class Scene;

class Object{
	public:
		~Object();
		
		String GetName()const;
		Object& SetName(const String& _name);
		
		bool IsActive()const;
		Object& SetActive(bool b = true);
		
		Transform& GetTransform();
		const Transform& GetTransform()const;
		
		Scene& GetScene();
		const Scene& GetScene()const;
		
		void Update(double timeEllapsed, double deltaTime = 0.0);

		ComponentManager& GetComponentManager();
		const ComponentManager& GetComponentManager()const;

		String ToString()const;
	private:
		friend class ObjectManager;
		friend class Array<Object>;
		Object(Scene& _scene);
		Object(Scene& _scene, const Object& object);

		ComponentManager componentManager;
	
		bool active = true;
		String name;
		Transform transform;
		Scene& scene;
};




}



#endif
