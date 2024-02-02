#include "d3dUtility.h"
#include "Texture.h"
#include <vector>

IDirect3DDevice9* Device = 0;
D3DXMATRIX carMatrix;
ID3DXMesh* carMesh = 0;
std::vector<D3DMATERIAL9>       meshMtrls(0);
std::vector<IDirect3DTexture9*> Textures(0);

const int Width = 1000;
const int Height = 700;

//Objects - Worlds - Materials
const int NumOfObj = 16;
ID3DXMesh* Objects[NumOfObj] = { 0 };
D3DXMATRIX  Worlds[NumOfObj];
D3DMATERIAL9 Mtrls[NumOfObj];

// Sky & Floor
D3DXMATRIX SkyFloor[2];
IDirect3DTexture9* skyTexture = 0;
IDirect3DTexture9* floorTexture = 0;
Texture* Floor = 0;
Texture* Sky = 0;

//walls
D3DXMATRIX walls[4];
IDirect3DTexture9* wallsTexture = 0; 
Texture* backWallTexture = 0;
Texture* frontWallTexture = 0;
Texture* leftWallTexture = 0;
Texture* rightWallTexture = 0;

//Movement variables
float xMove = 0.0f;
float yMove = 0.0f;
float zMove = 0.0f;
float movementSpeed = 0.04f;
float targetX = xMove;
float targetY = yMove;
float targetZ = zMove;
float rotationSpeed = 0.004f;
float rotation = 0.1f;

// Camera positions variables
float posX = 0.0f;
float posY = 6.5f;
float posZ = -9.0f;
bool method_camera = true;

