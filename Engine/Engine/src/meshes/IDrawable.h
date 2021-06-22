#ifndef IDRAWABLE_H
#define IDRAWABLE_H

/*
* A drawable interface that should implemented by all entities
* that want to be drawn onto the screen.
* 
* Usage example:
* 
* std::vector<IDrawable> drawables = ...;
* 
* for (IDrawable dr : drawables) {
*	dr.bind();
*	dr.draw();
* }
* 
*/
class IDrawable {

public:
	virtual ~IDrawable() = default;
	virtual void init() = 0;
	virtual void bind() = 0;
	virtual void unBind() = 0;
	virtual void draw() = 0;
};

#endif