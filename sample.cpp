#include <stdlib.h>
#include <stdio.h>
#include "jpeglib.h"

#define IMAGE_WIDTH  1280
#define IMAGE_HEIGHT 720
#define BYTE_PER_PIXEL 3

void drawColorBar(uint8_t *imgRGB, uint32_t width, uint32_t height)
{
	for (uint32_t y = 0; y < height / 2; y++) {
		for (uint32_t x = 0; x < width / 4; x++) { *imgRGB++ = 0xFF; *imgRGB++ = 0x00; *imgRGB++ = 0x00;}
		for (uint32_t x = 0; x < width / 4; x++) { *imgRGB++ = 0x00; *imgRGB++ = 0xFF; *imgRGB++ = 0x00;}
		for (uint32_t x = 0; x < width / 4; x++) { *imgRGB++ = 0x00; *imgRGB++ = 0x00; *imgRGB++ = 0xFF;}
		for (uint32_t x = 0; x < width / 4; x++) { *imgRGB++ = 0xFF; *imgRGB++ = 0xFF; *imgRGB++ = 0xFF;}
	}
	for (uint32_t y = 0; y < height / 2; y++) {
		for (uint32_t x = 0; x < width / 4; x++) { *imgRGB++ = 0xFF; *imgRGB++ = 0xFF; *imgRGB++ = 0xFF;}
		for (uint32_t x = 0; x < width / 4; x++) { *imgRGB++ = 0x88; *imgRGB++ = 0x88; *imgRGB++ = 0x88;}
		for (uint32_t x = 0; x < width / 4; x++) { *imgRGB++ = 0x44; *imgRGB++ = 0x44; *imgRGB++ = 0x44;}
		for (uint32_t x = 0; x < width / 4; x++) { *imgRGB++ = 0x00; *imgRGB++ = 0x00; *imgRGB++ = 0x00;}
	}
}

int encodeJpegMem(uint8_t *imgJpeg, uint8_t *imgRGB, uint32_t width, uint32_t height, uint32_t imgJpegSize)
{
	JSAMPROW lineBuffer[1] = {0};

	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	unsigned long writtenSize = imgJpegSize;
	jpeg_mem_dest(&cinfo, &imgJpeg, &writtenSize);

	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = BYTE_PER_PIXEL;
	cinfo.in_color_space = JCS_RGB;
	jpeg_set_defaults(&cinfo);
	cinfo.dct_method = JDCT_IFAST;
//	jpeg_set_quality(&cinfo, 10, TRUE);
	jpeg_start_compress(&cinfo, TRUE);
	for (uint32_t y = 0; y < height; y++) {
		lineBuffer[0] = imgRGB;
		imgRGB += width * BYTE_PER_PIXEL;
		jpeg_write_scanlines(&cinfo, lineBuffer, 1);
	}
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);

	return (imgJpegSize - cinfo.dest->free_in_buffer);
}

void jpegEncodeTest()
{
	/* Create the original image (1280 x 720 x RGB(24bit)) as color bar */
	static uint8_t s_imageRGB[IMAGE_WIDTH * IMAGE_HEIGHT * BYTE_PER_PIXEL];
	drawColorBar(s_imageRGB, IMAGE_WIDTH, IMAGE_HEIGHT);

	/* Encode jpeg and save it to memory */
	static uint8_t s_imageJEPG[IMAGE_WIDTH * IMAGE_HEIGHT / 4];
	uint32_t jpegSize;
	jpegSize = encodeJpegMem(s_imageJEPG, s_imageRGB, IMAGE_WIDTH, IMAGE_HEIGHT, IMAGE_WIDTH * IMAGE_HEIGHT / 4);
}