struct Position {
public:
	float x, y, z;
	Position(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

struct DegreeDirection{
public:
	float degree;
	char direction;
	DegreeDirection(float degree, char direction) {
		this -> degree = degree;
		this -> direction = direction;
	}

};

// Movement prototypes
void xMoveFunction(float direction);
void yMoveFunction(float direction);
void zMoveFunction(float direction);
void animation(float degree);
void RenderShadow();
D3DXMATRIX rotationFunction(D3DXMATRIX currentPosition, Position firstPosition, Position movedPosition, DegreeDirection rotateToset);
D3DXMATRIX translationAfterMovement(D3DXMATRIX objectToTranslate, Position move);

// .x file prototypes 
bool createMesh(ID3DXMesh*& Mesh, std::vector<D3DMATERIAL9>& Mtrls, std::vector<IDirect3DTexture9*>& Textures, LPCWSTR fileName);
void drawXFile(ID3DXMesh* Mesh, std::vector<D3DMATERIAL9>Mtrls, std::vector<IDirect3DTexture9*> Textures, D3DXMATRIX World);
void cleanupXFile(ID3DXMesh* Mesh, std::vector<IDirect3DTexture9*> Textures);
void adjustPositionForXFile();

//Text mesh and matrix
ID3DXMesh* Text = 0;
D3DXMATRIX textMatrix;

// Text prototypes
bool createTextFunction();
void drawText(ID3DXMesh* Text);

void lighting(){

	D3DXVECTOR3 dir(2.0f, 2.0f, 2.0f);
	D3DXCOLOR   c = d3d::WHITE;
	D3DLIGHT9 dirLight = d3d::InitDirectionalLight(&dir, &c);
	Device->SetLight(0, &dirLight);
	Device->LightEnable(0, true);

}

void createHuman(){
	D3DXCreateCylinder(Device, 0.0f, 0.5f, 0.5f, 20, 20, &Objects[0], 0); // Hat
	D3DXCreateSphere(Device, 0.57f, 20, 20, &Objects[1], 0); // Head
	D3DXCreateBox(Device, 0.15f, 0.03f, 0.03f, &Objects[2], 0); // Left eyebrow
	D3DXCreateBox(Device, 0.15f, 0.03f, 0.03f, &Objects[3], 0); // Right eyebrow
	D3DXCreateSphere(Device, 0.05f, 20, 20, &Objects[4], 0); // Left eye
	D3DXCreateSphere(Device, 0.05f, 20, 20, &Objects[5], 0); // Right eye
	D3DXCreateCylinder(Device, 0.0f, 0.15f, 0.35f, 20, 20, &Objects[6], 0); // Nose
	D3DXCreateBox(Device, 0.3f, 0.03f, 0.03f, &Objects[7], 0); // Mouth
	D3DXCreateCylinder(Device, 0.15f, 0.25f, 0.6f, 20, 20, &Objects[8], 0); // Neck
	D3DXCreateBox(Device, 2.0f, 2.5f, 1.05f, &Objects[9], 0); // Body
	D3DXCreateBox(Device, 0.7f, 0.7f, 1.0f, &Objects[10], 0); // Left shoulder
	D3DXCreateBox(Device, 0.7f, 0.7f, 1.0f, &Objects[11], 0); // Right shoulder
	D3DXCreateCylinder(Device, 0.2f, 0.2f, 1.5f, 20, 20, &Objects[12], 0); // Left arm
	D3DXCreateCylinder(Device, 0.2f, 0.2f, 1.5f, 20, 20, &Objects[13], 0); // Right arm
	D3DXCreateCylinder(Device, 0.25f, 0.25f, 2.0f, 20, 20, &Objects[14], 0);// Left leg
	D3DXCreateCylinder(Device, 0.25f, 0.25f, 2.0f, 20, 20, &Objects[15], 0);// Right leg


	// World matrices
	D3DXMATRIX leftLeg, rightLeg, leftArm, rightArm;
	D3DXMATRIX  Rx;
	D3DXMatrixRotationX(&Rx, D3DX_PI * 0.5);

	// --Head translation--
	D3DXMatrixTranslation(&Worlds[0], 0.0f, 0.0, -5.2f); // Hat
	D3DXMatrixTranslation(&Worlds[1], 0.0f, 4.50f, 0.0f); // Head
	D3DXMatrixTranslation(&Worlds[8], 0.0f, 3.75f, 0.0f);// Neck

	// --Face translation--
	D3DXMatrixTranslation(&Worlds[2], -0.2f, 4.62f, -0.52f); // Left eyebrow
	D3DXMatrixTranslation(&Worlds[3], 0.2f, 4.62f, -0.52f); // Right eyebrow
	D3DXMatrixTranslation(&Worlds[4], -0.2f, 4.5f, -0.52f); // Left eye
	D3DXMatrixTranslation(&Worlds[5], 0.2f, 4.5f, -0.52f); // Right eye
	D3DXMatrixTranslation(&Worlds[6], 0.0f, 4.4f, -0.52f); /// Nose
	D3DXMatrixTranslation(&Worlds[7], 0.0f, 4.2f, -0.51f); // Mouth

	// --Legs translation--
	D3DXMatrixTranslation(&leftLeg, -0.5f, 0.2f, 0.0f); // Left leg
	D3DXMatrixTranslation(&Worlds[14], 0.0f, 0.0f, 0.0f); // Left leg
	D3DXMatrixTranslation(&rightLeg , 0.5f, 0.2f, 0.0f); // Right leg
	D3DXMatrixTranslation(&Worlds[15], 0.0f, 0.0f, 0.0f); //Right leg

	// --Body translation--
	D3DXMatrixTranslation(&Worlds[9], 0.0f, 2.5f, 0.0f);//body
	D3DXMatrixTranslation(&leftArm, -1.2f, 2.2f, 0.0f); // Left arm
	D3DXMatrixTranslation(&Worlds[12], 0.0f, 0.0f, 0.0f); // Left arm
	D3DXMatrixTranslation(&rightArm, 1.2f, 2.2f, 0.0f); // Right arm
	D3DXMatrixTranslation(&Worlds[13], 0.0f, 0.0f, 0.0f); // Right arm

	// --Shoulders translation--	
	D3DXMatrixTranslation(&Worlds[10], -1.2f, 3.3f, 0.0f);//Left shoulder
	D3DXMatrixTranslation(&Worlds[11], 1.2f, 3.3f, 0.0f);//Right shoulder


	Worlds[14] = Rx * leftLeg; // Left leg
	Worlds[15] = Rx * rightLeg; // Right leg
	Worlds[12] = Rx * leftArm; // Left arm 
	Worlds[13] = Rx * rightArm; // Right arm
	Worlds[8] = (Rx * Worlds[8]); // Neck
	Worlds[0] *= (Rx);// Hat

	// Colors
	Mtrls[0] = d3d::YELLOW_MTRL;
	Mtrls[1] = d3d::WHITE_MTRL;
	Mtrls[2] = d3d::BLACK_MTRL;
	Mtrls[3] = d3d::BLACK_MTRL;
	Mtrls[4] = d3d::BLACK_MTRL;
	Mtrls[5] = d3d::BLACK_MTRL;
	Mtrls[6] = d3d::WHITE_MTRL;
	Mtrls[7] = d3d::BLACK_MTRL;
	Mtrls[8] = d3d::BLACK_MTRL;
	Mtrls[9] = d3d::YELLOW_MTRL;
	Mtrls[10] = d3d::BLACK_MTRL;
	Mtrls[11] = d3d::BLACK_MTRL;
	Mtrls[12] = d3d::WHITE_MTRL;
	Mtrls[13] = d3d::WHITE_MTRL;
	Mtrls[14] = d3d::WHITE_MTRL;
	Mtrls[15] = d3d::WHITE_MTRL;
}

void createSkyAndFloor(){
	D3DXMATRIX Rx , InverseRx , helper;
	D3DXMatrixRotationX(&Rx, D3DX_PI * 0.5);
	D3DXMatrixRotationX(&InverseRx, 3 * D3DX_PI * 0.5);
	D3DXMatrixTranslation(&SkyFloor[0], 0.0f, 0.0f, 0.0f); // Floor
	D3DXMatrixTranslation(&SkyFloor[1], 0.0f, 0.0f, 0.0f);// Sky
	SkyFloor[0] *= Rx;
	SkyFloor[1] *= InverseRx;
	D3DXMatrixTranslation(&helper, 0.0f, -1.85f, 0.0f); // Floor translation
	SkyFloor[0] *= helper;
	D3DXMatrixTranslation(&helper, 0.0f, 20.0f, 0.0f);// Sky translation
	SkyFloor[1] *= helper;

	D3DXCreateTextureFromFile(
		Device,
		L"FloorTexture.jpg",
		&floorTexture);
	D3DXCreateTextureFromFile(
		Device,
		L"SkyTexture.jpg",
		&skyTexture);

	Floor = new Texture(Device);
	Sky = new Texture(Device);
}

void createWalls(){
	
	D3DXMATRIX frontWallTranslation, leftWallTranslation, rightWallTranslation;
	D3DXMATRIX frontWallRotation, leftWallRotation, rightWallRotation;
	
	// Assign value to rotation matrixes
	D3DXMatrixRotationY(&frontWallRotation, D3DX_PI);
	D3DXMatrixRotationY(&leftWallRotation, -D3DX_PI * 0.5);
	D3DXMatrixRotationY(&rightWallRotation, D3DX_PI * 0.5);
	
	D3DXMatrixTranslation(&walls[0], 0.0f, 0.0f, 40.0f);// Back wall
	D3DXMatrixTranslation(&walls[1], 0.0f, 0.0f, 0.0f); // Front wall
	D3DXMatrixTranslation(&frontWallTranslation, 0.0f, 0.0f, -40.0f); // Front wall
	D3DXMatrixTranslation(&walls[2], 0.0f, 0.0f, 0.0f); //Left wall
	D3DXMatrixTranslation(&leftWallTranslation , -40.0f, 0.0f, 0.0f); // Left wall translation
	D3DXMatrixTranslation(&walls[3], 0.0f, 0.0f, 0.0f); // Right wall
	D3DXMatrixTranslation(&rightWallTranslation , 40.0f, 0.0f, 0.0f); // Right wall translation
	
	walls[1] = frontWallRotation * frontWallTranslation;
	walls[2] = leftWallRotation * leftWallTranslation;
	walls[3] = rightWallRotation * rightWallTranslation;


	D3DXCreateTextureFromFile(
		Device,
		L"WallTexture2.JPG",
		&wallsTexture);
	backWallTexture = new Texture(Device);
	frontWallTexture = new Texture(Device);
	leftWallTexture = new Texture(Device);
	rightWallTexture = new Texture(Device);
}

bool Setup(){

	// Play sound when program starts
	PlaySound(L"Theme-song.wav", NULL, SND_LOOP | SND_ASYNC);

	createTextFunction();
	createMesh(carMesh , meshMtrls , Textures , L"car.x");
	createHuman();
	createSkyAndFloor();
	createWalls();

	// Car translation
	D3DXMatrixTranslation(&carMatrix, -5.0f, 1.80f, 3.0f);

	// Set lighting related render stat/es
	lighting();
	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	Device->SetRenderState(D3DRS_SPECULARENABLE, true);

	// Set Texture Filter States.
	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);


	// Projection matrix
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5,
		(float)Width / (float)Height,
		1.0f,
		1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);


