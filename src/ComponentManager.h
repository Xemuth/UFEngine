#ifndef _UFEngine_ComponentManager_h_
#define _UFEngine_ComponentManager_h_

#include "Component.h"

namespace Upp{

class ComponentManager{
	public:
		template <class T, class... Args> T& Create(Args&&... args);
		template <class T> T& Copy(T& c);
		
		template <class T> bool Has()const;
		
		template <class T> int Find(int start = 0)const;
		
		template <class T> T& Get();
		template <class T> const T& Get()const;
		
		Component& Get(int position){
			if( position < 0 || position >= components.GetCount()) throw Exc("ComponentManager exception : Position where to get component is incorrect (" + AsString(position) + ")");
			return components[position];
		}
		const Component& Get(int position)const{
			if( position < 0 || position >= components.GetCount()) throw Exc("ComponentManager exception : Position where to get component is incorrect (" + AsString(position) + ")");
			return components[position];
		}
		
		template <class T> VectorMap<int, T*> GetAll();
		template <class T> const VectorMap<int, T*> GetAll()const;
		
		template <class T> int GetPosition(int start = 0)const;
		template <class T> bool Remove();
		template <class T, class M> bool Swap();
		template <class T> bool Swap(int newPosition);
		
		bool Swap(int oldPosition, int newPosition){
			if(oldPosition >= 0 && oldPosition < components.GetCount()){
				if(newPosition >= 0 && newPosition < components.GetCount()){
					components.Swap(oldPosition,newPosition);
					return true;
				}
			}
			return false;
		}

		void  SendMessageBroadcast(const String& message, const Value& args = Value()){
			for(Component& component : components){
				component.ReceiveMessage(message,args);
			}
		}
		
		template <class T> Value SendMessage(const String& message, const Value& args = Value());
		Value SendMessage(int componentPosition, const String& message, const Value& args = Value()){
			if(componentPosition >= 0 && componentPosition < components.GetCount()){
				return components[componentPosition].ReceiveMessage(message, args);
			}
			return Value();
		}

		int GetCount()const{
			return components.GetCount();
		}
		
		Array<Component>& GetComponents(){
			return components;
		}
		const Array<Component>& GetComponents()const{
			return components;
		}
		
	protected:
		friend class Object;
		ComponentManager(Object& _object) : object(_object) {}
		ComponentManager& operator=(const ComponentManager& cm);
		
		~ComponentManager(){components.Clear();}
	private:
		Object& object;
	//	Object& GetObject(){ASSERT_(object,"Object ptr in ComponentManager is null"); return *object;}
		Array<Component> components;
};




}



#endif
