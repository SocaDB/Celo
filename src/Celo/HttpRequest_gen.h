if ( inp_cont )
    goto *inp_cont;

l_1_:
    if ( *data == 'P' ) { if ( ++data >= end ) goto c_1_P; goto l_1_P; }
    if ( *data != 'G' ) goto e_400;
    if ( ++data >= end ) goto c_1_G;
l_1_G:
    if ( *data != 'E' ) goto e_400;
    if ( ++data >= end ) goto c_1_GE;
l_1_GE:
    if ( *data != 'T' ) goto e_400;
    if ( ++data >= end ) goto c_1_GET;
l_1_GET:
    if ( *data != ' ' ) goto e_400;
    if ( ++data >= end ) goto c_1_GET_s;
l_1_GET_s:

l_2__beg: // first call
    url.data = data;
    while ( true ) {
        if ( *data == ' ' ) {
            url.size = data - url.data;
            *data = 0;
            goto e_2_;
        }
        if ( ++data == end ) {
            url.size = data - url.data;
            goto c_2__cnt;
        }
    }
l_2__cnt: // cont
    while ( true ) {
        url.make_room_for_at_least_1_char();
        if ( *data == ' ' ) {
            url.data[ url.size ] = 0;
            goto e_2_;
        }
        url.data[ url.size++ ] = *data;
        if ( ++data == end )
            goto c_2__cnt;
    }

e_2_:
    req_GET(); return false;
l_1_P:
    if ( *data == 'U' ) { if ( ++data >= end ) goto c_1_PU; goto l_1_PU; }
    if ( *data != 'O' ) goto e_400;
    if ( ++data >= end ) goto c_1_PO;
l_1_PO:
    if ( *data != 'S' ) goto e_400;
    if ( ++data >= end ) goto c_1_POS;
l_1_POS:
    if ( *data != 'T' ) goto e_400;
    if ( ++data >= end ) goto c_1_POST;
l_1_POST:
    if ( *data != ' ' ) goto e_400;
    if ( ++data >= end ) goto c_1_POST_s;
l_1_POST_s:

l_3__beg: // first call
    url.data = data;
    while ( true ) {
        if ( *data == ' ' ) {
            url.size = data - url.data;
            *data = 0;
            goto e_3_;
        }
        if ( ++data == end ) {
            url.size = data - url.data;
            goto c_3__cnt;
        }
    }
l_3__cnt: // cont
    while ( true ) {
        url.make_room_for_at_least_1_char();
        if ( *data == ' ' ) {
            url.data[ url.size ] = 0;
            goto e_3_;
        }
        url.data[ url.size++ ] = *data;
        if ( ++data == end )
            goto c_3__cnt;
    }

e_3_:
l_4_:
    if ( *data != '\n' ) { if ( ++data >= end ) goto c_4_; goto l_4_; }
    if ( ++data >= end ) goto c_4__n;
l_4__n:
    if ( *data == '\r' ) { if ( ++data >= end ) goto c_4__n_r; goto l_4__n_r; }
    if ( *data == '\n' ) { if ( ++data >= end ) goto c_4__n_n; goto l_4__n_n; }
    if ( *data != 'C' ) goto l_4_;
    if ( ++data >= end ) goto c_4__nC;
l_4__nC:
    if ( *data != 'o' ) goto l_4_;
    if ( ++data >= end ) goto c_4__nCo;
l_4__nCo:
    if ( *data != 'n' ) goto l_4_;
    if ( ++data >= end ) goto c_4__nCon;
l_4__nCon:
    if ( *data != 't' ) goto l_4_;
    if ( ++data >= end ) goto c_4__nCont;
l_4__nCont:
    if ( *data != 'e' ) goto l_4_;
    if ( ++data >= end ) goto c_4__nConte;
l_4__nConte:
    if ( *data != 'n' ) goto l_4_;
    if ( ++data >= end ) goto c_4__nConten;
