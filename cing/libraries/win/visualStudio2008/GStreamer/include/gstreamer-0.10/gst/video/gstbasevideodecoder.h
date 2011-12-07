/* GStreamer
 * Copyright (C) 2008 David Schleef <ds@schleef.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef _GST_BASE_VIDEO_DECODER_H_
#define _GST_BASE_VIDEO_DECODER_H_

#ifndef GST_USE_UNSTABLE_API
#warning "GstBaseVideoDecoder is unstable API and may change in future."
#warning "You can define GST_USE_UNSTABLE_API to avoid this warning."
#endif

#include <gst/video/gstbasevideocodec.h>

G_BEGIN_DECLS

#define GST_TYPE_BASE_VIDEO_DECODER \
  (gst_base_video_decoder_get_type())
#define GST_BASE_VIDEO_DECODER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_BASE_VIDEO_DECODER,GstBaseVideoDecoder))
#define GST_BASE_VIDEO_DECODER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_BASE_VIDEO_DECODER,GstBaseVideoDecoderClass))
#define GST_BASE_VIDEO_DECODER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS((obj),GST_TYPE_BASE_VIDEO_DECODER,GstBaseVideoDecoderClass))
#define GST_IS_BASE_VIDEO_DECODER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_BASE_VIDEO_DECODER))
#define GST_IS_BASE_VIDEO_DECODER_CLASS(obj) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_BASE_VIDEO_DECODER))

/**
 * GST_BASE_VIDEO_DECODER_SINK_NAME:
 *
 * The name of the templates for the sink pad.
 */
#define GST_BASE_VIDEO_DECODER_SINK_NAME    "sink"
/**
 * GST_BASE_VIDEO_DECODER_SRC_NAME:
 *
 * The name of the templates for the source pad.
 */
#define GST_BASE_VIDEO_DECODER_SRC_NAME     "src"

/**
 * GST_BASE_VIDEO_DECODER_FLOW_NEED_DATA:
 *
 * Custom GstFlowReturn value indicating that more data is needed.
 */
#define GST_BASE_VIDEO_DECODER_FLOW_NEED_DATA GST_FLOW_CUSTOM_SUCCESS


typedef struct _GstBaseVideoDecoder GstBaseVideoDecoder;
typedef struct _GstBaseVideoDecoderClass GstBaseVideoDecoderClass;

struct _GstBaseVideoDecoder
{
  GstBaseVideoCodec base_video_codec;

  /*< private >*/
  GstAdapter *input_adapter;
  GstAdapter *output_adapter;

  GList *frames;

  gboolean have_sync;
  gboolean discont;
  gboolean started;

  GstVideoState state;

  gboolean sink_clipping;

  guint64 presentation_frame_number;
  guint64 system_frame_number;

  GstCaps *caps;
  gboolean have_src_caps;

  GstVideoFrame *current_frame;

  int distance_from_sync;
  int reorder_depth;

  GstClockTime buffer_timestamp;

  GstClockTime timestamp_offset;

  gdouble proportion;
  GstClockTime earliest_time;

  GstBuffer *codec_data;

  guint64 offset;
  GstClockTime last_timestamp;

  GstClockTime last_sink_timestamp;
  GstClockTime last_sink_offset_end;
  guint64 base_picture_number;

  int field_index;
};

struct _GstBaseVideoDecoderClass
{
  GstBaseVideoCodecClass base_video_codec_class;

  gboolean (*set_sink_caps) (GstBaseVideoDecoder *coder, GstCaps *caps);
  gboolean (*start) (GstBaseVideoDecoder *coder);
  gboolean (*stop) (GstBaseVideoDecoder *coder);
  gboolean (*reset) (GstBaseVideoDecoder *coder);
  int (*scan_for_sync) (GstBaseVideoDecoder *decoder, gboolean at_eos,
      int offset, int n);
  GstFlowReturn (*parse_data) (GstBaseVideoDecoder *decoder, gboolean at_eos);
  gboolean (*finish) (GstBaseVideoDecoder *coder, GstVideoFrame *frame);
  GstFlowReturn (*handle_frame) (GstBaseVideoDecoder *coder, GstVideoFrame *frame);
  GstFlowReturn (*shape_output) (GstBaseVideoDecoder *coder, GstVideoFrame *frame);
  GstCaps *(*get_caps) (GstBaseVideoDecoder *coder);
};

GType gst_base_video_decoder_get_type (void);

int gst_base_video_decoder_get_width (GstBaseVideoDecoder *coder);
int gst_base_video_decoder_get_height (GstBaseVideoDecoder *coder);

guint64 gst_base_video_decoder_get_timestamp_offset (GstBaseVideoDecoder *coder);

GstVideoFrame *gst_base_video_decoder_get_frame (GstBaseVideoDecoder *coder,
    int frame_number);
void gst_base_video_decoder_add_to_frame (GstBaseVideoDecoder *base_video_decoder,
    int n_bytes);
GstFlowReturn gst_base_video_decoder_finish_frame (GstBaseVideoDecoder *base_video_decoder,
    GstVideoFrame *frame);
GstFlowReturn gst_base_video_decoder_end_of_stream (GstBaseVideoDecoder *base_video_decoder,
    GstBuffer *buffer);
GstFlowReturn
gst_base_video_decoder_have_frame (GstBaseVideoDecoder *base_video_decoder);
GstVideoState * gst_base_video_decoder_get_state (GstBaseVideoDecoder *base_video_decoder);
void gst_base_video_decoder_set_state (GstBaseVideoDecoder *base_video_decoder,
    GstVideoState *state);
void gst_base_video_decoder_lost_sync (GstBaseVideoDecoder *base_video_decoder);
void gst_base_video_decoder_set_sync_point (GstBaseVideoDecoder *base_video_decoder);

void gst_base_video_decoder_set_src_caps (GstBaseVideoDecoder *base_video_decoder);


G_END_DECLS

#endif

