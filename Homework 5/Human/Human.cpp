#include "d3dUtility.h"
#include "Texture.h"

IDirect3DDevice9* Device = 0;

const int Width = 720;
const int Height = 720;

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

// Camera positions variables
float posX = 0.0f;
float posY = 5.0f;
float posZ = -8.0f;
bool method_camera = true;

void lighting() {

	D3DXVECTOR3 dir(2.0f, 2.0f, 2.0f);
	D3DXCOLOR   c = d3d::WHITE;
	D3DLIGHT9 dirLight = d3d::InitDirectionalLight(&dir, &c);
	Device->SetLight(0, &dirLight);
	Device->LightEnable(0, true);

}
void createHuman()
{
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

void createSkyAndFloor()
{
	D3DXMATRIX Rx , InverseRx , helper;
	D3DXMatrixRotationX(&Rx, D3DX_PI * 0.5);
	D3DXMatrixRotationX(&InverseRx, 3 * D3DX_PI * 0.5);
	D3DXMatrixTranslation(&SkyFloor[0], 0.0f, 0.0f, 0.0f); // Floor
	D3DXMatrixTranslation(&SkyFloor[1], 0.0f, 0.0f, 0.0f);// Sky
	SkyFloor[0] *= Rx;
	SkyFloor[1] *= InverseRx;
	D3DXMatrixTranslation(&helper, 0.0f, -1.85f, 0.0f); // Floor translation
	SkyFloor[0] *= helper;
	D3DXMatrixIdentity(&helper);
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
		L"WallTexture.jpg",
		&wallsTexture);
	backWallTexture = new Texture(Device);
	frontWallTexture = new Texture(Device);
	leftWallTexture = new Texture(Device);
	rightWallTexture = new Texture(Device);
}

bool Setup()
{
	createHuman();
	createSkyAndFloor();
	createWalls();

	// Set lighting related render states
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
		D3DX_PI * 0.5, // 45 - degree
		(float)Width / (float)Height,
		1.0f,
		1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);


	return true;
}

void Cleanup()
{
	for (int i = 0; i < NumOfObj; i++)
		d3d::Release<ID3DXMesh*>(Objects[i]);
}

bool Display(float timeDelta){
	if (Device){

		float cameraSpeed = 0.05f;

		if (::GetAsyncKeyState(VK_UP) & 0x8000f) posY += cameraSpeed;
		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f) posY -= cameraSpeed;
		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f) posX -= cameraSpeed;
		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f) posX += cameraSpeed;
		if (::GetAsyncKeyState('G') & 0x8000f) posZ += cameraSpeed;
		if (::GetAsyncKeyState('H') & 0x8000f) posZ -= cameraSpeed;
		
		D3DXVECTOR3 position(posX, posY, posZ);
		D3DXVECTOR3 target(0 , 0 , 0);

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
		

		for (int i = 0; i < NumOfObj; i++)
		{
			// set material and world matrix for ith object, then render
			// the ith object.
			Device->SetMaterial(&Mtrls[i]);
			Device->SetTransform(D3DTS_WORLD, &Worlds[i]);
			Objects[i]->DrawSubset(0);
		}

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true;
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