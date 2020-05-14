#pragma once
struct AVStream;
struct AVCodecContext;
struct AVCodec;

struct OutputStream
{
	struct AVStream *st;
	struct AVCodecContext *enc;
	struct AVCodec *codec;
	int streamIdx_in;
	int streamIdx_out;
};
