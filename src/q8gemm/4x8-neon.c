/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <arm_neon.h>

#include <qnnpack/q8gemm.h>


void q8gemm_ukernel_4x8__neon(
    size_t mr,
    size_t nr,
    size_t k,
    const uint8_t* restrict a,
    size_t a_stride,
    const uint8_t* restrict b,
    const int32_t* restrict bias,
    uint8_t* restrict c,
    size_t c_stride,
    const union qnnp_conv_quantization_params quantization_params[restrict static 1])
{
  int32x4_t vacc0x0123 = vld1q_s32(bias); bias += 4;
  int32x4_t vacc0x4567 = vld1q_s32(bias);
  int32x4_t vacc1x0123 = vacc0x0123;
  int32x4_t vacc1x4567 = vacc0x4567;
  int32x4_t vacc2x0123 = vacc0x0123;
  int32x4_t vacc2x4567 = vacc0x4567;
  int32x4_t vacc3x0123 = vacc0x0123;
  int32x4_t vacc3x4567 = vacc0x4567;

  const uint8_t* a0 = a;
  const uint8_t* a1 = (const uint8_t*) ((uintptr_t) a0 + a_stride);
  if (mr < 2) {
    a1 = a0;
  }
  const uint8_t* a2 = (const uint8_t*) ((uintptr_t) a1 + a_stride);
  if (mr <= 2) {
    a2 = a1;
  }
  const uint8_t* a3 = (const uint8_t*) ((uintptr_t) a2 + a_stride);
  if (mr != 4) {
    a3 = a2;
  }

  const uint8x8_t va_zero_point = vld1_dup_u8((const uint8_t*) &quantization_params->neon.input_zero_point);
  const uint8x8_t vb_zero_point = vld1_dup_u8((const uint8_t*) &quantization_params->neon.kernel_zero_point);
  for (; k >= 8; k -= 8) {
    const int16x8_t va0 = vreinterpretq_s16_u16(vsubl_u8(vld1_u8(a0), va_zero_point)); a0 += 8;
    const int16x8_t va1 = vreinterpretq_s16_u16(vsubl_u8(vld1_u8(a1), va_zero_point)); a1 += 8;
    const int16x8_t va2 = vreinterpretq_s16_u16(vsubl_u8(vld1_u8(a2), va_zero_point)); a2 += 8;
    const int16x8_t va3 = vreinterpretq_s16_u16(vsubl_u8(vld1_u8(a3), va_zero_point)); a3 += 8;

    {
      const int16x8_t vb01234567 = vreinterpretq_s16_u16(vsubl_u8(vld1_u8(b), vb_zero_point)); b += 8;

      vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vb01234567), vget_low_s16(va0), 0);
      vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vb01234567), vget_low_s16(va0), 0);
      vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vb01234567), vget_low_s16(va1), 0);
      vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vb01234567), vget_low_s16(va1), 0);
      vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vb01234567), vget_low_s16(va2), 0);
      vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vb01234567), vget_low_s16(va2), 0);
      vacc3x0123 = vmlal_lane_s16(vacc3x0123, vget_low_s16(vb01234567), vget_low_s16(va3), 0);
      vacc3x4567 = vmlal_lane_s16(vacc3x4567, vget_high_s16(vb01234567), vget_low_s16(va3), 0);
    }

    {
      const int16x8_t vb01234567 = vreinterpretq_s16_u16(vsubl_u8(vld1_u8(b), vb_zero_point)); b += 8;

      vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vb01234567), vget_low_s16(va0), 1);
      vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vb01234567), vget_low_s16(va0), 1);
      vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vb01234567), vget_low_s16(va1), 1);
      vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vb01234567), vget_low_s16(va1), 1);
      vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vb01234567), vget_low_s16(va2), 1);
      vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vb01234567), vget_low_s16(va2), 1);
      vacc3x0123 = vmlal_lane_s16(vacc3x0123, vget_low_s16(vb01234567), vget_low_s16(va3), 1);
      vacc3x4567 = vmlal_lane_s16(vacc3x4567, vget_high_s16(vb01234567), vget_low_s16(va3), 1);
    }

    {
      const int16x8_t vb01234567 = vreinterpretq_s16_u16(vsubl_u8(vld1_u8(b), vb_zero_point)); b += 8;

      vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vb01234567), vget_low_s16(va0), 2);
      vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vb01234567), vget_low_s16(va0), 2);
      vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vb01234567), vget_low_s16(va1), 2);
      vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vb01234567), vget_low_s16(va1), 2);
      vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vb01234567), vget_low_s16(va2), 2);
      vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vb01234567), vget_low_s16(va2), 2);
      vacc3x0123 = vmlal_lane_s16(vacc3x0123, vget_low_s16(vb01234567), vget_low_s16(va3), 2);
      vacc3x4567 = vmlal_lane_s16(vacc3x4567, vget_high_s16(vb01234567), vget_low_s16(va3), 2);
    }

    {
      const int16x8_t vb01234567 = vreinterpretq_s16_u16(vsubl_u8(vld1_u8(b), vb_zero_point)); b += 8;

      vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vb01234567), vget_low_s16(va0), 3);
      vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vb01234567), vget_low_s16(va0), 3);
      vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vb01234567), vget_low_s16(va1), 3);
      vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vb01234567), vget_low_s16(va1), 3);
      vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vb01234567), vget_low_s16(va2), 3);
      vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vb01234567), vget_low_s16(va2), 3);
      vacc3x0123 = vmlal_lane_s16(vacc3x0123, vget_low_s16(vb01234567), vget_low_s16(va3), 3);
      vacc3x4567 = vmlal_lane_s16(vacc3x4567, vget_high_s16(vb01234567), vget_low_s16(va3), 3);
    }

    {
      const int16x8_t vb01234567 = vreinterpretq_s16_u16(vsubl_u8(vld1_u8(b), vb_zero_point)); b += 8;

      vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vb01234567), vget_high_s16(va0), 0);
      vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vb01234567), vget_high_s16(va0), 0);
      vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vb01234567), vget_high_s16(va1), 0);
      vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vb01234567), vget_high_s16(va1), 0);
      vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vb01234567), vget_high_s16(va2), 0);
      vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vb01234567), vget_high_s16(va2), 0);
      vacc3x0123 = vmlal_lane_s16(vacc3x0123, vget_low_s16(vb01234567), vget_high_s16(va3), 0);
      vacc3x4567 = vmlal_lane_s16(vacc3x4567, vget_high_s16(vb01234567), vget_high_s16(va3), 0);
    }

    {
      const int16x8_t vb01234567 = vreinterpretq_s16_u16(vsubl_u8(vld1_u8(b), vb_zero_point)); b += 8;

      vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vb01234567), vget_high_s16(va0), 1);
      vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vb01234567), vget_high_s16(va0), 1);
      vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vb01234567), vget_high_s16(va1), 1);
      vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vb01234567), vget_high_s16(va1), 1);
      vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vb01234567), vget_high_s16(va2), 1);
      vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vb01234567), vget_high_s16(va2), 1);
      vacc3x0123 = vmlal_lane_s16(vacc3x0123, vget_low_s16(vb01234567), vget_high_s16(va3), 1);
      vacc3x4567 = vmlal_lane_s16(vacc3x4567, vget_high_s16(vb01234567), vget_high_s16(va3), 1);
    }

    {
      const int16x8_t vb01234567 = vreinterpretq_s16_u16(vsubl_u8(vld1_u8(b), vb_zero_point)); b += 8;

      vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vb01234567), vget_high_s16(va0), 2);
      vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vb01234567), vget_high_s16(va0), 2);
      vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vb01234567), vget_high_s16(va1), 2);
      vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vb01234567), vget_high_s16(va1), 2);
      vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vb01234567), vget_high_s16(va2), 2);
      vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vb01234567), vget_high_s16(va2), 2);
      vacc3x0123 = vmlal_lane_s16(vacc3x0123, vget_low_s16(vb01234567), vget_high_s16(va3), 2);
      vacc3x4567 = vmlal_lane_s16(vacc3x4567, vget_high_s16(vb01234567), vget_high_s16(va3), 2);
    }

    {
      const int16x8_t vb01234567 = vreinterpretq_s16_u16(vsubl_u8(vld1_u8(b), vb_zero_point)); b += 8;

      vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vb01234567), vget_high_s16(va0), 3);
      vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vb01234567), vget_high_s16(va0), 3);
      vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vb01234567), vget_high_s16(va1), 3);
      vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vb01234567), vget_high_s16(va1), 3);
      vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vb01234567), vget_high_s16(va2), 3);
      vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vb01234567), vget_high_s16(va2), 3);
      vacc3x0123 = vmlal_lane_s16(vacc3x0123, vget_low_s16(vb01234567), vget_high_s16(va3), 3);
      vacc3x4567 = vmlal_lane_s16(vacc3x4567, vget_high_s16(vb01234567), vget_high_s16(va3), 3);
    }
  }
  if (k != 0) {
    const size_t a_predecrement = 8 - k;
    const int64x1_t va_shift = vmov_n_s64(-8 * a_predecrement);
    const int16x8_t va0 = vreinterpretq_s16_u16(vsubl_u8(
        vreinterpret_u8_u64(vshl_u64(
            vreinterpret_u64_u8(vld1_u8(a0 - a_predecrement)), va_shift)),
        va_zero_point));
    const int16x8_t va1 = vreinterpretq_s16_u16(vsubl_u8(
        vreinterpret_u8_u64(vshl_u64(
            vreinterpret_u64_u8(vld1_u8(a1 - a_predecrement)), va_shift)),
        va_zero_point));
    const int16x8_t va2 = vreinterpretq_s16_u16(vsubl_u8(
        vreinterpret_u8_u64(vshl_u64(
            vreinterpret_u64_u8(vld1_u8(a2 - a_predecrement)), va_shift)),
        va_zero_point));
    const int16x8_t va3 = vreinterpretq_s16_u16(vsubl_u8(
        vreinterpret_u8_u64(vshl_u64(
            vreinterpret_u64_u8(vld1_u8(a3 - a_predecrement)), va_shift)),
        va_zero_point));

    {
      const int16x8_t vb01234567 = vreinterpretq_s16_u16(vsubl_u8(vld1_u8(b), vb_zero_point)); b += 8;

      vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vb01234567), vget_low_s16(va0), 0);
      vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vb01234567), vget_low_s16(va0), 0);
      vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vb01234567), vget_low_s16(va1), 0);
      vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vb01234567), vget_low_s16(va1), 0);
      vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vb01234567), vget_low_s16(va2), 0);
      vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vb01234567), vget_low_s16(va2), 0);
      vacc3x0123 = vmlal_lane_s16(vacc3x0123, vget_low_s16(vb01234567), vget_low_s16(va3), 0);
      vacc3x4567 = vmlal_lane_s16(vacc3x4567, vget_high_s16(vb01234567), vget_low_s16(va3), 0);
    }

    if (k >= 2) {
      const int16x8_t vb01234567 = vreinterpretq_s16_u16(vsubl_u8(vld1_u8(b), vb_zero_point)); b += 8;

      vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vb01234567), vget_low_s16(va0), 1);
      vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vb01234567), vget_low_s16(va0), 1);
      vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vb01234567), vget_low_s16(va1), 1);
      vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vb01234567), vget_low_s16(va1), 1);
      vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vb01234567), vget_low_s16(va2), 1);
      vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vb01234567), vget_low_s16(va2), 1);
      vacc3x0123 = vmlal_lane_s16(vacc3x0123, vget_low_s16(vb01234567), vget_low_s16(va3), 1);
      vacc3x4567 = vmlal_lane_s16(vacc3x4567, vget_high_s16(vb01234567), vget_low_s16(va3), 1);

      if (k >= 3) {
        const int16x8_t vb01234567 = vreinterpretq_s16_u16(vsubl_u8(vld1_u8(b), vb_zero_point)); b += 8;

        vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vb01234567), vget_low_s16(va0), 2);
        vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vb01234567), vget_low_s16(va0), 2);
        vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vb01234567), vget_low_s16(va1), 2);
        vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vb01234567), vget_low_s16(va1), 2);
        vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vb01234567), vget_low_s16(va2), 2);
        vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vb01234567), vget_low_s16(va2), 2);
        vacc3x0123 = vmlal_lane_s16(vacc3x0123, vget_low_s16(vb01234567), vget_low_s16(va3), 2);
        vacc3x4567 = vmlal_lane_s16(vacc3x4567, vget_high_s16(vb01234567), vget_low_s16(va3), 2);

        if (k >= 4) {
          const int16x8_t vb01234567 = vreinterpretq_s16_u16(vsubl_u8(vld1_u8(b), vb_zero_point)); b += 8;

          vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vb01234567), vget_low_s16(va0), 3);
          vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vb01234567), vget_low_s16(va0), 3);
          vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vb01234567), vget_low_s16(va1), 3);
          vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vb01234567), vget_low_s16(va1), 3);
          vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vb01234567), vget_low_s16(va2), 3);
          vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vb01234567), vget_low_s16(va2), 3);
          vacc3x0123 = vmlal_lane_s16(vacc3x0123, vget_low_s16(vb01234567), vget_low_s16(va3), 3);
          vacc3x4567 = vmlal_lane_s16(vacc3x4567, vget_high_s16(vb01234567), vget_low_s16(va3), 3);

          if (k >= 5) {
            const int16x8_t vb01234567 = vreinterpretq_s16_u16(vsubl_u8(vld1_u8(b), vb_zero_point)); b += 8;

            vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vb01234567), vget_high_s16(va0), 0);
            vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vb01234567), vget_high_s16(va0), 0);
            vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vb01234567), vget_high_s16(va1), 0);
            vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vb01234567), vget_high_s16(va1), 0);
            vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vb01234567), vget_high_s16(va2), 0);
            vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vb01234567), vget_high_s16(va2), 0);
            vacc3x0123 = vmlal_lane_s16(vacc3x0123, vget_low_s16(vb01234567), vget_high_s16(va3), 0);
            vacc3x4567 = vmlal_lane_s16(vacc3x4567, vget_high_s16(vb01234567), vget_high_s16(va3), 0);

            if (k >= 6) {
              const int16x8_t vb01234567 = vreinterpretq_s16_u16(vsubl_u8(vld1_u8(b), vb_zero_point)); b += 8;

              vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vb01234567), vget_high_s16(va0), 1);
              vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vb01234567), vget_high_s16(va0), 1);
              vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vb01234567), vget_high_s16(va1), 1);
              vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vb01234567), vget_high_s16(va1), 1);
              vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vb01234567), vget_high_s16(va2), 1);
              vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vb01234567), vget_high_s16(va2), 1);
              vacc3x0123 = vmlal_lane_s16(vacc3x0123, vget_low_s16(vb01234567), vget_high_s16(va3), 1);
              vacc3x4567 = vmlal_lane_s16(vacc3x4567, vget_high_s16(vb01234567), vget_high_s16(va3), 1);

              if (k >= 7) {
                const int16x8_t vb01234567 = vreinterpretq_s16_u16(vsubl_u8(vld1_u8(b), vb_zero_point)); b += 8;

                vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vb01234567), vget_high_s16(va0), 2);
                vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vb01234567), vget_high_s16(va0), 2);
                vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vb01234567), vget_high_s16(va1), 2);
                vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vb01234567), vget_high_s16(va1), 2);
                vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vb01234567), vget_high_s16(va2), 2);
                vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vb01234567), vget_high_s16(va2), 2);
                vacc3x0123 = vmlal_lane_s16(vacc3x0123, vget_low_s16(vb01234567), vget_high_s16(va3), 2);
                vacc3x4567 = vmlal_lane_s16(vacc3x4567, vget_high_s16(vb01234567), vget_high_s16(va3), 2);
              }
            }
          }
        }
      }
    }
  }

  const int32x4_t vmultiplier = vld1q_dup_s32(&quantization_params->neon.multiplier);
  vacc0x0123 = vqrdmulhq_s32(vacc0x0123, vmultiplier);
  vacc0x4567 = vqrdmulhq_s32(vacc0x4567, vmultiplier);
  vacc1x0123 = vqrdmulhq_s32(vacc1x0123, vmultiplier);
  vacc1x4567 = vqrdmulhq_s32(vacc1x4567, vmultiplier);
  vacc2x0123 = vqrdmulhq_s32(vacc2x0123, vmultiplier);
  vacc2x4567 = vqrdmulhq_s32(vacc2x4567, vmultiplier);
  vacc3x0123 = vqrdmulhq_s32(vacc3x0123, vmultiplier);
  vacc3x4567 = vqrdmulhq_s32(vacc3x4567, vmultiplier);

  const int32x4_t vright_shift = vld1q_dup_s32(&quantization_params->neon.right_shift);
  const int32x4_t vzero_shift_mask = vreinterpretq_s32_u32(vceqq_s32(vright_shift, vmovq_n_s32(0)));
  vacc0x0123 = vsraq_n_s32(vacc0x0123, vbicq_s32(vacc0x0123, vzero_shift_mask), 31);
  vacc0x4567 = vsraq_n_s32(vacc0x4567, vbicq_s32(vacc0x4567, vzero_shift_mask), 31);
  vacc1x0123 = vsraq_n_s32(vacc1x0123, vbicq_s32(vacc1x0123, vzero_shift_mask), 31);
  vacc1x4567 = vsraq_n_s32(vacc1x4567, vbicq_s32(vacc1x4567, vzero_shift_mask), 31);
  vacc2x0123 = vsraq_n_s32(vacc2x0123, vbicq_s32(vacc2x0123, vzero_shift_mask), 31);
  vacc2x4567 = vsraq_n_s32(vacc2x4567, vbicq_s32(vacc2x4567, vzero_shift_mask), 31);
  vacc3x0123 = vsraq_n_s32(vacc3x0123, vbicq_s32(vacc3x0123, vzero_shift_mask), 31);
  vacc3x4567 = vsraq_n_s32(vacc3x4567, vbicq_s32(vacc3x4567, vzero_shift_mask), 31);

  vacc0x0123 = vrshlq_s32(vacc0x0123, vright_shift);
  vacc0x4567 = vrshlq_s32(vacc0x4567, vright_shift);
  vacc1x0123 = vrshlq_s32(vacc1x0123, vright_shift);
  vacc1x4567 = vrshlq_s32(vacc1x4567, vright_shift);
  vacc2x0123 = vrshlq_s32(vacc2x0123, vright_shift);
  vacc2x4567 = vrshlq_s32(vacc2x4567, vright_shift);
  vacc3x0123 = vrshlq_s32(vacc3x0123, vright_shift);
  vacc3x4567 = vrshlq_s32(vacc3x4567, vright_shift);

  const int16x8_t voutput_zero_point = vld1q_dup_s16(&quantization_params->neon.output_zero_point);
