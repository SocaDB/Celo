RA( "-o", output  , "file_name : name of the resulting library ('dl_req.so' by default)", "dl_req.so" );
RA( "-b", base_dir, "directory : directory of files that have to be served statically ('html' by default)", "html" );
RA( "-e", sipe_ext, "sipe_file : sipe file for additionnal definition and surdefinitions", "" );
RA( "-s", sipe_exe, "path      : path to the sipe executable", "" );
RA( "-a", base_str, "class name: name of the generated HttpRequest parent class", "EventObj_HttpRequest" );
RA( "-I", inc_path, "path: additionnal include path", "" );
