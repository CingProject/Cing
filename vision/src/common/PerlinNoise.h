/*
This source file is part of the Vision project
For the latest info, see http://www.playthemagic.com/vision

Copyright (c) 2008 Julio Obelleiro and Jorge Cano

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

// Code from:
// http://www.flipcode.com/archives/Perlin_Noise_Class.shtml

#ifndef PerlinNoise_H_
#define PerlinNoise_H_

#include "CommonPrereqs.h"
#include "CommonTypes.h"

#include <stdlib.h>

namespace Common
{

#define SAMPLE_SIZE 1024

	class PerlinNoise
	{
	public:

		PerlinNoise(int octaves,float freq,float amp,int seed);


		inline float get(float x,float y)
		{
			float vec[2];
			vec[0] = x;
			vec[1] = y;
			return perlin_noise_2D(vec);
		};

	private:
		void  init_perlin(int n,float p);
		float perlin_noise_2D(float vec[2]);

		float noise1(float arg);
		float noise2(float vec[2]);
		float noise3(float vec[3]);
		void  normalize2(float v[2]);
		void  normalize3(float v[3]);
		void  init(void);

		int   mOctaves;
		float mFrequency;
		float mAmplitude;
		int   mSeed;

		int p[SAMPLE_SIZE + SAMPLE_SIZE + 2];
		float g3[SAMPLE_SIZE + SAMPLE_SIZE + 2][3];
		float g2[SAMPLE_SIZE + SAMPLE_SIZE + 2][2];
		float g1[SAMPLE_SIZE + SAMPLE_SIZE + 2];
		bool  mStart;

	};
} // namespace Common

#endif  //PerlinNoise_H_