#ifdef __aarch64__
  const int16x8_t vacc0x01234567 = vqaddq_s16(vqmovn_high_s32(vqmovn_s32(vacc0x0123), vacc0x4567), voutput_zero_point);
  const int16x8_t vacc1x01234567 = vqaddq_s16(vqmovn_high_s32(vqmovn_s32(vacc1x0123), vacc1x4567), voutput_zero_point);
  const int16x8_t vacc2x01234567 = vqaddq_s16(vqmovn_high_s32(vqmovn_s32(vacc2x0123), vacc2x4567), voutput_zero_point);
  const int16x8_t vacc3x01234567 = vqaddq_s16(vqmovn_high_s32(vqmovn_s32(vacc3x0123), vacc3x4567), voutput_zero_point);

  uint8x16_t vout0x01234567_1x01234567 = vqmovun_high_s16(vqmovun_s16(vacc0x01234567), vacc1x01234567);
  uint8x16_t vout2x01234567_3x01234567 = vqmovun_high_s16(vqmovun_s16(vacc2x01234567), vacc3x01234567);
#else
  const int16x8_t vacc0x01234567 = vqaddq_s16(vcombine_s16(vqmovn_s32(vacc0x0123), vqmovn_s32(vacc0x4567)), voutput_zero_point);
  const int16x8_t vacc1x01234567 = vqaddq_s16(vcombine_s16(vqmovn_s32(vacc1x0123), vqmovn_s32(vacc1x4567)), voutput_zero_point);
  const int16x8_t vacc2x01234567 = vqaddq_s16(vcombine_s16(vqmovn_s32(vacc2x0123), vqmovn_s32(vacc2x4567)), voutput_zero_point);
  const int16x8_t vacc3x01234567 = vqaddq_s16(vcombine_s16(vqmovn_s32(vacc3x0123), vqmovn_s32(vacc3x4567)), voutput_zero_point);

  uint8x16_t vout0x01234567_1x01234567 = vcombine_u8(vqmovun_s16(vacc0x01234567), vqmovun_s16(vacc1x01234567));
  uint8x16_t vout2x01234567_3x01234567 = vcombine_u8(vqmovun_s16(vacc2x01234567), vqmovun_s16(vacc3x01234567));
