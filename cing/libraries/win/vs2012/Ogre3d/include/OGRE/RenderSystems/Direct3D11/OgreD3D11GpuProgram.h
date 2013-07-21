/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2013 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef __D3D11GpuProgram_H_
#define __D3D11GpuProgram_H_

// Precompiler options
#include "OgreD3D11Prerequisites.h"
#include "OgreGpuProgram.h"

namespace Ogre {

	/** Direct3D implementation of a few things common to low-level vertex & fragment programs. */
	class D3D11GpuProgram : public GpuProgram
	{
	protected:
		D3D11Device & mDevice;
	public:
		D3D11GpuProgram(ResourceManager* creator, const String& name, ResourceHandle handle,
			const String& group, bool isManual, ManualResourceLoader* loader, D3D11Device & device);

	protected:
		/** @copydoc Resource::loadImpl */
		void loadImpl(void);
		/** Overridden from GpuProgram */
		void loadFromSource(void);
		/** Internal method to load from microcode, must be overridden by subclasses. */
		virtual void loadFromMicrocode(ID3D10Blob *  microcode) = 0;


	};

	/** Direct3D implementation of low-level vertex programs. */
	class D3D11GpuVertexProgram : public D3D11GpuProgram
	{
	protected:
		ID3D11VertexShader * mVertexShader;
	public:
		D3D11GpuVertexProgram(ResourceManager* creator, const String& name, ResourceHandle handle,
			const String& group, bool isManual, ManualResourceLoader* loader, D3D11Device & device);
		~D3D11GpuVertexProgram();
		/// Gets the vertex shader
		ID3D11VertexShader * getVertexShader(void) const;
	protected:
		/** @copydoc Resource::unloadImpl */
		void unloadImpl(void);
		void loadFromMicrocode(ID3D10Blob *  microcode);
	};

	/** Direct3D implementation of low-level fragment programs. */
	class D3D11GpuFragmentProgram : public D3D11GpuProgram
	{
	protected:
		ID3D11PixelShader * mPixelShader;
	public:
		D3D11GpuFragmentProgram(ResourceManager* creator, const String& name, ResourceHandle handle,
			const String& group, bool isManual, ManualResourceLoader* loader, D3D11Device & device);
		~D3D11GpuFragmentProgram();
		/// Gets the pixel shader
		ID3D11PixelShader * getPixelShader(void) const;
	protected:
		/** @copydoc Resource::unloadImpl */
		void unloadImpl(void);
		void loadFromMicrocode(ID3D10Blob *  microcode);
	};

	/** Direct3D implementation of low-level vertex programs. */
	class D3D11GpuDomainProgram : public D3D11GpuProgram
	{
	protected:
		ID3D11DomainShader * mDomainShader;
	public:
		D3D11GpuDomainProgram(ResourceManager* creator, const String& name, ResourceHandle handle,
			const String& group, bool isManual, ManualResourceLoader* loader, D3D11Device & device);
		~D3D11GpuDomainProgram();
		/// Gets the vertex shader
		ID3D11DomainShader * getDomainShader(void) const;
	protected:
		/** @copydoc Resource::unloadImpl */
		void unloadImpl(void);
		void loadFromMicrocode(ID3D10Blob *  microcode);
	};

	/** Direct3D implementation of low-level vertex programs. */
	class D3D11GpuHullProgram : public D3D11GpuProgram
	{
	protected:
		ID3D11HullShader * mHullShader;
	public:
		D3D11GpuHullProgram(ResourceManager* creator, const String& name, ResourceHandle handle,
			const String& group, bool isManual, ManualResourceLoader* loader, D3D11Device & device);
		~D3D11GpuHullProgram();
		/// Gets the vertex shader
		ID3D11HullShader * getHullShader() const;
	protected:
		/** @copydoc Resource::unloadImpl */
		void unloadImpl(void);
		void loadFromMicrocode(ID3D10Blob *  microcode);
	};


	/** 
		Direct3D implementation of low-level geometry programs. 
		Added due to need to accept geometry programs came from other profiles (nvgp4, for example)
	*/
	class D3D11GpuGeometryProgram : public D3D11GpuProgram
	{
	protected:
		ID3D11GeometryShader * mGeometryShader;
	public:
		D3D11GpuGeometryProgram(ResourceManager* creator, const String& name, ResourceHandle handle,
			const String& group, bool isManual, ManualResourceLoader* loader, D3D11Device & device);
		~D3D11GpuGeometryProgram();
		/// Gets the geometry shader
		ID3D11GeometryShader * getGeometryShader(void) const;
	protected:
		/** @copydoc Resource::unloadImpl */
		void unloadImpl(void);
		void loadFromMicrocode(ID3D10Blob *  microcode);
	};

	/** Specialisation of SharedPtr to allow SharedPtr to be assigned to D3D11GpuProgramPtr 
	@note Has to be a subclass since we need operator=.
	We could templatise this instead of repeating per Resource subclass, 
	except to do so requires a form VC6 does not support i.e.
	ResourceSubclassPtr<T> : public SharedPtr<T>
	*/
	class _OgreExport D3D11GpuProgramPtr : public SharedPtr<D3D11GpuProgram> 
	{
	public:
		D3D11GpuProgramPtr() : SharedPtr<D3D11GpuProgram>() {}
		explicit D3D11GpuProgramPtr(D3D11GpuProgram* rep) : SharedPtr<D3D11GpuProgram>(rep) {}
		D3D11GpuProgramPtr(const D3D11GpuProgramPtr& r) : SharedPtr<D3D11GpuProgram>(r) {} 
		D3D11GpuProgramPtr(const ResourcePtr& r) : SharedPtr<D3D11GpuProgram>()
		{
			// lock & copy other mutex pointer
                    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME);
                    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME);
                    
				pRep = static_cast<D3D11GpuProgram*>(r.getPointer());
			pUseCount = r.useCountPointer();
			if (pUseCount)
			{
				++(*pUseCount);
			}
		}

		/// Operator used to convert a ResourcePtr to a D3D11GpuProgramPtr
		D3D11GpuProgramPtr& operator=(const ResourcePtr& r)
		{
			if (pRep == static_cast<D3D11GpuProgram*>(r.getPointer()))
				return *this;
			release();
			// lock & copy other mutex pointer
			OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME);
                        OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME);
				pRep = static_cast<D3D11GpuProgram*>(r.getPointer());
			pUseCount = r.useCountPointer();
			if (pUseCount)
			{
				++(*pUseCount);
			}
			return *this;
		}
	};

}


#endif