l_4__nConten:
    if ( *data != 't' ) goto l_4_;
    if ( ++data >= end ) goto c_4__nContent;
l_4__nContent:
    if ( *data != '-' ) goto l_4_;
    if ( ++data >= end ) goto c_4__nContent_m;
l_4__nContent_m:
    if ( *data != 'L' ) goto l_4_;
    if ( ++data >= end ) goto c_4__nContent_mL;
l_4__nContent_mL:
    if ( *data != 'e' ) goto l_4_;
    if ( ++data >= end ) goto c_4__nContent_mLe;
l_4__nContent_mLe:
    if ( *data != 'n' ) goto l_4_;
    if ( ++data >= end ) goto c_4__nContent_mLen;
l_4__nContent_mLen:
    if ( *data != 'g' ) goto l_4_;
    if ( ++data >= end ) goto c_4__nContent_mLeng;
l_4__nContent_mLeng:
    if ( *data != 't' ) goto l_4_;
    if ( ++data >= end ) goto c_4__nContent_mLengt;
l_4__nContent_mLengt:
    if ( *data != 'h' ) goto l_4_;
    if ( ++data >= end ) goto c_4__nContent_mLength;
l_4__nContent_mLength:
    if ( *data != ':' ) goto l_4_;
    if ( ++data >= end ) goto c_4__nContent_mLength_d;
l_4__nContent_mLength_d:
    if ( *data != ' ' ) goto l_4_;
    if ( ++data >= end ) goto c_4__nContent_mLength_d_s;
l_4__nContent_mLength_d_s:

l_5_:
    if ( *data < '0' or *data > '9' ) goto e_5_;
    content_length = 10 * content_length + ( *data - '0' );
    if ( ++data >= end ) goto c_5_;
    goto l_5_;
e_5_:
    goto l_4_;
l_4__n_n:
    req_POST( data, end - data ); return false;
l_4__n_r:
    if ( *data != '\n' ) goto l_4_;
    if ( ++data >= end ) goto c_4__n_r_n;
l_4__n_r_n:
    req_POST( data, end - data ); return false;
l_1_PU:
    if ( *data != 'T' ) goto e_400;
    if ( ++data >= end ) goto c_1_PUT;
l_1_PUT:
    if ( *data != ' ' ) goto e_400;
    if ( ++data >= end ) goto c_1_PUT_s;
l_1_PUT_s:

l_6__beg: // first call
    url.data = data;
    while ( true ) {
        if ( *data == ' ' ) {
            url.size = data - url.data;
            *data = 0;
            goto e_6_;
        }
        if ( ++data == end ) {
            url.size = data - url.data;
            goto c_6__cnt;
        }
    }
l_6__cnt: // cont
    while ( true ) {
        url.make_room_for_at_least_1_char();
        if ( *data == ' ' ) {
            url.data[ url.size ] = 0;
            goto e_6_;
        }
        url.data[ url.size++ ] = *data;
        if ( ++data == end )
            goto c_6__cnt;
    }

e_6_:
l_7_:
    if ( *data != '\n' ) { if ( ++data >= end ) goto c_7_; goto l_7_; }
    if ( ++data >= end ) goto c_7__n;
l_7__n:
    if ( *data == '\r' ) { if ( ++data >= end ) goto c_7__n_r; goto l_7__n_r; }
    if ( *data == '\n' ) { if ( ++data >= end ) goto c_7__n_n; goto l_7__n_n; }
    if ( *data != 'C' ) goto l_7_;
    if ( ++data >= end ) goto c_7__nC;
l_7__nC:
    if ( *data != 'o' ) goto l_7_;
    if ( ++data >= end ) goto c_7__nCo;
l_7__nCo:
    if ( *data != 'n' ) goto l_7_;
    if ( ++data >= end ) goto c_7__nCon;
l_7__nCon:
    if ( *data != 't' ) goto l_7_;
    if ( ++data >= end ) goto c_7__nCont;
