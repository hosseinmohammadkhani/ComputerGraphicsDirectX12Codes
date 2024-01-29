#include <d3dx9.h>
#include <string>

struct Vertex{
	Vertex() {}
	Vertex(
		float x, float y, float z,
		float nx, float ny, float nz,
		float u, float v){
		_x = x;  _y = y;  _z = z;
		_nx = nx; _ny = ny; _nz = nz;
		_u = u;  _v = v;
	}
	float _x, _y, _z;
	float _nx, _ny, _nz;
	float _u, _v; 
};
#define FVF_VERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)
class Texture{
public:
	Texture(IDirect3DDevice9* device);


	bool draw(D3DXMATRIX* world, D3DMATERIAL9* mtrl, IDirect3DTexture9* tex);
private:
	IDirect3DDevice9* _device;
	IDirect3DVertexBuffer9* _vb;
	IDirect3DIndexBuffer9* _ib;
};

