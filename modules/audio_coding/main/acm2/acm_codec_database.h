/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

/*
 * This file generates databases with information about all supported audio
 * codecs.
 */

#ifndef WEBRTC_MODULES_AUDIO_CODING_MAIN_ACM2_ACM_CODEC_DATABASE_H_
#define WEBRTC_MODULES_AUDIO_CODING_MAIN_ACM2_ACM_CODEC_DATABASE_H_

#include "webrtc/common_types.h"
#include "webrtc/engine_configurations.h"
#include "webrtc/modules/audio_coding/neteq/interface/neteq.h"

namespace webrtc {

namespace acm2 {

// TODO(tlegrand): replace class ACMCodecDB with a namespace.
class ACMCodecDB {
 public:
  // Enum with array indexes for the supported codecs. NOTE! The order MUST
  // be the same as when creating the database in acm_codec_database.cc.
  enum {
    kNone = -1
#if (defined(WEBRTC_CODEC_ISAC) || defined(WEBRTC_CODEC_ISACFX))
    , kISAC
# if (defined(WEBRTC_CODEC_ISAC))
    , kISACSWB
    , kISACFB
# endif
#endif
    // Mono
    , kPCM16B
    , kPCM16Bwb
    , kPCM16Bswb32kHz
    // Stereo
    , kPCM16B_2ch
    , kPCM16Bwb_2ch
    , kPCM16Bswb32kHz_2ch
    // Mono
    , kPCMU
    , kPCMA
    // Stereo
    , kPCMU_2ch
    , kPCMA_2ch
#ifdef WEBRTC_CODEC_ILBC
    , kILBC
#endif
#ifdef WEBRTC_CODEC_G722
    // Mono
    , kG722
    // Stereo
    , kG722_2ch
#endif
#ifdef WEBRTC_CODEC_OPUS
    // Mono and stereo
    , kOpus
#endif
#ifdef WEBRTC_CODEC_G729
    , kG729
#endif
    , kCNNB
    , kCNWB
    , kCNSWB
#ifdef ENABLE_48000_HZ
    , kCNFB
#endif
    , kAVT
#ifdef WEBRTC_CODEC_RED
    , kRED
#endif
    , kNumCodecs
  };

  // Set unsupported codecs to -1
#ifndef WEBRTC_CODEC_ISAC
  enum {kISACSWB = -1};
  enum {kISACFB = -1};
# ifndef WEBRTC_CODEC_ISACFX
  enum {kISAC = -1};
# endif
#endif
  // 48 kHz not supported, always set to -1.
  enum {kPCM16Bswb48kHz = -1};
#ifndef WEBRTC_CODEC_ILBC
  enum {kILBC = -1};
#endif
#ifndef WEBRTC_CODEC_G722
  // Mono
  enum {kG722 = -1};
  // Stereo
  enum {kG722_2ch = -1};
#endif
#ifndef WEBRTC_CODEC_OPUS
  // Mono and stereo
  enum {kOpus = -1};
#endif
#ifndef WEBRTC_CODEC_G729
  enum {kG729 = -1};
#endif
#ifndef WEBRTC_CODEC_RED
  enum {kRED = -1};
#endif
#ifndef ENABLE_48000_HZ
  enum { kCNFB = -1 };
#endif

  // kMaxNumCodecs - Maximum number of codecs that can be activated in one
  //                 build.
  // kMaxNumPacketSize - Maximum number of allowed packet sizes for one codec.
  // These might need to be increased if adding a new codec to the database
  static const int kMaxNumCodecs =  50;
  static const int kMaxNumPacketSize = 6;

  // Codec specific settings
  //
  // num_packet_sizes     - number of allowed packet sizes.
  // packet_sizes_samples - list of the allowed packet sizes.
  // basic_block_samples  - assigned a value different from 0 if the codec
  //                        requires to be fed with a specific number of samples
  //                        that can be different from packet size.
  // channel_support      - number of channels supported to encode;
  //                        1 = mono, 2 = stereo, etc.
  struct CodecSettings {
    int num_packet_sizes;
    int packet_sizes_samples[kMaxNumPacketSize];
    int basic_block_samples;
    int channel_support;
  };

  // Gets codec information from database at the position in database given by
  // [codec_id].
  // Input:
  //   [codec_id] - number that specifies at what position in the database to
  //                get the information.
  // Output:
  //   [codec_inst] - filled with information about the codec.
  // Return:
  //   0 if successful, otherwise -1.
  static int Codec(int codec_id, CodecInst* codec_inst);

  // Returns codec id from database, given the information received in the input
  // [codec_inst].
  // Input:
  //   [codec_inst] - Information about the codec for which we require the
  //                  database id.
  // Return:
  //   codec id if successful, otherwise < 0.
  static int CodecNumber(const CodecInst& codec_inst);
  static int CodecId(const CodecInst& codec_inst);
  static int CodecId(const char* payload_name, int frequency, int channels);
  static int ReceiverCodecNumber(const CodecInst& codec_inst);

  // Returns the codec sampling frequency for codec with id = "codec_id" in
  // database.
  // TODO(tlegrand): Check if function is needed, or if we can change
  // to access database directly.
  // Input:
  //   [codec_id] - number that specifies at what position in the database to
  //                get the information.
  // Return:
  //   codec sampling frequency if successful, otherwise -1.
  static int CodecFreq(int codec_id);

  // Check if the payload type is valid, meaning that it is in the valid range
  // of 0 to 127.
  // Input:
  //   [payload_type] - payload type.
  static bool ValidPayloadType(int payload_type);

  // Databases with information about the supported codecs
  // database_ - stored information about all codecs: payload type, name,
  //             sampling frequency, packet size in samples, default channel
  //             support, and default rate.
  // codec_settings_ - stored codec settings: number of allowed packet sizes,
  //                   a vector with the allowed packet sizes, basic block
  //                   samples, and max number of channels that are supported.
  // neteq_decoders_ - list of supported decoders in NetEQ.
  static const CodecInst database_[kMaxNumCodecs];
  static const CodecSettings codec_settings_[kMaxNumCodecs];
  static const NetEqDecoder neteq_decoders_[kMaxNumCodecs];
};

}  // namespace acm2

}  // namespace webrtc

#endif  // WEBRTC_MODULES_AUDIO_CODING_MAIN_ACM2_ACM_CODEC_DATABASE_H_
