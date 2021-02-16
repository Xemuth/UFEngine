#include "ComponentManager.h"

#include "UFEContext.h"

namespace Upp{
/*
	Pur clone is an expensive way of working. Must be use with parsimony
*/
ComponentManager& ComponentManager::operator=(const ComponentManager& cm){
	for(const Component& c : cm.components){
		InstantiationConstraint IC = c.GetConstraint();
		if(IC == InstantiationConstraint::ONE_BY_SCENE){
			for(int i = 0; i <  object.GetScene().GetObjectManager().GetCount(); i++){
				Object& obj = object.GetScene().GetObjectManager().GetObjects()[i];
				if(&obj != &object){
					bool found = false;
					for(const Component& cObj :  obj.GetComponentManager().GetComponents()){
						if(typeid(c) == typeid(cObj)){
							String constrainteName = ((IC == InstantiationConstraint::ONE_BY_OBJECT) ? "'One by object'" : "'One by scene'");
							String typeName = typeid(c).name();
							LOG("ComponentManager exception : Can't copy component of type '"+ typeName + "'with constraint of type "+ constrainteName +" because one already exist in this scene (Look at object number "+ AsString(i) +" which is name is '" + obj.GetName() + "')");
							found = true;
							break;
						}
					}
					if(found) continue;
				}
			}
		}
		components.Add(pick(c.Clone(object)));
	}
	return *this;
}

}