l_7__nCont:
    if ( *data != 'e' ) goto l_7_;
    if ( ++data >= end ) goto c_7__nConte;
l_7__nConte:
    if ( *data != 'n' ) goto l_7_;
    if ( ++data >= end ) goto c_7__nConten;
l_7__nConten:
    if ( *data != 't' ) goto l_7_;
    if ( ++data >= end ) goto c_7__nContent;
l_7__nContent:
    if ( *data != '-' ) goto l_7_;
    if ( ++data >= end ) goto c_7__nContent_m;
l_7__nContent_m:
    if ( *data != 'L' ) goto l_7_;
    if ( ++data >= end ) goto c_7__nContent_mL;
l_7__nContent_mL:
    if ( *data != 'e' ) goto l_7_;
    if ( ++data >= end ) goto c_7__nContent_mLe;
l_7__nContent_mLe:
    if ( *data != 'n' ) goto l_7_;
    if ( ++data >= end ) goto c_7__nContent_mLen;
l_7__nContent_mLen:
    if ( *data != 'g' ) goto l_7_;
    if ( ++data >= end ) goto c_7__nContent_mLeng;
l_7__nContent_mLeng:
    if ( *data != 't' ) goto l_7_;
    if ( ++data >= end ) goto c_7__nContent_mLengt;
l_7__nContent_mLengt:
    if ( *data != 'h' ) goto l_7_;
    if ( ++data >= end ) goto c_7__nContent_mLength;
l_7__nContent_mLength:
    if ( *data != ':' ) goto l_7_;
    if ( ++data >= end ) goto c_7__nContent_mLength_d;
l_7__nContent_mLength_d:
    if ( *data != ' ' ) goto l_7_;
    if ( ++data >= end ) goto c_7__nContent_mLength_d_s;
l_7__nContent_mLength_d_s:

l_8_:
    if ( *data < '0' or *data > '9' ) goto e_8_;
    content_length = 10 * content_length + ( *data - '0' );
    if ( ++data >= end ) goto c_8_;
    goto l_8_;
e_8_:
    goto l_7_;
l_7__n_n:
    req_PUT( data, end - data ); return false;
l_7__n_r:
    if ( *data != '\n' ) goto l_7_;
    if ( ++data >= end ) goto c_7__n_r_n;
l_7__n_r_n:
    req_PUT( data, end - data ); return false;

