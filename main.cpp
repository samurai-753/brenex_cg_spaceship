// pova1
#include <iostream>
#include <vector>
#include <cmath>

#include "vart/scene.h"
#include "vart/light.h"
#include "vart/arrow.h"
#include "vart/meshobject.h"
#include "vart/sphere.h"
#include "vart/contrib/viewerglutogl.h"
#include "vart/contrib/mousecontrol.h"

const unsigned int MAX_KEYS = 11;
enum Key {
	UP, DOWN, RIGHT, LEFT,
	TURN_UP, TURN_DOWN,
	TURN_CC, TURN_CS,
	RESET
	};
bool keyPressed[MAX_KEYS];

class KbH : public VART::ViewerGlutOGL::KbHandler {
	private:
		VART::Transform* base_transform;
		float angle;

	public:
		KbH(VART::Transform* base_transform) {
			this->base_transform = base_transform;
			this->angle = 0.0;
		}
		
		virtual void OnKeyUp(int key) {
			switch (key) {
				case 'u':
					keyPressed[TURN_UP] = false;
					break;
				
				case 'o':
					keyPressed[TURN_DOWN] = false;
					break;
				
				case 'y':
					keyPressed[TURN_CC] = false;
					break;
				
				case 'h':
					keyPressed[TURN_CS] = false;
					break;

				case 'j':
					keyPressed[LEFT] = false;
					break;

				case 'l':
					keyPressed[RIGHT] = false;
					break;

				case 'i':
					keyPressed[UP] = false;
					break;

				case 'k':
					keyPressed[DOWN] = false;
					break;
				
				case 'r':
					keyPressed[RESET] = false;
					break;
			}
		}
		
		virtual void OnKeyDown(int key) {
			switch(key) {
				case 'u':
					keyPressed[TURN_UP] = true;
					break;
				
				case 'o':
					keyPressed[TURN_DOWN] = true;
					break;

				case 'y':
					keyPressed[TURN_CC] = true;
					break;
				
				case 'h':
					keyPressed[TURN_CS] = true;
					break;
				
				case 'l':
					keyPressed[RIGHT] = true;
					break;
				   
				case 'j':
					keyPressed[LEFT] = true;
					break;
				
				case 'k':
					keyPressed[DOWN] = true;
					break;
					
				case 'i':
					keyPressed[UP] = true;
					break;

				case 'r':
					keyPressed[RESET] = true;
					break;
			}
		}
};

class IH : public VART::ViewerGlutOGL::IdleHandler {
	private:
		VART::Transform* base_transform;
		VART::Transform* rotateY;
		VART::Transform* rotateX;
		VART::Transform* rotateZ;
		VART::Point4D pos;
		double angleY;
		double angleX;
		double angleZ;

	public:
		IH(VART::Transform* t, VART::Transform* y, VART::Transform* x, VART::Transform* z) : VART::ViewerGlutOGL::IdleHandler() {
			this->base_transform = t;
			this->rotateY = y;
			this->rotateX = x;
			this->rotateZ = z;
			this->pos = VART::Point4D::ORIGIN();
			this->angleY = 0.0;
			this->angleX = 0.0;
			this->angleZ = 0.0;
		}
		
		virtual ~IH() {}
		
		virtual void OnIdle() {
			if(keyPressed[UP]) {
				this->pos.SetX(this->pos.GetX() + std::cos(-this->angleY)/10);
				this->pos.SetY(this->pos.GetY() + std::sin(this->angleZ)/10);
				this->pos.SetZ(this->pos.GetZ() + std::sin(-this->angleY)/10);
				this->base_transform->MakeTranslation(this->pos);
			}

			if(keyPressed[DOWN]) {
				this->pos.SetX(this->pos.GetX() - std::cos(-this->angleY)/10);
				this->pos.SetZ(this->pos.GetZ() - std::sin(-this->angleY)/10);
				this->pos.SetY(this->pos.GetY() - std::sin(this->angleZ)/10);
				this->base_transform->MakeTranslation(this->pos);
			}

			if(keyPressed[RIGHT]) {
				this->angleY -= 0.0349066;
				this->rotateY->MakeYRotation(this->angleY);
			}

			if(keyPressed[LEFT]) {
				this->angleY += 0.0349066;
				this->rotateY->MakeYRotation(this->angleY);
			}

			if(keyPressed[TURN_UP]) {
				this->angleX -= 0.0349066;
				this->rotateX->MakeXRotation(this->angleX);
			}

			if(keyPressed[TURN_DOWN]) {
				this->angleX += 0.0349066;
				this->rotateX->MakeXRotation(this->angleX);
			}

			if(keyPressed[TURN_CC]) {
				this->angleZ += 0.0349066;
				this->rotateZ->MakeZRotation(this->angleZ);
			}

			if(keyPressed[TURN_CS]) {
				this->angleZ -= 0.0349066;
				this->rotateZ->MakeZRotation(this->angleZ);
			}

			if(keyPressed[RESET]) {
				this->angleX = 0.0;
				this->angleY = 0.0;
				this->angleZ = 0.0;
				this->pos = VART::Point4D::ORIGIN();

				this->rotateX->MakeIdentity();
				this->rotateY->MakeIdentity();
				this->rotateZ->MakeIdentity();
				this->base_transform->MakeIdentity();
			}

			viewerPtr->PostRedisplay();
		}
	
};

