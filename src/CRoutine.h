#ifndef _UFEngine_CRoutine_h_
#define _UFEngine_CRoutine_h_
#include "Component.h"
namespace Upp{
	
class CRoutine : public Component{
	public:
		CRoutine(){}
		CRoutine& operator=(const CRoutine& routine){
			beforeUpdate = routine.beforeUpdate;
			update = routine.update;
			afterUpdate = routine.afterUpdate;
			return *this;
		}
		
		Event<Component&> beforeUpdate;
		Event<Component&, double, double> update;
		Event<Component&> afterUpdate;
		
		CRoutine& SetEventBeforeUpdate(Event<Component&>& event){beforeUpdate = event; return *this;}
		CRoutine& SetEventUpdate(Event<Component&, double, double>& event){update = event; return *this;}
		CRoutine& SetEventAfterUpdate(Event<Component&>& event){afterUpdate = event; return *this;}
		
		virtual String GetName()const{
			return "CRoutine";
		}
	
		virtual void BeforeUpdate(){beforeUpdate(*this);}
		virtual void Update(double timeEllapsed, double deltaTime = 0.0){update(*this, timeEllapsed, deltaTime);}
		virtual void AfterUpdate(){afterUpdate(*this);}

		virtual String ToString()const{
			String toString;
			toString << "Object name: " << GetObject().GetName() << "\n";
			bool b1 = beforeUpdate;
			bool b2 = update;
			bool b3 = afterUpdate;
			toString << "Before update event set : " <<  AsString(b1) << "\n";
			toString << "update event set : " <<  AsString(b2) << "\n";
			toString << "After update event set : " <<  AsString(b3) << "\n";
			return toString;
		}
	protected:
		virtual CRoutine* Clone(Object& object) const{
			CRoutine* ptr = new CRoutine(*this);
			ptr->SetObject(object);
			return ptr;
		}
		CRoutine(const CRoutine& routine){
			*this = routine;
		}
};

}
#endif
