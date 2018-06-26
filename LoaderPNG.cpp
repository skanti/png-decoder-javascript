#include <emscripten/bind.h>
#include "png.h"

using namespace emscripten;

void load(std::string filename, unsigned char *image, int width, int height, int n_channels) {
	FILE *fp = fopen(filename.c_str(), "rb");

	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) throw std::runtime_error("png_create_read_struct failed\n");

	png_infop info = png_create_info_struct(png);
	if (!info) throw std::runtime_error("png_create_info_struct failed\n");

	if (setjmp(png_jmpbuf(png))) throw std::runtime_error("png_jmpbuf error.\n");;

	png_init_io(png, fp);

	png_read_info(png, info);

	int w = png_get_image_width(png, info);
	int h = png_get_image_height(png, info);
	int c = png_get_channels(png, info);
	auto color_type = png_get_color_type(png, info);
	auto bit_depth = png_get_bit_depth(png, info);

	assert(width == w);
	assert(height == h);
	assert(n_channels == c);

	// if 16 bit, strip down to 8 bit
	//if (bit_depth == 16)
	//	png_set_strip_16(png);

	if (bit_depth == 16)
		png_set_swap(png);

	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png);

	// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png);

	if (png_get_valid(png, info, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png);

	// These color_type don't have an alpha channel then fill it with 0xff.
	//if (color_type == PNG_COLOR_TYPE_RGB ||
	//	color_type == PNG_COLOR_TYPE_GRAY ||
	//	color_type == PNG_COLOR_TYPE_PALETTE)
	//	png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

	//if (color_type == PNG_COLOR_TYPE_GRAY ||
	//	color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
	//	png_set_gray_to_rgb(png);

	png_read_update_info(png, info);

	int n_bytes_per_row = png_get_rowbytes(png, info);
	png_bytep *row_pointers = (png_bytep *) malloc(sizeof(png_bytep)*height);
	for (int y = 0; y < height; y++) {
		row_pointers[y] = (png_byte *) malloc(n_bytes_per_row);
	}
	png_read_image(png, row_pointers);

	// flatting and putting into a contiguous std::vector
	for (int y = 0; y < height; y++) {
		memcpy(image + y*n_bytes_per_row, row_pointers[y], n_bytes_per_row);
		free(row_pointers[y]);
	}

	fclose(fp);
}

EMSCRIPTEN_BINDINGS(my_module) {
	function("load", &load, allow_raw_pointers());
}