	return true;
}

void Cleanup(){
	for (int i = 0; i < NumOfObj; i++)
		d3d::Release<ID3DXMesh*>(Objects[i]);
	cleanupXFile(carMesh , Textures);
	d3d::Release<ID3DXMesh*>(Text);
}

void animation(float degree) {

	// Legs animation
	Worlds[14] = rotationFunction(Worlds[14]
		, Position(-0.5f, 1.2f, 0.0f)
		, Position(xMove, yMove, zMove)
		, DegreeDirection(degree, 'x'));
	Device->SetTransform(D3DTS_WORLD, &Worlds[14]);
	Objects[14]->DrawSubset(0);

	Worlds[15] = rotationFunction(Worlds[15]
		, Position(0.5f, 1.2f, 0.0f)
		, Position(xMove, yMove, zMove)
		, DegreeDirection(-degree, 'x'));
	Device->SetTransform(D3DTS_WORLD, &Worlds[15]);
	Objects[15]->DrawSubset(0);

	// Hands animation
	Worlds[12] = rotationFunction(Worlds[12]
		, Position(-1.2f, 3.3f, 0.0f)
		, Position(xMove, yMove, zMove)
		, DegreeDirection(degree, 'x'));
	Device->SetTransform(D3DTS_WORLD, &Worlds[12]);
	Objects[12]->DrawSubset(0);

	Worlds[13] = rotationFunction(Worlds[13]
		, Position(1.2f, 3.3f, 0.0f)
		, Position(xMove, yMove, zMove)
		, DegreeDirection(-degree, 'x'));
	Device->SetTransform(D3DTS_WORLD, &Worlds[13]);
	Objects[13]->DrawSubset(0);
}

