#include "Texture.h"


Texture::Texture(IDirect3DDevice9* device)
{
	_device = device;

	_device->CreateVertexBuffer(
		4 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX,
		D3DPOOL_MANAGED,
		&_vb,
		0);


	Vertex* v;
	_vb->Lock(0, 0, (void**)&v, 0);

	// build plane

	// fill in the front face vertex data
	v[0] = Vertex(-40.0f, -40.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[1] = Vertex(-40.0f, 40.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 5.0f);
	v[2] = Vertex(40.0f, 40.0f, -1.0f, 0.0f, 0.0f, -1.0f, 5.0f, 5.0f);
	v[3] = Vertex(40.0f, -40.0f, -1.0f, 0.0f, 0.0f, -1.0f, 5.0f, 0.0f);
	_vb->Unlock();
	_device->CreateIndexBuffer(
		6 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&_ib,
		0);
	WORD* i = 0;
	_ib->Lock(0, 0, (void**)&i, 0);

	// fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;
	_ib->Unlock();
}

bool Texture::draw(D3DXMATRIX* world, D3DMATERIAL9* mtrl, IDirect3DTexture9* tex)
{

	if (world)
		_device->SetTransform(D3DTS_WORLD, world);
	if (mtrl)
		_device->SetMaterial(mtrl);
	if (tex)
		_device->SetTexture(0, tex);

	_device->SetStreamSource(0, _vb, 0, sizeof(Vertex));
	_device->SetIndices(_ib);
	_device->SetFVF(FVF_VERTEX);
	_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
		0,
		0,
		6, //24
		0,
		2);
	return true;
}

