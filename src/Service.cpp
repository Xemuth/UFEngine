#include "Service.h"
#include "UFEContext.h"

namespace Upp{
Service::Service(UFEContext& _context) : context(_context){

}
Service::~Service(){

}

UFEContext& Service::GetContext(){
	return context;
}

void Service::OnCreation(){
	
}

void Service::OnDestruction(){
	
}

void Service::BeforeUpdate(){
	
}

void Service::AfterUpdate(){
	
}

Value Service::ReceiveMessage(const String& message,const Value& args){
	return Value();
}

int Service::GetContextID()const{
	return context.GetID();
}

String Service::ToString()const{
	String service;
	service << "Context Id: " << context.GetID() << "\n";
	service << "No more data to retrieve, ToString function have not been overloaded\n";
	return service;
}

}