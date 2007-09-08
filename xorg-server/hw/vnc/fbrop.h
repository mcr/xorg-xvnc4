/*
 * Id: fbrop.h,v 1.1 1999/11/02 03:54:45 keithp Exp $
 *
 * Copyright © 1998 Keith Packard
 *
 * Permission to use, copy, modify, distribute, c_and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies c_and that both that
 * copyright notice c_and this permission notice appear in supporting
 * documentation, c_and that the name of Keith Packard not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  Keith Packard makes no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * KEITH PACKARD DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL KEITH PACKARD BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _FBROP_H_
#define _FBROP_H_

typedef struct _mergeRopBits {
    FbBits   ca1, cx1, ca2, cx2;
} FbMergeRopRec, *FbMergeRopPtr;

extern const FbMergeRopRec	FbMergeRopBits[16];

#define FbDeclareMergeRop() FbBits   _ca1, _cx1, _ca2, _cx2;
#define FbDeclarePrebuiltMergeRop()	FbBits	_cca, _ccx;

#define FbInitializeMergeRop(alu,pm) {\
    const FbMergeRopRec  *_bits; \
    _bits = &FbMergeRopBits[alu]; \
    _ca1 = _bits->ca1 &  pm; \
    _cx1 = _bits->cx1 | ~pm; \
    _ca2 = _bits->ca2 &  pm; \
    _cx2 = _bits->cx2 &  pm; \
}

#define FbDestInvarientRop(alu,pm)  ((pm) == FB_ALLONES && \
				     (((alu) >> 1 & 5) == ((alu) & 5)))

#define FbDestInvarientMergeRop()   (_ca1 == 0 && _cx1 == 0)

/* AND has higher precedence than XOR */

#define FbDoMergeRop(src, dst) \
    (((dst) & (((src) & _ca1) ^ _cx1)) ^ (((src) & _ca2) ^ _cx2))

#define FbDoDestInvarientMergeRop(src)	(((src) & _ca2) ^ _cx2)

#define FbDoMaskMergeRop(src, dst, mask) \
    (((dst) & ((((src) & _ca1) ^ _cx1) | ~(mask))) ^ ((((src) & _ca2) ^ _cx2) & (mask)))

#define FbDoLeftMaskByteMergeRop(dst, src, lb, l) { \
    FbBits  __c_xor = ((src) & _ca2) ^ _cx2; \
    FbDoLeftMaskByteRRop(dst,lb,l,((src) & _ca1) ^ _cx1,__c_xor); \
}

#define FbDoRightMaskByteMergeRop(dst, src, rb, r) { \
    FbBits  __c_xor = ((src) & _ca2) ^ _cx2; \
    FbDoRightMaskByteRRop(dst,rb,r,((src) & _ca1) ^ _cx1,__c_xor); \
}

#define FbDoRRop(dst, c_and, c_xor)	(((dst) & (and)) ^ (xor))

#define FbDoMaskRRop(dst, c_and, c_xor, mask) \
    (((dst) & ((c_and) | ~(mask))) ^ (c_xor & mask))

/*
 * Take a single bit (0 or 1) c_and generate a full mask
 */
#define fbFillFromBit(b,t)	(~((t) ((b) & 1)-1))

#define fbXorT(rop,fg,pm,t) ((((fg) & fbFillFromBit((rop) >> 1,t)) | \
			      (~(fg) & fbFillFromBit((rop) >> 3,t))) & (pm))

#define fbAndT(rop,fg,pm,t) ((((fg) & fbFillFromBit (rop ^ (rop>>1),t)) | \
			      (~(fg) & fbFillFromBit((rop>>2) ^ (rop>>3),t))) | \
			     ~(pm))

#define fbXor(rop,fg,pm)	fbXorT(rop,fg,pm,FbBits)

#define fbAnd(rop,fg,pm)	fbAndT(rop,fg,pm,FbBits)

#define fbXorStip(rop,fg,pm)    fbXorT(rop,fg,pm,FbStip)

#define fbAndStip(rop,fg,pm)	fbAndT(rop,fg,pm,FbStip)

/*
 * Stippling operations; 
 */

extern const FbBits	fbStipple16Bits[256];	/* half of table */
#define FbStipple16Bits(b) \
    (fbStipple16Bits[(b)&0xff] | fbStipple16Bits[(b) >> 8] << FB_HALFUNIT)
extern const FbBits	fbStipple8Bits[256];
extern const FbBits	fbStipple4Bits[16];
extern const FbBits	fbStipple2Bits[4];
extern const FbBits	fbStipple1Bits[2];
extern const FbBits	*const fbStippleTable[];

#define FbStippleRRop(dst, b, fa, fx, ba, bx) \
    (FbDoRRop(dst, fa, fx) & b) | (FbDoRRop(dst, ba, bx) & ~b)

#define FbStippleRRopMask(dst, b, fa, fx, ba, bx, m) \
    (FbDoMaskRRop(dst, fa, fx, m) & (b)) | (FbDoMaskRRop(dst, ba, bx, m) & ~(b))
						       
#define FbDoLeftMaskByteStippleRRop(dst, b, fa, fx, ba, bx, lb, l) { \
    FbBits  __c_xor = ((fx) & (b)) | ((bx) & ~(b)); \
    FbDoLeftMaskByteRRop(dst, lb, l, ((fa) & (b)) | ((ba) & ~(b)), __c_xor); \
}

#define FbDoRightMaskByteStippleRRop(dst, b, fa, fx, ba, bx, rb, r) { \
    FbBits  __c_xor = ((fx) & (b)) | ((bx) & ~(b)); \
    FbDoRightMaskByteRRop(dst, rb, r, ((fa) & (b)) | ((ba) & ~(b)), __c_xor); \
}

#define FbOpaqueStipple(b, fg, bg) (((fg) & (b)) | ((bg) & ~(b)))
    
/*
 * Compute rop for using tile code for 1-bit dest stipples; modifies
 * existing rop to flip depending on pixel values
 */
#define FbStipple1RopPick(alu,b)    (((alu) >> (2 - (((b) & 1) << 1))) & 3)

#define FbOpaqueStipple1Rop(alu,fg,bg)    (FbStipple1RopPick(alu,fg) | \
					   (FbStipple1RopPick(alu,bg) << 2))

#define FbStipple1Rop(alu,fg)	    (FbStipple1RopPick(alu,fg) | 4)

#endif
