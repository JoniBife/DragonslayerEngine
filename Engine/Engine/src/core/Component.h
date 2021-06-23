#ifndef COMPONENT_H
#define COMPONENT_H

/*
* Base class for all components. A component can be added to a GameObject.
* Examples of components include: RigidBody, MeshRenderer etc..
*/
class Component {

public:
	/*Updates the component*/
	virtual void update() = 0;
	
};

#endif 


