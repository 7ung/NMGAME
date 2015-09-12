#include "Texture.h"
US_FRAMEWORK

Texture::Texture(void)
{
	_color = C_WHITE;
	_texture = nullptr;		// nullptr have the same meaning NULL.
}

Texture::~Texture(void)
{
	//_texture->Release();
	// do no thing. use release instead.
}

HRESULT Texture::loadFromFile(LPD3DXSPRITE spriteHandle, LPWSTR filePath, D3DXCOLOR color)
{
	HRESULT			result;

	result = D3DXGetImageInfoFromFile(filePath, &this->_imageInfo);
	if (result != D3D_OK)
	{
		return result;
	}

	LPDIRECT3DDEVICE9 _device = DeviceManager::getInstance()->getDevice();
	spriteHandle->GetDevice(&_device);

	result = D3DXCreateTextureFromFileEx(
		_device,
		filePath,
		this->_imageInfo.Width,
		this->_imageInfo.Height,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		color,
		&this->_imageInfo,
		nullptr,
		&this->_texture);
	
	_color = color;

	return result;
}
void Texture::release()
{
	this->_texture->Release();
}
void Texture::render(LPD3DXSPRITE spriteHandle, const RECT* rect, const GVector3* center, const GVector3* position)
{
	spriteHandle->Draw(
		this->_texture,
		rect,
		center,
		position,
		_color);
}

void Texture::render(LPD3DXSPRITE spriteHandle, RECT * srcRect, GVector2 position, GVector2 scale, float rotate, GVector2 origin, int zIndex)
{
	D3DXMATRIX matFinal;
	D3DXMATRIX matTransformed;
	D3DXMATRIX matOld;

	// origin in position
	GVector3 center = GVector3(abs(srcRect->right - srcRect->left) * origin.x, abs(srcRect->top - srcRect->bottom) * origin.x, zIndex);
	
	// get matrix texture
	spriteHandle->GetTransform(&matOld);

	D3DXMatrixTransformation2D(
		&matTransformed,						// ma tran ket qua sau transform
		&position,						// goc toa do / diem neo
		0.0f,									
		&scale,									// ti le scale
		&position,						// goc toa do / diem neo
		D3DXToRadian(rotate),					// g�c xoay theo radian
		0										// vi tr�, b�n d??i c?p nh?t v? tr� r?i
		);

	matFinal = matTransformed * matOld;

	//set matrix transformed
	spriteHandle->SetTransform(&matFinal);

	spriteHandle->Draw(
		this->_texture,
		srcRect,
		&center,
		&GVector3(position.x, position.y, zIndex),
		_color);

	spriteHandle->SetTransform(&matOld);
}

void Texture::render(LPD3DXSPRITE spriteHandle, RECT * srcRect, Viewport viewport, GVector2 position, GVector2 scale, float rotate, GVector2 origin, int zIndex)
{
	D3DXMATRIX matFinal;
	D3DXMATRIX matTransformed;
	D3DXMATRIX matOld;

	// world position to viewport
	D3DXMATRIX mt;
	D3DXMatrixIdentity(&mt);
	mt._22 = -1.0f;
	mt._41 = (-1) * viewport.getPositionWorld().x;
	mt._42 = viewport.getPositionWorld().y;
	D3DXVECTOR4 positionViewport;
	D3DXVec3Transform(&positionViewport, &GVector3(position.x, position.y, zIndex), &mt);

	// origin in position
	GVector3 center = GVector3(abs(srcRect->right - srcRect->left) * origin.x, abs(srcRect->top - srcRect->bottom) * origin.x, zIndex);
	GVector2 originPosition = GVector2(positionViewport.x, positionViewport.y);

	// get matrix texture
	spriteHandle->GetTransform(&matOld);

	D3DXMatrixTransformation2D(
		&matTransformed,						// ma tran ket qua sau transform
		&originPosition,						// goc toa do / diem neo
		0.0f,
		&scale,									// ti le scale
		&originPosition,						// goc toa do / diem neo
		D3DXToRadian(rotate),					// g�c xoay theo radian
		0										// vi tr�, b�n d??i c?p nh?t v? tr� r?i
		);

	matFinal = matTransformed * matOld;

	//set matrix transformed
	spriteHandle->SetTransform(&matFinal);

	spriteHandle->Draw(
		this->_texture,
		srcRect,
		&center,
		&GVector3(positionViewport.x, positionViewport.y, zIndex),
		_color);

	spriteHandle->SetTransform(&matOld);
}

void Texture::setColor(D3DXCOLOR color)
{
	this->_color = color;
}

int Texture::getWidth()
{
	return _imageInfo.Width;
}

int Texture::getHeight()
{
	return _imageInfo.Height;
}