D3DXMATRIX rotationFunction(D3DXMATRIX currentPosition, Position firstPosition, Position movedPosition, DegreeDirection rotate) {
	D3DXMATRIX bringToZero, rotationMatrix , backToFirst;
	D3DXMatrixTranslation(&bringToZero, -(firstPosition.x + movedPosition.x)
		, -(firstPosition.y + movedPosition.y), -(firstPosition.z + movedPosition.z));
	//now the object is in 0.0.0
	currentPosition *= bringToZero;
	switch (rotate.direction){
	case 'x':
		D3DXMatrixRotationX(&rotationMatrix, rotate.degree);
		break;
	case 'y':
		D3DXMatrixRotationY(&rotationMatrix, rotate.degree);
		break;
	case 'z':
		D3DXMatrixRotationZ(&rotationMatrix, rotate.degree);
		break;
	}
	currentPosition *= rotationMatrix;
	
	// Initial position
	D3DXMatrixTranslation(&backToFirst, (firstPosition.x + movedPosition.x)
		, (firstPosition.y + movedPosition.y), (firstPosition.z + movedPosition.z));
	currentPosition *= backToFirst;
	return currentPosition;
}

void xMoveFunction(float direction) {

	if (direction > 0) {
		xMove += movementSpeed;
		for (int i = 0; i < NumOfObj; i++)
			Worlds[i] = translationAfterMovement(Worlds[i], Position(movementSpeed, 0.0f, 0.0f));
	}
	else {
		xMove -= movementSpeed;
		for (int i = 0; i < NumOfObj; i++)
			Worlds[i] = translationAfterMovement(Worlds[i], Position(-movementSpeed, 0.0f, 0.0f));
	}
}

