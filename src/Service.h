#ifndef _UFEngine_Service_h_
#define _UFEngine_Service_h_

#include <Core/Core.h>

namespace Upp{
	
class UFEContext;

class Service{
	public:
		Service(UFEContext& _context);
		virtual ~Service();

		UFEContext& GetContext();
		
		virtual String GetName()const = 0;
		
		virtual void OnCreation();
		virtual void OnDestruction();
		
		virtual void BeforeUpdate();
		virtual void AfterUpdate();
		
		virtual Value ReceiveMessage(const String& message,const Value& args = Value());
		
		virtual String ToString()const;
		
	protected:
		friend class ServiceManager;
		int GetContextID()const;
	private:
		UFEContext& context;
};

}

#endif
