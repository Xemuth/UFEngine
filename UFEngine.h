#ifndef _UFEngine_UEngine_h_
#define _UFEngine_UEngine_h_

/*
	UFEngine
	Writted by Clément Hamon, 22/09/2020
	
	UFE is here to provide everything needed to create a 3D engine / Scenegraph
	See Plugin/Documentations to know what service/component are available and what
	they are used to
*/

#include <Core/Core.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <plugin/glm/glm.hpp>
#include <plugin/glm/gtc/matrix_transform.hpp>
#include <plugin/glm/gtc/type_ptr.hpp>
#include <plugin/glm/gtx/quaternion.hpp>
#include <plugin/glm/ext/quaternion_trigonometric.hpp>
#include <plugin/glm/gtc/constants.hpp>
#include <plugin/glm/gtx/norm.hpp>
#include <plugin/glm/gtx/compatibility.hpp>
#include <plugin/glm/gtx/string_cast.hpp>
#include <plugin/glm/gtx/vector_angle.hpp>

#include "src/UFEContext.h"
#include "src/Scene.h"
#include "src/Object.h"
#include "src/Transform.h"
#include "src/Service.h"
#include "src/Component.h"

#include "src/SceneManager.h"
#include "src/ServicesManager.h"
#include "src/ObjectManager.h"
#include "src/ComponentManager.h"
#include "src/ComponentManagerEx.h"
#include "src/CRoutine.h"

#endif
