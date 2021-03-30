UFE Design and concept presentation

//POST 1

During presentation of [SurfaceCtrl, the 3D viewer](https://www.ultimatepp.org/forums/index.php?t=msg&th=11148&start=0&)I have said I was working on something bigger,
a kind of game engine to handle in a better way 3D/2D environment. Today, after many hours of work, I want to present my work to Upp community in order to :
First : Get some feedback about Utility of my work to U++ 
Second : having an honest review of my work both in terms of "clean code" and design/architecture 
Third : Because I'm kind of proud of what I have done and I'm happy to share it with U++ community

Ultimate framework engine is a game engine dedicated to handling of 3D object. The way it work is simple, it use 6 classes :
![archi](https://i.imgur.com/UvNx8en.png)

-Context is the only instanciable class, it allow creation of scene and service, and provide a tiny API to calculate ellasped time and delta time.
-Scene have purpose of carrying object. it provide an ObjectManager to create or remove object of is scene.
-Object have a Transform object and can have component(0 - N component(s) managed by a ComponentManager)
-Transform is a part of object which represent position rotation and scall of the object
-Component is one of two abstract class of UFE, it provide an interface with a lot of functions allowing UFE to interact with your own code and your own inherrited class of the Component class
-Service is the second abstract class, like Component, service is simply an interface. The main idea here, is to use service to provide many custom function (by exemple rendering using opengl)and use 
 component to call some function of service depending on Object component is bind on. (I will show some examples further !)

All class that can instantiate object use a Manager in order to allow creation / destruction / retrieve of object (SceneManager, ServiceManager, ObjectManager ...)

**Context timer API :**

```c++
class UFEContext{
...
	void TimerStart();
	void TimerStop();
	void TimerReset();
	void TimerUpdateByHand(double timeStepInMS);
	double GetTimeEllapsed();
	double GetDeltaTime();
...
};
```
Time is important when we are doing 3D, it's for this reason Context provide a way of tracking ellasped time. 
delta/ellasped time can be retrieve over the Update function of component or by calling GetTimeEllapsed() or GetDeltaTime().

**GameLoop and update :**

```c++
class UFEContext{
...
	void Update();
...
}
class Scene{
...
	void Update(double timeEllapsed, double deltaTime = 0.0);
...
}
class Object{
...
	void Update(double timeEllapsed, double deltaTime = 0.0);
...
}
class Component{
...
	virtual void Update(double timeEllapsed, double deltaTime = 0.0);
...
}
```
To update all object, we must call Update function of context object, it will call the update function of the active scene
then all update function of all objects of this scene which will call all update function of each component of object. This said, a typical gameloop looks like that :

```c++
context.TimerStart(); // we start the timer before entering the game loop 
//GLFW is a lib used to generate a window 
while (!glfwWindowShouldClose(window)) //While we didnt asked to close Window
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //openGL function (nothing to do with UFE)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//openGL function
	

try{
	context.Update(); //our UFE context
}catch(Upp::Exc& exception){
	Upp::Cout() << exception << Upp::EOL;
}

glfwSwapBuffers(window); //GLFW function (nothing to do with UFE)
glfwPollEvents();//GLFW function

}
```

**All manager :**
UFE have a lot of class manager:

```c++
class UFEContext{
...
	ServiceManager& GetServiceManager();
	const ServiceManager& GetServiceManager()const;
	

SceneManager& GetSceneManager();
const SceneManager& GetSceneManager()const;

...
}

class Scene{
...
	ObjectManager& GetObjectManager();
	const ObjectManager& GetObjectManager()const;
...
}

class Object{
...
	ComponentManager& GetComponentManager();
	const ComponentManager& GetComponentManager()const;
...
}
```



it's simply a wrapper of Vector allowing creation of Scene/Object/Component. It provide a way to retrieve Scene/Object/Component by name or ID  etc...

**Transform object :**
The transform object is always carried by Object of UFE. It's used to track position rotation and scale of is object into a 3D environment. 
It use (glm::vec3) [OpenGL mathematic library website](https://glm.g-truc.net/0.9.9/index.html) to save scale and position and glm::quaternion to save rotation.
It also provide many function to move or look at an object :
![Transform](https://i.imgur.com/yYvStdB.png)

**Service and Component :**
As said earlier, Service and Component are the two abstract class of UFE :
![Service and Component](https://i.imgur.com/lsmT4lf.png)
It have to be inherrited to add custom features to UFE (the example introduced later will present of to use them to render thing with OpenGL)

**Error handling :**
All manager of UFE throw exception when trying to retrieve object wihtout giving wrong name or wrong iterator

**ToString function and Dump example :**
All UFE object have ToString function defined. Which mean you can Dump any object of UFE, Here is a dump of UFEContext object after decleration of the example we gonna see later: http://pastebin.fr/71568
Here is a screenshot of console : 
![Dump](https://i.imgur.com/wYDkEvC.png)

**PS : this name has been chosen in 5 seconds, feel free to tell me to not use Ultimate framework in it**



**Presentation of OpenGL service:**

The first service I have worked on is of course an OpenGL Renderer ! let's see how I have made this.
![Archi](https://i.imgur.com/09lIWKX.png)

To handle all OpenGL stuff like Shader / Model / Texture etc I have created some class (which can be seen on the screenshot under the 'Structure' separator)
All instance of thoses class and all the rendering code must be carried by the inherrited Service class (named SGLRender for Service OpenGL Renderer):

```c++
/*
	In UFE Service always start by S letter
*/
class SGLRenderer : public Service{
	public:
		SGLRenderer(UFEContext& _context);
		~SGLRenderer();
		

	String GetName()const{return "SGLRenderer";}
	
	DataManager<Model>& GetModels(){return models;}
	const DataManager<Model>& GetModels()const{return models;}
	
	DataManager<Shader>& GetShaders(){return shaders;}
	const DataManager<Shader>& GetShaders()const{return shaders;}
	
	DataManager<Renderer>& GetRenderers(){return renderers;}
	const DataManager<Renderer>& GetRenderers()const{return renderers;}
	
	DataManager<Texture>& GetTextures(){return textures;}
	const DataManager<Texture>& GetTextures()const{return textures;}
	
	void AddToRenderQueue(Renderer& renderer, Shader& shader, Model& model, Object& object); //Ask to the service to add a (model + Shader + Renderer + object) to the renderer queue in order to be rendered on next update 
	
	virtual String ToString()const;
	
	virtual void AfterUpdate();

...		
}
```

DataManager is a simple wrapper arround a VectorMap, allowing creation deletion etc...

We now need a way of binding Model and Shader to UFE Object. To do this, we gonna create some Component (which can be seen on the screenshot under the 'Components' separator)

```c++
/*
	In UFEngine Component always start by C letter
*/
class CGLModel : public Component{
	public:
		String model;
		CGLModel& SetModel(const String& modelName){model = modelName; return *this;}
		

	virtual String ToString()const{
		//To String is not importante in this example
	}
	virtual String GetName()const{
		return "CGLModel";
	}

};
```

As you can see, this CGLModel component have only purpose of storing a String with the name of model which need to be use to render the object. 
This component will be added to Any UFE Object since it inherrite from Component abstract class :

```c++
Upp::Object& triangle = context.GetSceneManager().GetActiveScene().GetObjectManager().CreateObject("object1");
triangle.GetComponentManager().CreateComponent<Upp::CGLModel>().SetModel("triangle"); //triangle is the name of the model we loaded before on the code
```

But at the moment, their is no way to ask our service to render an Object using is Model component. To awnser this problem, I have created a Component named CGLShader :
(the version of CGLShader presented here is lighter than the real CGLShader, we will see the real one later)

```c++
class CGLShader : public Component{
	public:
		CGLShader(){
			//Out of scope of this example
		}
		

	virtual void Update(double timeEllapsed, double deltaTime = 0.0){
		try{
			if(!shader.GetCount())throw Exc("no renderer name have been set");
			
			SGLRenderer& sglService = GetObject().GetScene().GetContext().GetServiceManager().GetService<SGLRenderer>();
			Shader& shad = sglService.GetShaders().Get(shader);
			Model& model = sglService.GetModels().Get(GetObject().GetComponentManager().GetActiveComponent<CGLModel>().model);
			Renderer& renderer = sglService.GetRenderers().Get(GetObject().GetComponentManager().GetActiveComponent<CGLRenderer>().renderer);
			
			sglService.AddToRenderQueue(renderer,shad, model, GetObject());
		}catch(Exc& exception){
			LOG("CGLRenderer update of " + GetObject().GetName() + " failled : " + exception);
		}
	}
	
	String shader;
	
	CGLShader& SetShader(const String& shaderName){shader = shaderName; return *this;}
	
	virtual String ToString()const{
		//Out of scope of this example
	}
	virtual String GetName()const{
		return "CGLShader";
	}

};
```



Beside the fact CGLShader is used (like CGLModel) to refere via is String to a Shader loaded before in SGLService, it also have is Update(...) Function overwritten to perform a think at each update :
Basically, 
	-it first retrieve the SGLRenderer Service
    -then he resolve the shader refered by itself 
	-then he look for the CGLModel component of is Object to resolve the Model object 
    -then he look for the CGLRenderer component of is object to resolve the Renderer object
	-Last, he call AddToRenderQueue(...) function of SGLRenderer to ask the service to render this object on the next frame


Here is how the OpenGL renderer service basicly work, however, it also provide mutch more Component allowing to add Texture and perform different kind of Draw ! lets see this in an example !



**Example of using UFE and OpenGL renderer service :**

[Video of this example](https://youtu.be/S4ISRm7bgSw)

for the simplicity of this example, here is the base code (A simple Upp::TopWindow with a GLCtrl implemented in it) :

**UFEWithUpp.cpp**

```c++
#include <CtrlLib/CtrlLib.h>
#include <GLCtrl/GLCtrl.h>
#include <UFEngine/UFEngine.h>
#include <pluginUFE/OpenGLRenderer/OpenGLRenderer.h>

#include "vertices.h"
#include "Components.h"

namespace Upp{

class UFEWithUpp : public TopWindow{
public:
	typedef UFEWithUpp CLASSNAME;
	

UFEWithUpp()
{
	Add(GLCanvas.HSizePos(10, 10).VSizePos(10, 10));
	context.TimerStart(); //Notice that we start contex time at creation
	GLCanvas.WhenGLPaint = THISBACK(OnPaint);
}

virtual bool Key(dword key, int count){
	//We gonna use this function to add some interaction with the example, for now it only close the window and stop the program when escape is pressed
	if(key == K_ESCAPE){
		Close();
	}
}

private:
	UFEContext context;
	GLCtrl GLCanvas;
	

bool loaded = false;
void OnPaint(){
	//This function will be the game loop
}

void Init(){
	//In this function, we gonna load everything we need to render a cube and a triangle
}

};

}

GUI_APP_MAIN
{
	Upp::StdLogSetup(Upp::LOG_COUT| Upp::LOG_FILE);
	Upp::UFEWithUpp().Sizeable().Zoomable().Run();
}
```

**Component.h**

```c++
#ifndef _ExempleOpenGLService_Components_h_
#define _ExempleOpenGLService_Components_h_

#include <Core/Core.h>
#include <UFEngine/UFEngine.h>

namespace Upp{
	class RotationComponent : public Upp::Component{
		public:
			virtual Upp::String GetName()const{
				return "RotationComponent";
			}
			virtual void Update(double timeEllapsed, double deltaTime = 0.0){
				GetObject().GetTransform().Rotate(deltaTime * 50, glm::vec3(0,1,0));
			}
			virtual String ToString()const{
				String toString;
				toString << "Object name: " << GetObject().GetName() << "\n";
				toString << "Active: " << IsActive() << "\n";
				toString << "Purpose: This Component allow object to rotate clockwise\n";
				return toString;
			}
	};
	

class TranslationComponent : public Upp::Component{
	public:
		virtual Upp::String GetName()const{
			return "TranslationComponent";
		}
		virtual void Update(double timeEllapsed, double deltaTime = 0.0){
			GetObject().GetTransform().SetPosition(glm::vec3(0,glm::cos(timeEllapsed) * 10,0));
		}
		virtual String ToString()const{
			String toString;
			toString << "Object name: " << GetObject().GetName() << "\n";
			toString << "Active: " << IsActive() << "\n";
			toString << "Purpose: This Component allow object to move upward and downward depending on time (simple 			                          cos(timeEllapsed))\n";
			return toString;
		}
};

class LookAt : public Upp::Component{
	private:
		Upp::Object* obj = nullptr;
	public:
		
		LookAt& SetObjectToLook(Object& _obj){
			if(_obj.GetScene().GetName().IsEqual(GetObject().GetScene().GetName())){
				obj = &_obj;
			}else{
				LOG("LookAt component exception: Object to focus is not on the same scene ! Parallel universe can't look at each 					  other since they don't know about himself !");
			}
			return *this;
		}
		
		virtual Upp::String GetName()const{
			return "LookAt";
		}
		
		virtual String ToString()const{
			String toString;
			toString << "Object name: " << GetObject().GetName() << "\n";
			toString << "Active: " << IsActive() << "\n";
			if(obj){
				toString << "Actual focus: object named '" << obj->GetName() << "'\n";
			}else{
				toString <<"Actual focus: No object is actually focused";
			}
			toString << "Purpose: This Component allow object to look at another Transform object permatly\n";
			return toString;
		}

		virtual void Update(double timeEllapsed, double deltaTime = 0.0){
			if(obj){
				GetObject().GetTransform().LookAt((*obj).GetTransform().GetPosition(),GetObject().GetTransform().GetWorldUp());
			}
		}
};

}
#endif
```

**Vertices.h**

```c++
#ifndef _ExempleOpenGLService_vertices_h_
#define _ExempleOpenGLService_vertices_h_
#include <Core/Core.h>

Upp::Vector<float> verticesTriangle{
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

Upp::Vector<float> verticesCarre{
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

     0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f
};
#endif
```

In order to draw something on screen, let's say a triangle, we have to :

1: Create a scene by using UFEContext object named context 
2: Create an object on this scene which will represent our triangle (we gonna call it triangle)
3: Adding OpenGL renderer service to our context (by creating a SGLRenderer using the ServiceManager)
4: give the vector of float representing the triangle (the one named verticesTriangle on Vertices.h) to our OpenGL renderer service (in order to load it into opengl)
5: Definiting how our data should be read and draw using Renderer and Shader object of OpenGL renderer service
6: Add Component to our Triangle Object to make it refer our Model Renderer and Shader
7: Write a correct game loop
8: Add a camera to the scene


Let's begin !

**1 : Create a scene** 
Since creating a scene should be done at loading of our code, we must do it in our Init function, to create a scene we simply call our SceneManager and ask for creation :

```c++
void Init(){
	try{
		Scene& scene1 = context.GetSceneManager().CreateScene("scene1");
		context.GetSceneManager().SetActiveScene("scene1");
	}catch(Upp::Exc& exc){
		Upp::Cout() << exc << Upp::EOL;
	}
}
```

UFE identify scene by linking them to a String (here "scene1"), To find what scene to render he simply get the active scene which is set by calling SetActiveScene(String scenename) from SceneManager.
*Quick note : UFE automaticly set the active scene on the first created if no active scene are specified*

2 : Create an object on this scene
Like our scene, to create an object we just call the ObjectManager of our scene :

```c++
void Init(){
	try{
		...
		Upp::Object& triangle = context.GetSceneManager().GetActiveScene().GetObjectManager().CreateObject("triangle");
	}catch(Upp::Exc& exc){
		Upp::Cout() << exc << Upp::EOL;
	}
}
```

**3 : Adding OpenGL renderer service to our context**
We simply call our ServiceManager and ask him to create an instance of SGLRenderer (for a bit cleaner code, I have added this part to the begining of Init function)

```c++
void Init(){
	try{
		Upp::SGLRenderer& SGL = context.GetServiceManager().CreateService<Upp::SGLRenderer>();
    	...
    }catch(Upp::Exc& exc){
    	Upp::Cout() << exc << Upp::EOL;
    }

}
```

**4 : Bring the float Vector representing a triangle to our SGLRenderer to create a Model**
As Scene or Object, our SGLRenderer provide a ModelManager allowing to create Model object. This Model object is capable of sending data to VRAM via OpenGL, let's do it !

```c++
void Init(){	
	try{
		...
		Upp::Model& ourTriangleModel = SGL.GetModels().Create("triangleModel");
		ourTriangleModel.AddVertices(verticesTriangle,9);
		ourTriangleModel.Load();
		...
	}catch(Upp::Exc& exc){
		Upp::Cout() << exc << Upp::EOL;
	}
}
```

The AddVertices function of model object simply take a Vector of float and is vertices number. Then we have to call Load function to send all data registered to our VRAM.

**5 : Explain how our model should be read and how it should be draw using Renderer and Shader**
In OpenGL, to draw something we have to specify how to draw it using Shader. I wont explain how shader work in this example ([Check this for explanation about shader and shading language)](https://learnopengl.com/Getting-started/Shaders)
however, I will provide you shader I have used to draw our triangle :

```c++
void Init(){
	try{
		...
		tring vertexShader = "#version 400 core\r\nlayout (location  = 0) in vec3 positions;\r\nuniform mat4 view;\r\nuniform mat4 proj;\r\nuniform mat4 model;\r\nvoid main(void){\r\n\tgl_Position = proj * view * model * vec4(positions,1.0);\r\n}";
		String fragmentShader = "#version 400\r\nout vec4 out_Color;\r\nvoid main(void){\r\n\tout_Color = vec4(1,0,0,1);\r\n}";
		
		Upp::Shader& shader = SGL.GetShaders().Create("simpleShader");
		try{
			shader.LoadFromString(vertexShader,Upp::ShaderType::VERTEX);
			shader.LoadFromString(fragmentShader,Upp::ShaderType::FRAGMENT);
			shader.Link();
			
			shader.SetUniformProjectionMatrixName("proj");
			shader.SetUniformViewMatrixName("view");
	
			shader.RegisterUniform("view");
			shader.RegisterUniform("proj");
			shader.RegisterUniform("model");
		}catch(Upp::Exc& e){
			Upp::Cout() << e << Upp::EOL;
		}
		...
	}catch(Upp::Exc& exc){
		Upp::Cout() << exc << Upp::EOL;
	}
}
```



We simply create a Shader using the SGLRenderer and load it using both String set before We then link it. 
the part about registering uniform and setting some uniform name allow UFE to automaticly send data to the shader before drawing object.

After having set the way Model should be draw we have to define how our Model should be read (i.e sent to our shader). To do this, we have to create a Renderer object using SGLRenderer :

```c++
void Init(){
	try{
		...
		Upp::Renderer& renderer = SGL.GetRenderers().Create("triangleRenderer");
		renderer.GetVertexArrayAttribFormat().Append(
			Upp::Vector<Upp::VAAF>{
				Upp::VAAF(0, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(float))
			}
		);
		renderer.LoadVAO();
		...
	}catch(Upp::Exc& exc){
		Upp::Cout() << exc << Upp::EOL;
	}
}
```

What VAAF is for ? VAAF is a simple struct that define how data should be sent over all layout of our shader. the way it work is based on the [glVertexAttribFormat ](https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribFormat.xhtml)function of OpenGL.

In our example, we simply set one VAAF on the VertexAttribFormat vector of Renderer object. This one VAAF define that the layout we use is the number 0, it have to receive 3 element of type GL_FLOAT, the GL_FALSE define that the data should not be normalized and the last argument is the distance between elements within the buffer we use (i.e our Model).

if you have not really understood this part, this is because it require some OpenGL base knowledge, see [Explanation on shader and VAO](https://learnopengl.com/Getting-started/Hello-Triangle) (VAO is our Renderer)

```c++
void Init(){
	try{
		...
		tring vertexShader = "#version 400 core\r\nlayout (location  = 0) in vec3 positions;\r\nuniform mat4 view;\r\nuniform mat4 proj;\r\nuniform mat4 model;\r\nvoid main(void){\r\n\tgl_Position = proj * view * model * vec4(positions,1.0);\r\n}";
		String fragmentShader = "#version 400\r\nout vec4 out_Color;\r\nvoid main(void){\r\n\tout_Color = vec4(1,0,0,1);\r\n}";
		

	Upp::Shader& shader = SGL.GetShaders().Create("simpleShader");
	try{
		shader.LoadFromString(vertexShader,Upp::ShaderType::VERTEX);				             shader.LoadFromString(fragmentShader,Upp::ShaderType::FRAGMENT);
		shader.Link();
		
		shader.SetUniformProjectionMatrixName("proj");
		shader.SetUniformViewMatrixName("view");

		shader.RegisterUniform("view");
		shader.RegisterUniform("proj");
		shader.RegisterUniform("model");
	}catch(Upp::Exc& e){
		Upp::Cout() << e << Upp::EOL;
	}
	...
}catch(Upp::Exc& exc){
	Upp::Cout() << exc << Upp::EOL;
}

}
```

Now we have all our data we need to render thing using OpenGL, we can back to our Triangle object and add Component to it to refere our Shader Renderer and Model :

```c++
void Init(){
	try{
		...
		triangle.GetComponentManager().CreateComponent<Upp::CGLModel>().SetModel("triangleModel");
		triangle.GetComponentManager().CreateComponent<Upp::CGLRenderer>().SetRenderer("triangleRenderer");
			          triangle.GetComponentManager().CreateComponent<Upp::CGLShader().SetShader("simpleShader").SetUniformModelMatrixName("model");
	}catch(Upp::Exc& exc){
		Upp::Cout() << exc << Upp::EOL;
	}
}
```

As you can see, all component simply refer to all data we created before using their name.

**7 : Write a correct game loop**
We now have an active Scene with an Object which have component referring to Triangle model Renderer and shader. Let's ask UFE to draw all of this using on Paint function :

```c++
void OnPaint(){
	if(!loaded){
		Init();
		loaded = true;
	}
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

    try{
        context.Update();
    }catch(Upp::Exc& exception){
        Upp::Cout() << exception << Upp::EOL;
    }
    GLCanvas.Refresh();

}
```

this loop will allow UFE to draw many frame on second (it depend on your machine)

**8 : Add a camera to the scene**
if you launched this example before this part, then you may have seen a background but no triangle ! this is because we have not defined a camera (a point of view) to see our Scene !
let's add a camera :

```c++
void Init(){
	try{
		...
		Upp::Object& camera = context.GetSceneManager().GetActiveScene().GetObjectManager().CreateObject("camera");
		camera.GetComponentManager().CreateComponent<Upp::CGLCameraPerspective>();
		camera.GetTransform().SetPosition(0,0,3);
	}catch(Upp::Exc& exc){
		Upp::Cout() << exc << Upp::EOL;
	}
}
```

if you now launch the code, you will now see a red triangle. 
![triangle](https://i.imgur.com/tFHHqg1.png)

the complete Init function :

```c++
void Init(){
	try{
		Upp::SGLRenderer& SGL = context.GetServiceManager().CreateService<Upp::SGLRenderer>();
		

	Upp::Model& ourTriangleModel = SGL.GetModels().Create("triangleModel");
	ourTriangleModel.AddVertices(verticesTriangle,9);
	ourTriangleModel.Load();

	String vertexShader = "#version 400 core\r\nlayout (location  = 0) in vec3 positions;\r\nuniform mat4 view;\r\nuniform mat4 proj;\r\nuniform mat4 model;\r\nvoid main(void){\r\n\tgl_Position = proj * view * model * vec4(positions,1.0);\r\n}";
	String fragmentShader = "#version 400\r\nout vec4 out_Color;\r\nvoid main(void){\r\n\tout_Color = vec4(1,0,0,1);\r\n}";
	Upp::Shader& shader = SGL.GetShaders().Create("simpleShader");
	try{
		shader.LoadFromString(vertexShader,Upp::ShaderType::VERTEX);
		shader.LoadFromString(fragmentShader,Upp::ShaderType::FRAGMENT);
		shader.Link();
		
		shader.SetUniformProjectionMatrixName("proj");
		shader.SetUniformViewMatrixName("view");

		shader.RegisterUniform("view");
		shader.RegisterUniform("proj");
		shader.RegisterUniform("model");
	}catch(Upp::Exc& e){
		Upp::Cout() << e << Upp::EOL;
	}

	Upp::Renderer& renderer = SGL.GetRenderers().Create("triangleRenderer");
	renderer.GetVertexArrayAttribFormat().Append(
		Upp::Vector<Upp::VAAF>{
			Upp::VAAF(0, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(float))
		}
	);
	renderer.LoadVAO();
	
	Scene& scene1 = context.GetSceneManager().CreateScene("scene1");
	context.GetSceneManager().SetActiveScene("scene1");
	
	Upp::Object& triangle = context.GetSceneManager().GetActiveScene().GetObjectManager().CreateObject("triangle");
	triangle.GetComponentManager().CreateComponent<Upp::CGLModel>().SetModel("triangleModel");
	triangle.GetComponentManager().CreateComponent<Upp::CGLRenderer>().SetRenderer("triangleRenderer");
	triangle.GetComponentManager().CreateComponent<Upp::CGLShader>().SetShader("simpleShader").SetUniformModelMatrixName("model");

	Upp::Object& camera = context.GetSceneManager().GetActiveScene().GetObjectManager().CreateObject("camera");
	camera.GetComponentManager().CreateComponent<Upp::CGLCameraPerspective>();
	camera.GetTransform().SetPosition(0,0,3);
	
	DUMP(context);
}catch(Upp::Exc& exc){
	Upp::Cout() << exc << Upp::EOL;
}

}
```

