#ifndef _UFEngine_Component_h_
#define _UFEngine_Component_h_
#include <Core/Core.h>

namespace Upp{
	
class Object;
class Scene;
class UFEContext;

enum class InstantiationConstraint { NONE, ONE_BY_OBJECT, ONE_BY_SCENE };

#define CLONE_COMPONENT(ClassName) virtual ClassName* Clone(Object& object) const{\
										ClassName* ptr = new ClassName(*this);\
										ptr->SetObject(object);\
										return ptr;\
									}\

class Component{
	public:
		static const InstantiationConstraint Constraint = InstantiationConstraint::NONE;
		virtual InstantiationConstraint GetConstraint()const{return Constraint;}
		
		Component();
		
		virtual ~Component();
		
		Object& GetObject();
		const Object& GetObject()const;
		virtual String GetName()const = 0;
		
		virtual void OnCreation();
		virtual void OnDestruction();
		
		virtual void BeforeUpdate();
		virtual void Update(double timeEllapsed, double deltaTime = 0.0);
		virtual void AfterUpdate();
		
		virtual Value ReceiveMessage(const String& message,const Value& args = Value());
		virtual String ToString()const;
		
	protected:
		friend class ComponentManager;
		Component& SetObject(Object& _object);
		
		virtual Component* Clone(Object& object)const = 0; //Clone is for deep copy of a component
	private:
		Object* object = nullptr;
};
	
}
#endif
