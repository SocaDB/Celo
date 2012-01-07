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
    // GET 

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
    inp_cont = 0; return req_GET();
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
    // POST 

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
    inp_cont = 0; return req_POST();
l_1_PU:
    if ( *data != 'T' ) goto e_400;
    if ( ++data >= end ) goto c_1_PUT;
l_1_PUT:
    if ( *data != ' ' ) goto e_400;
    if ( ++data >= end ) goto c_1_PUT_s;
l_1_PUT_s:
    // PUT 

l_4__beg: // first call
    url.data = data;
    while ( true ) {
        if ( *data == ' ' ) {
            url.size = data - url.data;
            *data = 0;
            goto e_4_;
        }
        if ( ++data == end ) {
            url.size = data - url.data;
            goto c_4__cnt;
        }
    }
l_4__cnt: // cont
    while ( true ) {
        url.make_room_for_at_least_1_char();
        if ( *data == ' ' ) {
            url.data[ url.size ] = 0;
            goto e_4_;
        }
        url.data[ url.size++ ] = *data;
        if ( ++data == end )
            goto c_4__cnt;
    }

e_4_:
    inp_cont = 0; return req_PUT();
c_1_P: inp_cont = &&l_1_P; goto str_cpy_if_needed;
c_1_G: inp_cont = &&l_1_G; goto str_cpy_if_needed;
c_1_GE: inp_cont = &&l_1_GE; goto str_cpy_if_needed;
c_1_GET: inp_cont = &&l_1_GET; goto str_cpy_if_needed;
c_1_GET_s: inp_cont = &&l_1_GET_s; goto str_cpy_if_needed;
c_2__cnt: inp_cont = &&l_2__cnt; goto str_cpy_if_needed;
c_1_PU: inp_cont = &&l_1_PU; goto str_cpy_if_needed;
c_1_PO: inp_cont = &&l_1_PO; goto str_cpy_if_needed;
c_1_POS: inp_cont = &&l_1_POS; goto str_cpy_if_needed;
c_1_POST: inp_cont = &&l_1_POST; goto str_cpy_if_needed;
c_1_POST_s: inp_cont = &&l_1_POST_s; goto str_cpy_if_needed;
c_3__cnt: inp_cont = &&l_3__cnt; goto str_cpy_if_needed;
c_1_PUT: inp_cont = &&l_1_PUT; goto str_cpy_if_needed;
c_1_PUT_s: inp_cont = &&l_1_PUT_s; goto str_cpy_if_needed;
c_4__cnt: inp_cont = &&l_4__cnt; goto str_cpy_if_needed;
str_cpy_if_needed:
    url.make_own_copy_if_not_the_case();
    return;