void yMoveFunction(float direction) {
	if (direction > 0) {
		yMove += movementSpeed;
		for (int i = 0; i < NumOfObj; i++)
			Worlds[i] = translationAfterMovement(Worlds[i], Position(0.0f, movementSpeed, 0.0f));
	}
	else {
		yMove -= movementSpeed;
		for (int i = 0; i < NumOfObj; i++)
			Worlds[i] = translationAfterMovement(Worlds[i], Position(0.0f, -movementSpeed, 0.0f));

	}
}

void zMoveFunction(float direction) {
	if (direction > 0) {
		zMove += movementSpeed;
		for (int i = 0; i < NumOfObj; i++)
			Worlds[i] = translationAfterMovement(Worlds[i], Position(0.0f, 0.0f, movementSpeed));
	}
	else {
		zMove -= movementSpeed;
		for (int i = 0; i < NumOfObj; i++)
			Worlds[i] = translationAfterMovement(Worlds[i], Position(0.0f, 0.0f, -movementSpeed));
	}
}

D3DXMATRIX translationAfterMovement(D3DXMATRIX object, Position move) {
	D3DXMATRIX helper;
	D3DXMatrixTranslation(&helper, move.x, move.y, move.z);
	return object *= helper;
}

bool Display(float timeDelta){
	if (Device){

		float cameraSpeed = 0.1f;
		float doorMovement = 0.0f;

		if (::GetAsyncKeyState(VK_UP) & 0x8000f) posY += cameraSpeed;
		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f) posY -= cameraSpeed;
		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f) posX -= cameraSpeed;
		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f) posX += cameraSpeed;
		if (::GetAsyncKeyState('G') & 0x8000f) posZ += cameraSpeed;
		if (::GetAsyncKeyState('H') & 0x8000f) posZ -= cameraSpeed;

		//WASD keys
		if (::GetAsyncKeyState('S') & 0x8000f) {

			if (zMove <= 38.0f){
				
				// Legs and arms movement(after a compelete rotate , reset the position)
				if (rotation >= 0.68f || rotation <= -0.68f) rotationSpeed *= -1;
	
				// Distance between wall and human
				if (doorMovement >= 0){
					walls[1] = translationAfterMovement(walls[1], Position(0.0f, -0.05, 0.0f));
					doorMovement -= 0.05;
				}
				rotation += rotationSpeed;
				animation(rotationSpeed);
				zMoveFunction(1.0f);
			}
		}
		if (::GetAsyncKeyState('W') & 0x8000f){
			if (zMove >= -38.0f){
				// Legs and arms movement(after a compelete rotate , reset the position)
				if (rotation >= 0.68f || rotation <= -0.68f) rotationSpeed *= -1;
				
				// If human gets close to door , door opens
				if (zMove <= -20.0f){
					doorMovement += 0.03;
					walls[1] = translationAfterMovement(walls[1], Position(0.0f, 0.2, 0.0f));
				}
				rotation += rotationSpeed;
				animation(rotationSpeed);
				zMoveFunction(-1.0f);

			}

		}
		if (::GetAsyncKeyState('A') & 0x8000f){
			if (xMove >= -38.0f) xMoveFunction(-1.0f); // Left path
		}
		if (::GetAsyncKeyState('D') & 0x8000f) {
			if (xMove <= 38.0f) xMoveFunction(1.0f); // Right path
		}
		
			
		D3DXVECTOR3 position(posX, posY, posZ);
		D3DXVECTOR3 target(targetX , targetY , targetZ);

		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXMATRIX V;
		D3DXMatrixLookAtLH(&V, &position, &target, &up);

		Device->SetTransform(D3DTS_VIEW, &V);

		// Draw the scene
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0x00000000, 1.0f, 0);
		Device->BeginScene();
		Floor->draw(&SkyFloor[0], &Mtrls[6], floorTexture);
		Sky->draw(&SkyFloor[1], &Mtrls[6], skyTexture);
		backWallTexture->draw(&walls[0], &Mtrls[6], wallsTexture);
		frontWallTexture->draw(&walls[1], &Mtrls[6], wallsTexture);
		leftWallTexture->draw(&walls[2], &Mtrls[6], wallsTexture);
		rightWallTexture->draw(&walls[3], &Mtrls[6], wallsTexture);
		

		for (int i = 0; i < NumOfObj; i++){
			// set material and world matrix for ith object, then render
			// the ith object.
			Device->SetMaterial(&Mtrls[i]);
			Device->SetTransform(D3DTS_WORLD, &Worlds[i]);
			Objects[i]->DrawSubset(0);
		}
		
		RenderShadow();
		drawXFile(carMesh , meshMtrls , Textures , carMatrix);

		//Text display
		D3DXMatrixTranslation(&textMatrix, -10.0f, 3.50f, 2.0f);
		Device->SetTransform(D3DTS_WORLD, &textMatrix);
		Device->SetMaterial(&d3d::RED_MTRL);
		Text->DrawSubset(0);

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true;
}