class Piramide2 : public VART::MeshObject {
	public:
		Piramide2();
};

Piramide2::Piramide2() {
	std::vector<double> points({
		-1, 0, 0,
		0, 0, 1,
		2, 0, 0,
		0, 0, -1,
		0, 1, 0
	});
	
	this->vertCoordVec = points;
	
	VART::Material PLASTIC_PINK(VART::Color(234, 81, 178));
	
	std::vector<unsigned int> faces_quad({
		3, 2, 1, 0,
	});
	VART::Mesh mesh_quadri;
	mesh_quadri.type = VART::Mesh::QUADS;
	mesh_quadri.indexVec = faces_quad;
	mesh_quadri.material = PLASTIC_PINK;
	this->meshList.push_back(mesh_quadri);
	
	std::vector<unsigned int> faces_trig({
		1, 2, 4,
		2, 3, 4,
		3, 0, 4,
		0, 1, 4,
	});
	VART::Mesh mesh_trig;
	mesh_trig.type = VART::Mesh::TRIANGLES;
	mesh_trig.indexVec = faces_trig;
	mesh_trig.material = PLASTIC_PINK;
	this->meshList.push_back(mesh_trig);
	
	this->ComputeVertexNormals();
	this->ComputeBoundingBox();
	this->ComputeRecursiveBoundingBox();
}


int main(int argv, char* argc[]) {
	VART::ViewerGlutOGL::Init(&argv, argc);
	
	VART::Scene scene;
	VART::ViewerGlutOGL viewer;
	
	// ========================================================================

	VART::Transform base_transform;
	base_transform.MakeIdentity();

	VART::Transform rotateY;
	rotateY.MakeIdentity();
	base_transform.AddChild(rotateY);
	
	VART::Transform rotateX;
	rotateX.MakeIdentity();
	rotateY.AddChild(rotateX);

	VART::Transform rotateZ;
	rotateZ.MakeIdentity();
	rotateX.AddChild(rotateZ);

	Piramide2 p;
	rotateZ.AddChild(p);
	
	scene.AddObject(&base_transform);
	
	KbH kbh(&rotateY);
	viewer.SetKbHandler(&kbh);

	IH idle_handler(&base_transform, &rotateY, &rotateX, &rotateZ);
	viewer.SetIdleHandler(&idle_handler);
	
	// ========================================================================
	
	VART::Arrow arrowX(2);
	VART::Arrow arrowY(VART::Point4D::ORIGIN(), VART::Point4D::Y()*2);
	VART::Arrow arrowZ(VART::Point4D::ORIGIN(), VART::Point4D::Z()*2);
	
	arrowX.SetMaterial(VART::Color::RED());
	arrowY.SetMaterial(VART::Color::GREEN());
	arrowZ.SetMaterial(VART::Color::BLUE());
	
	scene.AddObject(&arrowX);
	scene.AddObject(&arrowY);
	scene.AddObject(&arrowZ);
	
	VART::Camera camera(
		VART::Point4D(10, 10, 10),
		VART::Point4D::ORIGIN(),
		VART::Point4D::Y()
	);
	scene.AddLight(VART::Light::BRIGHT_AMBIENT());
	scene.AddCamera(&camera);

	viewer.SetTitle("walk");
	viewer.SetScene(scene);

	std::cout << "Modo de usar:" << std::endl
		<< "- movimentação em ijkl" << std::endl
		<< "- rotação em X uo" << std::endl
		<< "- rotação em Z yh" << std::endl
		<< "- resetar posição r" << std::endl
		<< std::endl;

	scene.DrawLightsOGL();
	VART::ViewerGlutOGL::MainLoop();
	return 0;
}