#endif
  const uint8x16_t voutput_min = vld1q_dup_u8(&quantization_params->neon.output_min);
  const uint8x16_t voutput_max = vld1q_dup_u8(&quantization_params->neon.output_max);

  vout0x01234567_1x01234567 = vmaxq_u8(vout0x01234567_1x01234567, voutput_min);
  vout2x01234567_3x01234567 = vmaxq_u8(vout2x01234567_3x01234567, voutput_min);
  vout0x01234567_1x01234567 = vminq_u8(vout0x01234567_1x01234567, voutput_max);
  vout2x01234567_3x01234567 = vminq_u8(vout2x01234567_3x01234567, voutput_max);

  uint8_t* c0 = c;
  uint8_t* c1 = (uint8_t*) ((uintptr_t) c0 + c_stride);
  if (mr < 2) {
    c1 = c0;
  }
  uint8_t* c2 = (uint8_t*) ((uintptr_t) c1 + c_stride);
  if (mr <= 2) {
    c2 = c1;
  }
  uint8_t* c3 = (uint8_t*) ((uintptr_t) c2 + c_stride);
  if (mr != 4) {
    c3 = c2;
  }
  if (nr == 8) {
    vst1_u8(c0, vget_low_u8(vout0x01234567_1x01234567));
    vst1_u8(c1, vget_high_u8(vout0x01234567_1x01234567));
    vst1_u8(c2, vget_low_u8(vout2x01234567_3x01234567));
    vst1_u8(c3, vget_high_u8(vout2x01234567_3x01234567));
  } else {
    if (nr >= 4) {
      vst1q_lane_u32(__builtin_assume_aligned(c0, 1), vreinterpretq_u32_u8(vout0x01234567_1x01234567), 0); c0 += 4;
      vst1q_lane_u32(__builtin_assume_aligned(c1, 1), vreinterpretq_u32_u8(vout0x01234567_1x01234567), 2); c1 += 4;
      vst1q_lane_u32(__builtin_assume_aligned(c2, 1), vreinterpretq_u32_u8(vout2x01234567_3x01234567), 0); c2 += 4;
      vst1q_lane_u32(__builtin_assume_aligned(c3, 1), vreinterpretq_u32_u8(vout2x01234567_3x01234567), 2); c3 += 4;
      vout0x01234567_1x01234567 = vextq_u8(vout0x01234567_1x01234567, vout0x01234567_1x01234567, 4);
      vout2x01234567_3x01234567 = vextq_u8(vout2x01234567_3x01234567, vout2x01234567_3x01234567, 4);
      nr -= 4;
    }
    if (nr >= 2) {
      vst1q_lane_u16(__builtin_assume_aligned(c0, 1), vreinterpretq_u16_u8(vout0x01234567_1x01234567), 0); c0 += 2;
      vst1q_lane_u16(__builtin_assume_aligned(c1, 1), vreinterpretq_u16_u8(vout0x01234567_1x01234567), 4); c1 += 2;
      vst1q_lane_u16(__builtin_assume_aligned(c2, 1), vreinterpretq_u16_u8(vout2x01234567_3x01234567), 0); c2 += 2;
      vst1q_lane_u16(__builtin_assume_aligned(c3, 1), vreinterpretq_u16_u8(vout2x01234567_3x01234567), 4); c3 += 2;
      vout0x01234567_1x01234567 = vextq_u8(vout0x01234567_1x01234567, vout0x01234567_1x01234567, 2);
      vout2x01234567_3x01234567 = vextq_u8(vout2x01234567_3x01234567, vout2x01234567_3x01234567, 2);
      nr -= 2;
    }
    if (nr != 0) {
      vst1q_lane_u8(c0, vout0x01234567_1x01234567, 0);
      vst1q_lane_u8(c1, vout0x01234567_1x01234567, 8);
      vst1q_lane_u8(c2, vout2x01234567_3x01234567, 0);
      vst1q_lane_u8(c3, vout2x01234567_3x01234567, 8);
    }
  }
}