bool createMesh(ID3DXMesh*& Mesh, std::vector<D3DMATERIAL9>& Mtrls, std::vector<IDirect3DTexture9*>& Textures, LPCWSTR fileName){
	HRESULT hr = 0;

	//
	// Load the XFile data.
	//

	ID3DXBuffer* adjBuffer = 0;
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD        numMtrls = 0;


	hr = D3DXLoadMeshFromX(
		fileName,
		D3DXMESH_MANAGED,
		Device,
		&adjBuffer,
		&mtrlBuffer,
		0,
		&numMtrls,
		&Mesh);

	if (FAILED(hr)){
		::MessageBox(0, L"D3DXLoadMeshFromX() - FAILED", 0, 0);
		return false;
	}
	//
	// Extract the materials, and load textures.
	//

	if (mtrlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for (int i = 0; i < numMtrls; i++)
		{
			// the MatD3D property doesn't have an ambient value set
			// when its loaded, so set it now:
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			// save the ith material
			Mtrls.push_back(mtrls[i].MatD3D);

			// check if the ith material has an associative texture
			if (mtrls[i].pTextureFilename != 0)
			{
				// yes, load the texture for the ith subset
				IDirect3DTexture9* tex = 0;
				D3DXCreateTextureFromFile(
					Device,
					(LPCWSTR)mtrls[i].pTextureFilename,
					&tex);

				// save the loaded texture
				Textures.push_back(tex);
			}
			else
			{
				// no texture for the ith subset
				Textures.push_back(0);
			}
		}
	}
	d3d::Release<ID3DXBuffer*>(mtrlBuffer); // done w/ buffer

	//
	// Optimize the mesh.
	//

	hr = Mesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)adjBuffer->GetBufferPointer(),
		0, 0, 0);

	d3d::Release<ID3DXBuffer*>(adjBuffer); // done w/ buffer

	if (FAILED(hr))
	{
		::MessageBox(0, L"OptimizeInplace() - FAILED", 0, 0);
		return false;
	}
}