#define ERR( NUM, MSG ) e_##NUM: error_##NUM(); return false;
#include "ErrorCodes.h"
#undef ERR
c_1_P: inp_cont = &&l_1_P; goto mocintc_;
c_1_G: inp_cont = &&l_1_G; goto mocintc_;
c_1_GE: inp_cont = &&l_1_GE; goto mocintc_;
c_1_GET: inp_cont = &&l_1_GET; goto mocintc_;
c_1_GET_s: inp_cont = &&l_1_GET_s; goto mocintc_;
c_2__cnt: inp_cont = &&l_2__cnt; goto mocintc_url;
c_1_PU: inp_cont = &&l_1_PU; goto mocintc_;
c_1_PO: inp_cont = &&l_1_PO; goto mocintc_;
c_1_POS: inp_cont = &&l_1_POS; goto mocintc_;
c_1_POST: inp_cont = &&l_1_POST; goto mocintc_;
c_1_POST_s: inp_cont = &&l_1_POST_s; goto mocintc_;
c_3__cnt: inp_cont = &&l_3__cnt; goto mocintc_url;
c_4_: inp_cont = &&l_4_; goto mocintc_url;
c_4__n: inp_cont = &&l_4__n; goto mocintc_url;
c_4__n_r: inp_cont = &&l_4__n_r; goto mocintc_url;
c_4__n_n: inp_cont = &&l_4__n_n; goto mocintc_url;
c_4__nC: inp_cont = &&l_4__nC; goto mocintc_url;
c_4__nCo: inp_cont = &&l_4__nCo; goto mocintc_url;
c_4__nCon: inp_cont = &&l_4__nCon; goto mocintc_url;
c_4__nCont: inp_cont = &&l_4__nCont; goto mocintc_url;
c_4__nConte: inp_cont = &&l_4__nConte; goto mocintc_url;
c_4__nConten: inp_cont = &&l_4__nConten; goto mocintc_url;
c_4__nContent: inp_cont = &&l_4__nContent; goto mocintc_url;
c_4__nContent_m: inp_cont = &&l_4__nContent_m; goto mocintc_url;
c_4__nContent_mL: inp_cont = &&l_4__nContent_mL; goto mocintc_url;
c_4__nContent_mLe: inp_cont = &&l_4__nContent_mLe; goto mocintc_url;
c_4__nContent_mLen: inp_cont = &&l_4__nContent_mLen; goto mocintc_url;
c_4__nContent_mLeng: inp_cont = &&l_4__nContent_mLeng; goto mocintc_url;
c_4__nContent_mLengt: inp_cont = &&l_4__nContent_mLengt; goto mocintc_url;
c_4__nContent_mLength: inp_cont = &&l_4__nContent_mLength; goto mocintc_url;
c_4__nContent_mLength_d: inp_cont = &&l_4__nContent_mLength_d; goto mocintc_url;
c_4__nContent_mLength_d_s: inp_cont = &&l_4__nContent_mLength_d_s; goto mocintc_url;
c_5_: inp_cont = &&l_5_; goto mocintc_url;
c_4__n_r_n: inp_cont = &&l_4__n_r_n; goto mocintc_url;
c_1_PUT: inp_cont = &&l_1_PUT; goto mocintc_;
c_1_PUT_s: inp_cont = &&l_1_PUT_s; goto mocintc_;
c_6__cnt: inp_cont = &&l_6__cnt; goto mocintc_url;
c_7_: inp_cont = &&l_7_; goto mocintc_url;
c_7__n: inp_cont = &&l_7__n; goto mocintc_url;
c_7__n_r: inp_cont = &&l_7__n_r; goto mocintc_url;
c_7__n_n: inp_cont = &&l_7__n_n; goto mocintc_url;
c_7__nC: inp_cont = &&l_7__nC; goto mocintc_url;
c_7__nCo: inp_cont = &&l_7__nCo; goto mocintc_url;
c_7__nCon: inp_cont = &&l_7__nCon; goto mocintc_url;
c_7__nCont: inp_cont = &&l_7__nCont; goto mocintc_url;
c_7__nConte: inp_cont = &&l_7__nConte; goto mocintc_url;
c_7__nConten: inp_cont = &&l_7__nConten; goto mocintc_url;
c_7__nContent: inp_cont = &&l_7__nContent; goto mocintc_url;
c_7__nContent_m: inp_cont = &&l_7__nContent_m; goto mocintc_url;
c_7__nContent_mL: inp_cont = &&l_7__nContent_mL; goto mocintc_url;
c_7__nContent_mLe: inp_cont = &&l_7__nContent_mLe; goto mocintc_url;
c_7__nContent_mLen: inp_cont = &&l_7__nContent_mLen; goto mocintc_url;
c_7__nContent_mLeng: inp_cont = &&l_7__nContent_mLeng; goto mocintc_url;
c_7__nContent_mLengt: inp_cont = &&l_7__nContent_mLengt; goto mocintc_url;
c_7__nContent_mLength: inp_cont = &&l_7__nContent_mLength; goto mocintc_url;
c_7__nContent_mLength_d: inp_cont = &&l_7__nContent_mLength_d; goto mocintc_url;
c_7__nContent_mLength_d_s: inp_cont = &&l_7__nContent_mLength_d_s; goto mocintc_url;
c_8_: inp_cont = &&l_8_; goto mocintc_url;
c_7__n_r_n: inp_cont = &&l_7__n_r_n; goto mocintc_url;
mocintc_url:
    url.make_own_copy_if_not_the_case();
    return;
mocintc_:
    return;
