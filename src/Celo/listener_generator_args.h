/*
 Copyright 2012 Structure Computation  www.structure-computation.com
 Copyright 2012 Hugo Leclerc

 This file is part of Celo.

 Celo is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Celo is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with Celo. If not, see <http://www.gnu.org/licenses/>.
*/


RA( "-o", output  , "file_name : name of the resulting library ('dl_req.so' by default)", "dl_req.so" );
RA( "-b", base_dir, "directory : directory of files that have to be served statically ('html' by default)", "html" );
RA( "-e", sipe_ext, "sipe_file : sipe file for additionnal definition and surdefinitions", "" );
RA( "-s", sipe_exe, "path      : path to the sipe executable", "" );
RA( "-a", base_str, "class name: name of the generated HttpRequest parent class", "EventObj_HttpRequest" );
RA( "-I", inc_path, "path: additionnal include path", "" );
