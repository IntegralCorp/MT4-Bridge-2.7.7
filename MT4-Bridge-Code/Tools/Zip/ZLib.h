//+------------------------------------------------------------------+
//|                                                                  |
//|                   Copyright 2000-2019, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
//+----------------------------------------------------------------------------+
//| Copyright (C) 1995-2017 Jean-loup Gailly and Mark Adler                    |
//|                                                                            |
//| This software is provided 'as-is', without any express or implied          |
//| warranty.  In no event will the authors be held liable for any damages     |
//| arising from the use of this software.                                     |
//|                                                                            |
//| Permission is granted to anyone to use this software for any purpose,      |
//| including commercial applications, and to alter it and redistribute it     |
//| freely, subject to the following restrictions:                             |
//|                                                                            |
//| 1. The origin of this software must not be misrepresented; you must not    |
//|    claim that you wrote the original software. If you use this software    |
//|    in a product, an acknowledgment in the product documentation would be   |
//|    appreciated but is not required.                                        |
//| 2. Altered source versions must be plainly marked as such, and must not be |
//|    misrepresented as being the original software.                          |
//| 3. This notice may not be removed or altered from any source distribution. |
//|                                                                            |
//| Jean-loup Gailly        Mark Adler                                         |
//| jloup@gzip.org          madler@alumni.caltech.edu                          |
//|                                                                            |
//|                                                                            |
//| The data format used by the zlib library is described by RFCs (Request for |
//| Comments) 1950 to 1952 in the files http://tools.ietf.org/html/rfc1950     |
//| (zlib format), rfc1951 (deflate format) and rfc1952 (gzip format).         |
//+----------------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| ZLib                                                             |
//+------------------------------------------------------------------+
class SZlib
  {
public:
   //+------------------------------------------------------------------+
   //| Declaration                                                      |
   //+------------------------------------------------------------------+
   struct            z_stream;
   struct            ct_data;
   struct            tree_desc;
   struct            internal_state;
   struct            gz_header;
   struct            code;
   struct            inflate_state;
   struct            static_tree_desc;
   struct            config;
   //+------------------------------------------------------------------+
   //| Constants                                                        |
   //+------------------------------------------------------------------+
   enum
     {
      //---
      MAX_WBITS             =15,
      //---
      Z_DEFLATED            =8,
      //---
      Z_BUFSIZE             =16384,
      //---
      DEF_MEM_LEVEL         =8,
      //---
      Z_DEFAULT_STRATEGY    =0,
      //---
      Z_NO_FLUSH            =0,
      Z_PARTIAL_FLUSH       =1,
      Z_SYNC_FLUSH          =2,
      Z_FULL_FLUSH          =3,
      Z_FINISH              =4,
      Z_BLOCK               =5,
      //---
      Z_OK                  =0,
      Z_STREAM_END          =1,
      Z_NEED_DICT           =2,
      Z_ERRNO               =-1,
      Z_STREAM_ERROR        =-2,
      Z_DATA_ERROR          =-3,
      Z_MEM_ERROR           =-4,
      Z_BUF_ERROR           =-5,
      Z_VERSION_ERROR       =-6,
      //---
      Z_NO_COMPRESSION      =0,
      Z_BEST_SPEED          =1,
      Z_BEST_COMPRESSION    =9,
      Z_DEFAULT_COMPRESSION =-1,
      //--- Magic numbers
      LOCALHEADERMAGIC      =0x04034b50,
      CENTRALHEADERMAGIC    =0x02014b50,
      ENDOFCENTRALDIRMAGIC  =0x06054b50,
      ZIP64ENDOFCENTDIRMAGIC=0x06064b50,
      ZIP64EOCD_LOCATORMAGIC=0x07064b50,
      //--- offsets
      OFFSET_LOCAL_FILENAME =30,
      OFFSET_ZIP64EOCD_CDOFF=48,
      OFFSET_CENTDIR_FNAME  =46,
      OFFSET_LOCAL_COMPSIZE =14,
      //--- extra header id
      EXTRAHEADERID_ZIP64   =0x0001,
      //--- 
     };

private:
   //+------------------------------------------------------------------+
   //| Enumerations                                                     |
   //+------------------------------------------------------------------+
   enum              inflate_mode { HEAD, FLAGS, TIME, OS, EXLEN,  EXTRA, NAME, COMMENT, HCRC, DICTID, DICT, TYPE, TYPEDO, STORED, COPY, TABLE, LENLENS, CODELENS,LEN, LENEXT, DIST, DISTEXT, MATCH, LIT, CHECK, LENGTH, DONE, BAD, MEM,SYNC };
   enum              codetype     { CODES, LENS, DISTS };
   enum              block_state  { NEED_MORE, BLOCK_DONE, FINISH_STARTED, FINISH_DONE };
   //+------------------------------------------------------------------+
   //| Prototypes                                                       |
   //+------------------------------------------------------------------+
   typedef void*       (*func_alloc) (UINT items,UINT size);
   typedef void        (*func_free) (void* address);
   typedef block_state (*func_compress)(internal_state *s,int flush);

public:
   //+------------------------------------------------------------------+
   //| Structures                                                       |
   //+------------------------------------------------------------------+
   struct z_stream
     {
      char             *next_in;
      UINT              avail_in;
      UINT64            total_in;
      char             *next_out;
      UINT              avail_out;
      UINT64            total_out;
      internal_state   *state;
      func_alloc        zalloc;
      func_free         zfree;
      int               data_type;
      UINT              adler;
      UINT              reserved;
     };
   //---
   struct ct_data
     {
      union
        {
         WORD              freq;
         WORD              code;
        }               fc;
      union
        {
         WORD              dad;
         WORD              len;
        }               dl;
     };
   //---
   struct tree_desc
     {
      ct_data          *dyn_tree;
      int               max_code;
      const static_tree_desc *stat_desc;
     };
   //---
   struct internal_state
     {
      z_stream         *strm;
      int               status;
      char             *pending_buf;
      UINT              pending_buf_size;
      char             *pending_out;
      UINT              pending;
      int               wrap;
      gz_header        *gzhead;
      UINT              gzindex;
      char              method;
      int               last_flush;
      UINT              w_size;
      UINT              w_bits;
      UINT              w_mask;
      char             *window;
      UINT              window_size;
      WORD             *prev;
      WORD             *head;
      UINT              ins_h;
      UINT              hash_size;
      UINT              hash_bits;
      UINT              hash_mask;
      UINT              hash_shift;
      long              block_start;
      UINT              match_length;
      unsigned          prev_match;
      int               match_available;
      UINT              strstart;
      UINT              match_start;
      UINT              lookahead;
      UINT              prev_length;
      UINT              max_chain_length;
      UINT              max_lazy_match;
      int               level;
      int               strategy;
      UINT              good_match;
      int               nice_match;
      ct_data           dyn_ltree[573];
      ct_data           dyn_dtree[61];
      ct_data           bl_tree[39];
      tree_desc         l_desc;
      tree_desc         d_desc;
      tree_desc         bl_desc;
      WORD              bl_count[16];
      int               heap[573];
      int               heap_len;
      int               heap_max;
      BYTE              depth[573];
      BYTE             *l_buf;
      UINT              lit_bufsize;
      UINT              last_lit;
      WORD             *d_buf;
      UINT              opt_len;
      UINT              static_len;
      UINT              matches;
      int               last_eob_len;
      WORD              bi_buf;
      int               bi_valid;
     };
   //---
   struct gz_header
     {
      int               text;
      UINT              time;
      int               xflags;
      int               os;
      char             *extra;
      UINT              extra_len;
      UINT              extra_max;
      char             *name;
      UINT              name_max;
      char             *comment;
      UINT              comm_max;
      int               hcrc;
      int               done;
     };
   //---
   struct code
     {
      BYTE              op;
      BYTE              bits;
      WORD              val;
     };
   //---
   struct inflate_state
     {
      inflate_mode      mode;
      int               last;
      int               wrap;
      int               havedict;
      int               flags;
      unsigned          dmax;
      UINT              check;
      UINT              total;
      gz_header        *head;
      unsigned          wbits;
      unsigned          wsize;
      unsigned          whave;
      unsigned          write;
      BYTE             *window;
      UINT              hold;
      unsigned          bits;
      unsigned          length;
      unsigned          offset;
      unsigned          extra;
      code const       *lencode;
      code const       *distcode;
      unsigned          lenbits;
      unsigned          distbits;
      unsigned          ncode;
      unsigned          nlen;
      unsigned          ndist;
      unsigned          have;
      code             *next;
      WORD              lens[320];
      WORD              work[288];
      code              codes[2048];
     };
   //---
   struct static_tree_desc
     {
      const ct_data    *static_tree;
      const int        *extra_bits;
      int               extra_base;
      int               elems;
      int               max_length;
     };
   //---
   struct config
     {
      WORD              good_length;
      WORD              max_lazy;
      WORD              nice_length;
      WORD              max_chain;
      func_compress     func;
     };
   //+------------------------------------------------------------------+
   //| Tables                                                           |
   //+------------------------------------------------------------------+
   static const config configuration_table[10];
   static const UINT crc_table[8][256];
   static const int  base_length[29];
   static const int  base_dist[30];
   static const BYTE length_code[256];
   static const BYTE dist_code[512];
   static const BYTE bl_order[19];
   static const ct_data static_ltree[288];
   static const ct_data static_dtree[30];
   static const int  extra_lbits[29];
   static const int  extra_dbits[30];
   static const int  extra_blbits[19];
   static const static_tree_desc static_l_desc;
   static const static_tree_desc static_d_desc;
   static const static_tree_desc static_bl_desc;

public:
   //--- deflate
   static int        deflateInit2(z_stream *strm,int level,int method,int windowBits,int memLevel,int strategy);
   static int        deflate(z_stream *strm,int flush);
   static int        deflateEnd(z_stream *strm);
   //--- inflate
   static int        inflateInit2(z_stream *strm,int windowBits);
   static int        inflate(z_stream *strm,int flush);
   static int        inflateEnd(z_stream *strm);
   //--- CRC
   static UINT       crc32(UINT crc,const BYTE *buf,unsigned len);

private:
   //--- memory routines
   static void*      zcalloc(unsigned items,unsigned size);
   static void       zcfree(void *ptr);
   //--- deflate
   static block_state deflate_stored(internal_state *s,int flush);
   static block_state deflate_fast(internal_state *s,int flush);
   static block_state deflate_slow(internal_state *s,int flush);
   static int        deflateReset(z_stream *strm);
   //--- inflate
   static void       inflate_fast(z_stream *strm, unsigned start);
   static int        inflateReset(z_stream *strm);
   static int        inflate_table(codetype type,const WORD *lens,unsigned codes,code  **table,unsigned *bits,WORD *work);
   //--- CRC
   static UINT       crc32_little(UINT crc,const BYTE *buf,unsigned len);
   static UINT       crc32_big(UINT crc,const BYTE  *buf,unsigned len);
   //---
   static UINT       adler32(UINT adler,const BYTE *buf,UINT len);
   //---
   static void       _tr_init(internal_state *s);
   static void       _tr_align(internal_state *s);
   static void       _tr_stored_block(internal_state *s,const char* buf,UINT stored_len,int eof);
   static void       _tr_flush_block(internal_state *s,char *buf,UINT stored_len,int eof);
   //---
   static void       bi_windup(internal_state *s);
   static void       bi_flush(internal_state *s);
   static unsigned   bi_reverse(unsigned ncode,int len);
   //---
   static void       init_block(internal_state *s);
   static void       copy_block(internal_state *s,const char *buf,unsigned len,int header);
   static void       compress_block(internal_state *s,const ct_data *ltree,const ct_data *dtree);
   //---
   static void       fill_window(internal_state *s);
   static int        updatewindow(const z_stream *strm,unsigned out);
   //---
   static UINT       longest_match(internal_state *s,unsigned cur_match);
   static UINT       longest_match_fast(internal_state *s,unsigned cur_match);
   //---
   static void       build_tree(internal_state *s,tree_desc *desc);
   static int        build_bl_tree(internal_state *s);
   static void       send_tree(internal_state *s,const ct_data *tree,int max_code);
   static void       send_all_trees(internal_state *s, int lcodes, int dcodes,int blcodes);
   static void       scan_tree(internal_state *s,ct_data *tree,int max_code);
   static void       gen_codes (ct_data *tree,int max_code,const WORD *bl_count);
   static void       gen_bitlen(internal_state *s,const tree_desc *desc);
   //---
   static int        read_buf(z_stream *strm,char *buf,unsigned size);
   static void       flush_pending(z_stream *strm);
   static void       lm_init(internal_state *s);
   static void       putShortMSB(internal_state *s,UINT b);
   static void       set_data_type(const internal_state *s);
   static void       pqdownheap(internal_state *s,const ct_data *tree,int k);
   static void       fixedtables(inflate_state *state);
  };
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
  