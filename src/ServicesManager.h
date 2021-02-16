#ifndef _UFEngine_ServicesManager_h_
#define _UFEngine_ServicesManager_h_

#include "Service.h"

namespace Upp{
	
class UFEContext;

class ServiceManager{
	public:
		template <class T> T& Create(){
			const std::type_info& type = typeid(T);
			for(Service& service : services){
				if(typeid(service) == type){
					throw ("ServiceManager exception : A service of type " + String(type.name()) + " already exist in this program");
				}
			}
			T& freshService = services.Create<T>(context);
			freshService.OnCreation();
			return freshService;
		}
		
		template <class T> bool Has()const{
			const std::type_info& type = typeid(T);
			for(const Service& service : services){
				if(typeid(service) == type){
					return true;
				}
			}
			return false;
		}
		
		template <class T> T& Get(){
			const std::type_info& type = typeid(T);
			for(Service& service : services){
				if(typeid(service) == type){
					return dynamic_cast<T&>(service);
				}
			}
			throw Exc("ServiceManager exception : Can't find a service of type " + String(type.name()) + " in this program");
		}
		template <class T> const T& Get()const{
			const std::type_info& type = typeid(T);
			for(const Service& service : services){
				if(typeid(service) == type){
					return dynamic_cast<const T&>(service);
				}
			}
			throw Exc("ServiceManager exception : Can't find a service of type " + String(type.name()) + " in this program");
		}

		template <class T> bool Remove(){
			const std::type_info& type = typeid(T);
			for(int i = 0; i < services.GetCount(); i++){
				Service& s = services[i];
				if(typeid(s) == type){
					services[i].OnDestruction();
					services.Remove(i);
					return true;
				}
			}
			return false;
		}
		
		const int GetCount()const{
			return services.GetCount();
		}
		
		Array<Service>& GetServices(){
			return services;
		}
		
		const Array<Service>& GetServices()const{
			return services;
		}
		
	protected:
		friend class UFEContext;
		ServiceManager(UFEContext& _context) : context(_context){}
		~ServiceManager(){
			services.Clear();
		}
		
		UFEContext& GetContext(){return context;}
		
		Service& GetServiceNoCast(const String& serviceName){
			for(Service& service : services){
				if(service.GetName().IsEqual(serviceName)){
					return service;
				}
			}
			throw Exc("ServiceManager exception : no service named "+ serviceName +" can be found");
		}
	
	private:
		UFEContext& context;
		Array<Service> services;
};

}


#endif
