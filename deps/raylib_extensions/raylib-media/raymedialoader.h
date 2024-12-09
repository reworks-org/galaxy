#pragma once

#include "raymedia.h"

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

// This is an example structure representing a memory buffer with a read position
typedef struct MediaMemoryStream
{
	uint8_t* data; // Pointer to the memory buffer
	int64_t  size; // Total size of the buffer in bytes
	int64_t  pos;  // Current read position within the buffer
} MediaMemoryStream;

// This custom read function reads data from a MediaMemoryStream
// @param userData Pointer to the user-provided MediaMemoryStream
// @param buf Pointer to the destination buffer where data will be read into
// @param bufSize Number of bytes to read
// @return The number of bytes successfully read or MEDIA_IO_EOF on end of stream
extern inline int memoryRead(void* userData, uint8_t* buf, int bufSize)
{
	MediaMemoryStream* ctx = (MediaMemoryStream*)userData;

	if (ctx->pos >= ctx->size)
	{
		return MEDIA_IO_EOF; // End of file
	}

	int64_t bytesToRead = bufSize;

	if (ctx->pos + bytesToRead > ctx->size)
	{
		bytesToRead = ctx->size - ctx->pos;
	}

	memcpy(buf, ctx->data + ctx->pos, bytesToRead);

	ctx->pos += bytesToRead;

	return (int)bytesToRead;
}

// This custom seek function modifies the read position in the MediaMemoryStream
// @param userData Pointer to the user-provided MediaMemoryStream
// @param offset Offset to apply based on whence
// @param whence Reference position for the offset (SEEK_SET, SEEK_CUR, SEEK_END)
// @return The new position in the stream or MEDIA_IO_INVALID on error
extern inline int64_t memorySeek(void* userData, int64_t offset, int whence)
{
	MediaMemoryStream* ctx = (MediaMemoryStream*)userData;

	int64_t newPos;

	switch (whence)
	{
		case SEEK_SET:
			newPos = offset;
			break;
		case SEEK_CUR:
			newPos = ctx->pos + offset;
			break;
		case SEEK_END:
			newPos = ctx->size + offset;
			break;
		default:
			return MEDIA_IO_INVALID;
	}

	if (newPos < 0 || newPos > (int64_t)ctx->size)
	{
		return MEDIA_IO_INVALID; // Invalid seek
	}

	ctx->pos = (int64_t)newPos;

	return newPos;
}

extern inline MediaStreamReader MediaGetMemoryReader(MediaMemoryStream* stream)
{
	MediaStreamReader reader = {0};
	reader.readFn            = &memoryRead;
	reader.seekFn            = &memorySeek;
	reader.userData          = stream;

	return reader;
}
