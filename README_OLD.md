# Ultimate Framework Engine

UFE is an engine dedicated to 3D scene creation. It provide multiple API to create and manage Scenes, Objects and Time. 
Its **Flexibility** is assured by usage a two abstract class named **Service* and **Component**. 
All **Object** of a **Scene** carry instance of class which are inherited class from base class **Component**. 
By redefining virtual function and working with **Service inherited class**, all **Components inherited class** will be able to provide many different operation
going from 3D rendering to physic/audio to video game mecaniques. the field of operating of this architecture is somewhere near the infinity !

![Class diagram of UFE core](https://i.imgur.com/8bcJ0I9.png)


This README have purpose of listing all know UFE plugin:

* [OpenGLRenderer](https://github.com/Xemuth/pluginUFE-OpenGLRenderer): Allow rendering via OpenGL of provided data structures.


Example of how UFE work using OpenGLRenderer :

In this example I will present how to render a simple cube with 2 texture applied on it. we gonna make it rotate !
<em>the complete code example of this demo can be found [here](https://github.com/Xemuth/ExempleOpenGLService), a video of this demo can be found [here](https://www.youtube.com/watch?v=7q83T7CDXGM) </em>


The first thing to do after creating a UFEContext object (as global or local scope, it don't matter as long as you keep it during the complete time of the algorithme) is to Add the service named SGLRenderer which is the service provided by plugin OpenGLRenderer used to render things with OpenGL:
```
//here context is a global variable of type UFEContext 
Upp::SGLRenderer& openGL = context.GetServiceManager().CreateService<Upp::SGLRenderer>();
```

SGLRenderer provide many function to create MeshData, Renderer, Texture etc... See the documentation of OpenGLRenderer plugin.
So we gonna create some modelData, Texture and Renderer to render a textured cube on screen :
```
//We pass a Vector<float> named verticesCarre (which represente coordinate of a cube in 3D space).
//We also pass a Vector<float> texCoordsCarre (which represente how the texture should be bind to our model. [See OpenGL tutorial to understand](https://learnopengl.com/Getting-started/Textures))
//Both Upp::Vector<float>{} are here to pass empty vector to normals and colors arguments since we wont use them at all.
openGL.CreateMeshData("carre").AddMesh(verticesCarre,Upp::Vector<float>{},Upp::Vector<float>{}, texCoordsCarre).Load();
//Load function will send all the data to Video RAM
```

Now we get a square, we must create a way to render it. SGLRenderer provide an object named Renderer, which allow you to set how to read the data sent to VRAM
and how to render it using Shader :
```
//Here we create an empty renderer named 'texture'
Upp::Renderer& rendererTexture = openGL.CreateRenderer("texture");
```

Now we have an empty renderer we must set some data to it. First lets set the shader. To do this, we retrieve is shader object and ask him to read a shader from file.
<em>For more information about shader, see [OpenGL shader tutorial](https://learnopengl.com/Getting-started/Shaders)</em>
```
Upp::ShaderProgram& shaderTexture =  rendererTexture.GetShaderProgram();
try{
//We load our shader
	shaderTexture.LoadFromFile("C:\\Upp\\xemuth\\Apps\\ExempleOpenGLService\\vertex.glsl",Upp::ShaderType::VERTEX);
	shaderTexture.LoadFromFile("C:\\Upp\\xemuth\\Apps\\ExempleOpenGLService\\fragmentTexture.glsl",Upp::ShaderType::FRAGMENT);
//We link our shaderProgram
	shaderTexture.Link();

//We register all Value (Uniform) we gonna sent to OpenGL when rendering
	shaderTexture.RegisterUniform("view");
	shaderTexture.RegisterUniform("proj");
	shaderTexture.RegisterUniform("model");
	shaderTexture.RegisterUniform("tex");
	shaderTexture.RegisterUniform("tex2");
}catch(Upp::Exc& e){
	Upp::Cout() << e << Upp::EOL;
}
```

Now, when our renderer will be use to render, it will first send this both information to our shader. However you will may ask "But what about our object coordinate ?". hold on ! it's coming !
Last thing to do with this renderer object is to define how the data which will have to be render have to be read by OpenGL. Here I will simply use the default way:
```
rendererTexture.GenerateVAO(); //We generate the default VAO (the one corresponding to the data we passed the function above)
```
We are done with our Renderer. 

Next thing to do before rendering a textured cube is to load texture in the OpenGL VRAM ! To do this, SGLRenderer provide a Texture object :
```
openGL.CreateLoadTexture("wall",Upp::StreamRaster::LoadFileAny(Upp::GetFileDirectory(__FILE__) +   "wall.jpg"));
openGL.CreateLoadTexture("awesomeFace",Upp::StreamRaster::LoadFileAny(Upp::GetFileDirectory(__FILE__) +   "AwesomeFace.png"));
```
Here I use a direct function of SGLRenderer to load 2 textures and give them a name which will be use to reference them.

We have everything we want to create our textured cube so now lets create it :

First lets have a look at code needed to add this cube to the scene
```
Upp::Object& cube = context.GetSceneManager().GetActiveScene().GetObjectManager().CreateObject("cube");
cube.GetComponentManager().CreateComponent<Upp::CGLModel>().SetModel("carre");
cube.GetComponentManager().CreateComponent<Upp::CGLRenderer>().SetRenderer("texture").SetModelMatrixUniformName("model");
cube.GetComponentManager().CreateComponent<Upp::CGLTexture>().SetTexture("wall").SetChannelToUse(0).SetUniformName("tex");
cube.GetComponentManager().CreateComponent<Upp::CGLTexture>().SetTexture("awesomeFace").SetChannelToUse(1).SetUniformName("tex2");
cube.GetComponentManager().CreateComponent<Upp::RotationComponent>();
cube.GetTransform().SetPosition(-2,0,0);
```
As you can see,the cube itself take only 6 lines to move, rotate, have multi texture etc...
lets explain them:

```
Upp::Object& cube = context.GetSceneManager().GetActiveScene().GetObjectManager().CreateObject("cube"); 
```
First, we need to create cube object. At this time, no texture/rendering method or anything
is stick to the object.

```
cube.GetComponentManager().CreateComponent<Upp::CGLModel>().SetModel("carre");
```
Next, we add a model data component to the object. To link a model data we need to pass a String via
'SetModel(const String& key)' function which will serve as Key to reference a loaded model of
SGLRenderer. In our case we set "carre" which is a model we loaded via SGLRenderer before our
cube code.

```
cube.GetComponentManager().CreateComponent<Upp::CGLRenderer>().SetRenderer("texture").SetModelMatrixUniformName("model");
```
The same will occure for the renderer we want to use to render our cube. We create our renderer
component and call a Methode name 'SetRenderer(const String& key)' and we passe the key (which is
a String) that represente the renderer we want to use. However, unlike our Model component, you can notice the use
of another function name 'SetModelMatrixUniformName(const String& name)'. CGLRenderer provide
two event, one named beforeRendering (is declaration is : Event<Renderer&, CGLCamera&, Object&> beforeRendering)
and the other is named afterRendering (is declaration is same as beforeRendering) and they
serve purpose of executing code before and after being rendered. By default, Renderer component
try to sent Model matrix of is object into shader using a variable named uniformName. The
SetModelMatrixUniformName allow user to define the name of is model matrix variable of the shader used in the renderer


```
cube.GetComponentManager().CreateComponent<Upp::CGLTexture>().SetTexture("wall").SetChannelToUse(0).SetUniformName("tex");
cube.GetComponentManager().CreateComponent<Upp::CGLTexture>().SetTexture("awesomeFace").SetChannelToUse(1).SetUniformName("tex2");
```
The next step is to define what texture we want to use. For this, we use CGLTexture component which allow to define the
name of texture we want to use via 'SetTexture(const String& key') (the texture need to be loaded via SGLRenderer)
Also, CGLTexture provide another two function. The first one is 'SetChannelToUse(int channel)'
which allow user to define in which OpenGL texture channel is texture should be bind. The second one
'SetUniformName(const String& name)' allow user to define the same of texture used in shader.
Same as CGLRenderer, CGLTexture provide two event, before and after rendering. By default, the
beforeRendering event is set to use channelToUse and UniformName  to bind and send to shader
information needed to render.

```
cube.GetComponentManager().CreateComponent<Upp::RotationComponent>();
```
Now we had a RotationComponent to make the cube rotate on itself, the component code can be
found in the repository of this example

```
cube.GetTransform().SetPosition(-2,0,0);
```
We move cube to a defined position.

And Tadam ! here is rotating textured cube ! 


A complete documentation about many example will be posted here soon...

