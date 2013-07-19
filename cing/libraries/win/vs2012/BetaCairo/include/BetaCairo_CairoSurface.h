/*

	BetaCairo a wrapper for the Cairo Graphics library
	Copyright (C) 2005, 2006 Robin Southern http://get.nxogre.org/cetacairo/

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/
#ifndef __betacairo_cairosurface_H__
#define __betacairo_cairosurface_H__

#include "BetaCairo_Include.h"

namespace BetaCairo {

	class _BetaCairoExport CairoSurface {

		friend Drawing;

		protected:

			CairoSurface(float width, float height, PixelFormat format);
			~CairoSurface();

			void __create(float width, float height, PixelFormat format);
			void __destroy();
			
			/// Get's the stride of a PixelFormat
			/// @micro
			/// @return Stride length in bytes.
			unsigned char __getStride(PixelFormat format) {
				//return format == PF_RGB ? 3 : 4; 
				// Some reason stride for PF_RGB and PF_RGBA are both 4 bytes.
				return 4;
			}

		public:

			/// Set Pen Thickness
			void setLineThickness(float thickness, Unit unit = PIXEL);

			/// Set the current scalar;
			void setScalar(Unit unit);

			/// Set font face and size.
			void setFont(std::string font, float size, Unit sizeUnit = PIXEL);

			/// Returns the pixel format
			/// @micro
			/// @return The Format of the texture
			PixelFormat	getPixelFormat() {
				return mPixelFormat;}

			/// Set the current scalar
			/// @micro
			/// @param s New scale
			void	setScalar(float s) {
				mScalar = s;}

			void moveTo(float x, float y) {
				cairo_move_to(mCR, x * mScalar, y * mScalar);}
			
			void paint() {
				cairo_paint(mCR);}

			void paintAlpha(float alpha) {
				cairo_paint_with_alpha(mCR, alpha);}

			void stroke() {
				cairo_stroke(mCR);}

			void strokePreserve() {
				cairo_stroke_preserve(mCR);}

			void lineTo(float x, float y) {
				cairo_line_to(mCR, x * mScalar, y * mScalar);}

			void rectangle(float x, float, float w, float h) {
				cairo_rectangle(mCR, x * mScalar, x * mScalar, w * mScalar, h * mScalar);}

			void setSourcePattern(Pattern pattern) {
				cairo_set_source(mCR, pattern.pattern);}

			void setColour(Ogre::ColourValue colour) {
				cairo_set_source_rgba(mCR, colour.r, colour.g, colour.b, colour.a);}

			void fill() {
				cairo_fill(mCR);}
			
			void fillPreserve() {
				cairo_fill_preserve(mCR);}

			TextExtents getTextExtents(std::string text);
			
			void text(std::string text) {
				cairo_show_text(mCR, text.c_str());}

			void translate(float x, float y) {
				cairo_translate(mCR, x * mScalar,y * mScalar);}

			void saveToPNG(std::string filename) {
				cairo_surface_write_to_png(mSurface, filename.c_str());}

			void setOperator(DrawOperator op) {
				cairo_set_operator(mCR, (cairo_operator_t) op);}

			DrawOperator getOperator() {
				return (DrawOperator) (int) cairo_get_operator(mCR);}

			void arc(float x, float y, float radius, float a, float b) {
				cairo_arc(mCR,x,y,radius, a * 0.017453292519943294f, b * 0.017453292519943294f);}

			void setAntiAliasing(AntiAliasing aa) {
				cairo_set_antialias(mCR, (cairo_antialias_t) aa);}

			void SetLineCap(LineEnds ll) {
				cairo_set_line_cap(mCR, (cairo_line_cap_t) ll);}

			void SetLineJoin(LineJoin lj) {
				cairo_set_line_join(mCR, (cairo_line_join_t) lj);}

			cairo_surface_t* getSurface(){
				return mSurface;
			}

			cairo_t* getCairoT(){
				return mCR;
			}

		protected:

			cairo_surface_t		*mSurface;
			cairo_t				    *mCR;

			unsigned int		mWidth;
			unsigned int		mHeight;
			PixelFormat			mPixelFormat;
			unsigned char		*mImage;
			float				mScalar;

		private:
	
	};

};

#endif