void drawXFile(ID3DXMesh* Mesh, std::vector<D3DMATERIAL9> Mtrls, std::vector<IDirect3DTexture9*> Textures, D3DXMATRIX World){
	for (int i = 0; i < Mtrls.size(); i++){
		Device->SetMaterial(&Mtrls[i]);
		Device->SetTexture(0, Textures[i]);
		Device->SetTransform(D3DTS_WORLD, &World);
		Mesh->DrawSubset(i);
	}
}
void cleanupXFile(ID3DXMesh* Mesh, std::vector<IDirect3DTexture9*> Textures){
	d3d::Release<ID3DXMesh*>(Mesh);

	for (int i = 0; i < Textures.size(); i++)
		d3d::Release<IDirect3DTexture9*>(Textures[i]);
}

bool createTextFunction(){
	//
	// Get a handle to a device context.
	//
	HDC hdc = CreateCompatibleDC(0);
	HFONT hFont;
	HFONT hFontOld;

	//
	// Describe the font we want.
	//

	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT));

	lf.lfHeight = 25;    // in logical units
	lf.lfWidth = 12;    // in logical units
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = 800;   // boldness, range 0(light) - 1000(bold)
	lf.lfItalic = false;
	lf.lfUnderline = false;
	lf.lfStrikeOut = false;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = 0;
	lf.lfClipPrecision = 0;
	lf.lfQuality = 0;
	lf.lfPitchAndFamily = 0;
	//strcpy(lf.lfFaceName, "Times New Roman"); // font style

	//
	// Create the font and select it with the device context.
	//
	hFont = CreateFontIndirect(&lf);
	hFontOld = (HFONT)SelectObject(hdc, hFont);

	//
	// Create the text mesh based on the selected font in the HDC.
	//
	D3DXCreateText(Device, hdc, L"Hossein Mohammadkhani",
		0.001f, 0.4f, &Text, 0, 0);

	//
	// Restore the old font and free the acquired HDC.
	//
	SelectObject(hdc, hFontOld);
	DeleteObject(hFont);
	DeleteDC(hdc);

	return true;
}

void RenderShadow()
{
	Device->SetRenderState(D3DRS_STENCILENABLE, true);
	Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	Device->SetRenderState(D3DRS_STENCILREF, 0x0);
	Device->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	Device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	Device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	Device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR); // increment to 1

	// position shadow

	D3DXVECTOR4 lightDirection(2.0f, 2.0f, 2.0f, 0.0f);
	D3DXPLANE groundPlane(0.0f, 1.0f, 0.0f, 0.0f);

	D3DXMATRIX S, translation, W;
	D3DXMatrixShadow(
		&S,
		&lightDirection,
		&groundPlane);
	D3DXMatrixTranslation(&translation, -1.60f, -1.80f, -1.75f);
	S *= translation;

	// alpha blend the shadow
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	D3DMATERIAL9 mtrl = d3d::InitMtrl(d3d::BLACK, d3d::BLACK, d3d::BLACK, d3d::BLACK, 0.0f);
	mtrl.Diffuse.a = 0.01f; // 50% transparency.
	mtrl.Ambient.a = 0.5f;
	// Disable depth buffer so that z-fighting doesn't occur when we
	// render the shadow on top of the floor.
	Device->SetRenderState(D3DRS_ZENABLE, false);

	for (int i = 0; i < NumOfObj; i++)
	{
		W = Worlds[i] * S;
		Device->SetTransform(D3DTS_WORLD, &W);
		Device->SetMaterial(&mtrl);
		Objects[i]->DrawSubset(0);
	}


	Device->SetRenderState(D3DRS_ZENABLE, true);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	Device->SetRenderState(D3DRS_STENCILENABLE, false);
}

// WndProc
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hwnd);
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

// WinMain
int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd)
{
	if (!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, L"InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if (!Setup())
	{
		::MessageBox(0, L"Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop(Display);

	Cleanup();

	Device->Release();

	return 0;
}